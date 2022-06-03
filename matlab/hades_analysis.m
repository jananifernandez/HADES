function [signal_container, parameter_container] = hades_analysis(insig, pars)

% Local variables
applyDiffWhitening = 1; 
hopsize = pars.hopsize;
blocksize = pars.blocksize;
inputsize = size(insig,1);
nMics = size(insig,2);
nBands = length(pars.centreFreq); 
timeslots = pars.timeslots;
nGrid = size(pars.grid_dirs_deg,1);
nBlocks = ceil(inputsize/blocksize);
grid_dirs_rad = pars.grid_dirs_deg*pi/180;
grid_dirs_xyz = unitSph2cart(grid_dirs_rad);
insig = [insig; zeros(nBlocks*blocksize-inputsize,nMics)]; % Pad signals to an integer number of blocks

% Initialise the time-frequency transform
if pars.hybridMode>0
    if pars.LDmode>0, afSTFT(hopsize,nMics,1,'hybrid','low_delay'); else, afSTFT(hopsize,nMics,1,'hybrid');  end
else
    if pars.LDmode>0, afSTFT(hopsize,nMics,1,'low_delay'); else, afSTFT(hopsize,nMics,1);  end
end
% safmex_afSTFT(nMics, 0, hopsize, blocksize, 1, 0, 48e3);
 
% Initialise sinal buffers for storing the input signals in the TF domain
signal_container.inTF = zeros(nMics, timeslots, nBands, nBlocks);
signal_container.Cx_avg = zeros(nMics, nMics, nBands, nBlocks);
signal_container.Cx = zeros(nMics, nMics, nBands, nBlocks);

% Initialise structure for storing estimated spatial parameters
parameter_container.diffuseness = zeros(nBands,nBlocks); 
parameter_container.doa_idx = ones(nBands,nBlocks);
parameter_container.doa_hist = zeros(nGrid,nBands);

% Main processing loop
Cx = zeros(nMics,nMics,nBands);
diffuseness = zeros(nBands,1);
doa_idx = zeros(nBands,1);
startIndex = 1;
blockIndex = 1;
progress = 1;
while startIndex+blocksize <= nBlocks*blocksize
    % Print current progress
    if (startIndex+blocksize)*100/inputsize > progress
        fprintf('*'); if mod(progress,10)==0, fprintf('\n'), end
        progress=progress+1;
    end
     
    % Apply time-frequency transform
    TDrange = startIndex + (0:blocksize-1);
    temp = afSTFT(insig(TDrange,:));      % returns (nBands,nFrames,nChan)
    % temp = permute(safmex_afSTFT(insig(TDrange,:).'), [1 3 2]);      % returns (nBands,nChan,nFrames)
    newInputFrame = permute(temp, [3 2 1]); % permute to (nChan,nFrames,nBands) 
    signal_container.inTF(:,:,:,blockIndex) = newInputFrame;
    
    % Update covariance matrix per band 
    for band = 1:nBands
        new_Cx = newInputFrame(:,:,band) * newInputFrame(:,:,band)';
        signal_container.Cx(:,:,band,blockIndex) = new_Cx;
        Cx(:,:,band) = pars.temporal_avg_coeff * Cx(:,:,band) + (1-pars.temporal_avg_coeff) * new_Cx ;
    end
    signal_container.Cx_avg(:,:,:,blockIndex) = Cx;
    
    % Loop over bands 
    if ~isempty(pars.oracle_data)
        doa_idx(:,1) = pars.oracle_data.src_idx;
        diffuseness(:,1) = pars.oracle_data.diffuseness;
    else
        for band=1:nBands 
            % Parameter estimation requires first computing the EVD of the input spatial covarince matrix (SCM; Cx)  
            if applyDiffWhitening==1
                [U,E] = sorted_eig(pars.DFCmtx(:,:,band));
                T = sqrt(pinv(E))*U';    
                [V,S] = sorted_eig(T*Cx(:,:,band)*T');    
                %[V,~] = sorted_eig(Cx(:,:,band));
            else
                [V,S] = sorted_eig(Cx(:,:,band)); 
            end

            % Estimate the number of sources (if required)
            switch pars.DIFFUSENESS_ESTIMATOR 
                case 'SDDIFF' 
                    % Apply diffuse whitening process and recompute SCM eigenvalues  
                    lambda = diag(real(S));
                    diffuseness(band) = shdiff(lambda); 
                otherwise, assert(0);
            end

            % Source DoA estimation (if required)
            switch pars.DOA_ESTIMATOR 
                case 'MUSIC'   
                    A_grid = reshape(pars.H_grid(band,:,:),[nMics nGrid]); 
                    if applyDiffWhitening==1
                        A_grid = T*A_grid;
                    end
                    Vn = V(:,2:end);  
                    [~,doa_idx(band)] = sdMUSIC(grid_dirs_rad, grid_dirs_xyz, A_grid, 1, Vn); 
                otherwise, assert(0);
            end
        end
    end
    
    % CONDUCT ANY FREQUENCY AVERAGING HERE!  
    
    % Store estimated parameters  
    parameter_container.diffuseness(:, blockIndex) = diffuseness; 
    parameter_container.doa_idx(:, blockIndex) = doa_idx;
    parameter_container.doa_hist(doa_idx, :) = parameter_container.doa_hist(doa_idx, :) + 1; 
   
    % Increment block index
    startIndex = startIndex + blocksize;
    blockIndex = blockIndex+1;
end

% STFT destroy
afSTFT();
% safmex_afSTFT();

end

%%%%%%%%%%%%%%%%%%%%%%%
function [V,D] = sorted_eig(COV)
% Written by Archontis Politis
[V,D] = eig(COV);
d = real(diag(D));
[d_sorted, idx] = sort(d,'descend');
D = diag(d_sorted);
V = V(:,idx);
end

%%%%%%%%%%%%%%%%%%%%%%%
function [est_dirs, est_idx] = sdMUSIC(grid_dirs, grid_xyz, A_grid, nSrc, Vn)
% Based on sphMUSIC(), written by Archontis Politis
% get MUSIC spectrum
nGrid = size(grid_dirs,1);
P_music = zeros(nGrid,1);
for ng = 1:nGrid
    stVec = A_grid(:,ng);
    P_music(ng) = 1 / (stVec' * (Vn * Vn') * stVec);
end

kappa  = 50; % Von-Mises concentration factor (contrib. by Dr. Sakari Tervo)
P_minus_peak = P_music;
est_dirs = zeros(nSrc, 2);
est_idx = zeros(nSrc, 1);
for k = 1:nSrc
    [~, peak_idx] = max(P_minus_peak);
    est_dirs(k,:) = grid_dirs(peak_idx,:);
    est_idx(k) = peak_idx;
    VM_mean = grid_xyz(peak_idx,:); % orientation of VM distribution
    VM_mask = kappa/(2*pi*exp(kappa)-exp(-kappa)) * exp(kappa*grid_xyz*VM_mean'); % VM distribution
    VM_mask = 1./(0.00001+VM_mask); % inverse VM distribution
    P_minus_peak = P_minus_peak.*VM_mask;
end
end

%%%%%%%%%%%%%%%%%%%%%%%%
function K = computeSORTE(lambda)
% Written by Archontis Politis
N = length(lambda);
Dlambda = lambda(1:end-1) - lambda(2:end);
sigma2 = zeros(N-1,1);
for k=1:N-1 
    meanDlambda = 1/(N-k)*sum(Dlambda(k:N-1));
    sigma2(k) = 1/(N-k)*sum( (Dlambda(k:N-1) - meanDlambda).^2 );
end
SORTE = zeros(N-2,1);
for k=1:N-2 
    if sigma2(k)>0, SORTE(k) = sigma2(k+1)/sigma2(k);
    elseif sigma2(k) == 0, SORTE(k) = Inf;
    end
end

[~,K] = min(SORTE(1:end-1));  
end
