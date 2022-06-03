function outsig = hades_synthesis(synthesis_pars, signal_container, parameter_container)

% Local variables 
hopsize = synthesis_pars.hopsize;
blocksize = synthesis_pars.blocksize;
timeslots = synthesis_pars.timeslots;
nBands = length(synthesis_pars.centreFreq);  
nBlocks =  size(signal_container.inTF,4);
nMics = size(signal_container.inTF,1);
nGrid = size(synthesis_pars.grid_dirs_deg,1);
ref_inds = synthesis_pars.ref_inds;
 
% Initialise the time-frequency transform
if synthesis_pars.hybridMode>0
    if synthesis_pars.LDmode>0
        afSTFT(hopsize,1,2,'hybrid','low_delay'); 
        afSTFTdelay = 7*hopsize;
    else
        afSTFT(hopsize,1,2,'hybrid');
        afSTFTdelay = 12*hopsize;
    end
else
    if synthesis_pars.LDmode>0
        afSTFT(hopsize,1,2,'low_delay'); 
        afSTFTdelay = 4*hopsize;
    else
        afSTFT(hopsize,1,2);  
        afSTFTdelay = 9*hopsize;
    end
end

% Comment if not calculating metrics
outsig = zeros(afSTFTdelay+blocksize*(nBlocks+3), 2);

% Array steering vectors:
A_grid = permute(synthesis_pars.H_grid(:,:,:), [2 3 1]);  

% Integration weights
if mean(abs(synthesis_pars.grid_dirs_deg(:,2)*pi/180))<0.0001
    W = (1/nGrid).* diag(ones(size(synthesis_pars.grid_dirs_deg,1),1));
else
    W = diag(getVoronoiWeights(synthesis_pars.grid_dirs_deg*pi/180))./(4*pi);
end

% Diffuse-coherence matrices for the HRTF set and employed microphone array
H_bin_dcm = zeros(2,2,nBands);
H_grid_dcm = zeros(2,2,nBands);
for band=1:nBands
    h_bin_nb = synthesis_pars.hrtfs(:,:,band);
    h_grid_nb = A_grid(ref_inds,:,band);
    H_bin_dcm(:,:,band)  = (h_bin_nb  * W * h_bin_nb');
    H_grid_dcm(:,:,band) = (h_grid_nb * W * h_grid_nb');
end

% Equalisation term to align the diffuse-field response as captured by
% the array, to instead be of that as captured by the HRTF set:
diff_eq = zeros(nBands,1); 
for band = 1:nBands, diff_eq(band) = sqrt(real(trace(H_bin_dcm(:,:,band))/(trace(H_grid_dcm(:,:,band))+eps))); end
    
% Run-time variables
M = zeros(2,nMics,nBands); 

% Process loop
startIndex = 1;
blockIndex = 1;
progress = 1; 
while blockIndex <= nBlocks
    % display progress
    if (startIndex+blocksize)*100/(hopsize*(nBlocks+1)) > progress
        fprintf('*'); if mod(progress,10)==0, fprintf('\n'), end
        progress=progress+1;
    end 

    % TF signals and spatial covariance matrices per band for this block
    inTF = signal_container.inTF(:,:,:,blockIndex);   
    Cx = signal_container.Cx(:,:,:,blockIndex);  
    
    % Estimated spatial parameters corresponding to this block of audio
    est_idx = parameter_container.doa_idx(:,blockIndex);
    diffuseness = parameter_container.diffuseness(:,blockIndex);

    % Input energy for each band
    energy_per_band = zeros(nBands,1); 
    for band = 1:nBands, energy_per_band(band) = real(trace(Cx(:,:,band))); end
    
    % Compute mixing matrix per band
    for band=1:nBands
        Cx_w = Cx(:,:,band) + eye(size(Cx(:,:,band)))*trace(Cx(:,:,band))/nMics*10+0.0001*eye(size(Cx(:,:,band))); 
         
        % Anechoic RTFs / array steering vectors 
        h_dir = synthesis_pars.hrtfs(:,est_idx(band),band); 
        As = A_grid(:,est_idx(band),band); 
        As_l = As./As(ref_inds(1),:);
        As_r = As./As(ref_inds(2),:);
        g_l = h_dir(1)./As(ref_inds(1),:);
        g_r = h_dir(2)./As(ref_inds(2),:);
        if(abs(g_l)>4 || abs(g_r)>4), g_l = 1.0; g_r = 1.0; end % bypass if > 12dB
        
        if synthesis_pars.ENABLE_COVARIANCE_MATCHING 
            % Target spatial covariance matrix  
            hhH = (h_dir*h_dir');    
            % Target covariance matrices for direct and diffuse streams
            Cy_dir = (1-diffuseness(band))*energy_per_band(band)*hhH./(trace(H_bin_dcm(:,:,band))+eps);
            Cy_diff = diffuseness(band)*energy_per_band(band)*eye(2)*H_bin_dcm(:,:,band)./(trace(H_bin_dcm(:,:,band))+eps); 
            % Combine target covariance matrices into one
            Cy = Cy_dir + Cy_diff;  
        end
        
        % Diffuse baseline/prototype mixing matrix
        Q_diff = zeros(2,nMics);
        Q_diff(1,ref_inds(1)) = diff_eq(band);
        Q_diff(2,ref_inds(2)) = diff_eq(band);
         
        % Source baseline/prototype mixing matrix
        switch synthesis_pars.SOURCE_BEAMFORMING_OPTION 
            case 'FaS'
                w_l = g_l*pinv(As_l); 
                w_r = g_r*pinv(As_r); 
                Q_dir = [w_l; w_r];   
            case 'BMVDR'
                w_l = g_l*(As_l'/Cx_w*As_l)\(As_l'/Cx_w); 
                w_r = g_r*(As_r'/Cx_w*As_r)\(As_r'/Cx_w); 
                Q_dir = [w_l; w_r];
            case 'RLCMV'
                assert(0); % untested
                w_l = ones(1,2)*(As_l'/Cx_w*As_l)\(As_l'/Cx_w); 
                w_r = ones(1,2)*(As_r'/Cx_w*As_r)\(As_r'/Cx_w); 
                Q_dir = [w_l; w_r];   
        end 
        
        % Baseline/prototype mixing matrix
        if strcmp(synthesis_pars.SOURCE_BEAMFORMING_OPTION, 'none') 
            % Single-stream (No beamforming, just passing through the reference signals)
            Q = zeros(2,nMics);
            Q(1,ref_inds(1)) = 1;
            Q(2,ref_inds(2)) = 1;
            if synthesis_pars.ENABLE_DRC
                Q = G_diff(band).*Q; 
            end 
        else
            % Dual-stream prototype:
            Q = (1-diffuseness(band)).*Q_dir + diffuseness(band).*Q_diff;
        end
        
        % Final mixing matrix 
        if synthesis_pars.ENABLE_COVARIANCE_MATCHING  
            [new_M, ~] = formulate_M_and_Cr(Cx(:,:,band), Cy, Q, 1, 0.1);  
        else
            new_M = Q;   
        end
        
        % Average over time
        M(:,:,band) = synthesis_pars.temporal_avg_coeff*M(:,:,band) + (1-synthesis_pars.temporal_avg_coeff)*new_M;             
    end   
    
    % Apply mixing matrix 
    outTF = zeros(2,timeslots,nBands); 
    for band=1:nBands
        outTF(:,:,band) = M(:,:,band) * inTF(:,:,band);  
        parameter_container.output_energy(:,(blockIndex-1)*timeslots+1:blockIndex*timeslots,band) = abs(outTF(:,:,band)).^2; 
    end 
    
    % inverse-STFT
    outputBlock = outTF; 
    % permute back to (nBands,nFrames,nSH)
    tempOut = permute(outputBlock, [3 2 1]);
    % time-domain range of current block
    TDrange = startIndex + (0:blocksize-1);
    % write to output signal
    tmp = afSTFT(tempOut); 
    outsig(TDrange,:)= tmp;
    
    % For next iteration
    startIndex = startIndex + blocksize;
    blockIndex = blockIndex+1; 
end
 
outsig = outsig(afSTFTdelay+(1:((nBlocks-1)*blocksize)),:);  

% STFT destroy
afSTFT();

end
