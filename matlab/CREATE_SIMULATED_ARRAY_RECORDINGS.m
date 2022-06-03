clear all,  dbstop if error %#ok close all,

% Src Indices
 
fs = 48000;  

% Input scene parameters
APPLY_AIR_ABS = 1;
hpf_cutoff = 120; % 
configuration = '4+4'; % '2+2', '3+3', '4+4'
input_path = 'resources_/';
file_name = 'h_array.mat';
roomType = 'medium'; %'medium', 'anechoic', 'medium2', 'high'
srcIndices = [8 2]; % 2: [90 0] 8: [0,0] 13:[-90,0] 4: [60,0] 11:[-60,0]     4 8 11

[srcSigs, fs_in] = audioread([input_path '4SPEAKERS_maleEng_femEng_maleDan_femDan.wav']);

assert(fs_in==fs)

%% HPF the src signals  
w_hh = hpf_cutoff/(fs/2);
h_filt = fir1(1500, w_hh, 'high').';
%freqz(h_filt,1)
srcSigs = fftfilt(repmat(h_filt, [1 size(srcSigs,2)]), [srcSigs; zeros(1500,size(srcSigs,2))]);

%% LOAD H
nSrc = length(srcIndices);

% for creating the array "recording"
load([input_path file_name])
nMics = size(h_array,2);
Lfilt = size(h_array,1);
nGrid = size(h_array,3); 
grid_dirs_deg = grid_dirs_rad*180/pi; 
switch configuration
    case '2+2'
        h_array = h_array(:,[2 4 6 8],:);
    case '3+3'
        h_array = h_array(:,[2 3 4 6 7 8],:);
    case '4+4'
        % do nothing
end

% for creating the binaural reference
%sofa = loadSofaFile('/Users/mccorml1/Documents/HRIRs_SOFA/KemarAuralID.sofa');
sofa = loadSofaFile('/Users/fernanj2/Documents/HRIRs_SOFA/D2_48K_24bit_256tap_FIR_SOFA_KEMAR.sofa');
h_ref = sofa.IR;
ref_dirs_deg = sofa.SourcePosition(1:2,:).';
ref_dirs_rad = ref_dirs_deg * pi/180;
ind = findClosestGridPoints(unitSph2cart(ref_dirs_rad), unitSph2cart(grid_dirs_rad));
h_ref = h_ref(:,:,ind);
%h_ref = convertHRTF2MinPhaseAllpass(h_ref); disp('test whether this is helping or not...')
ref_dirs_deg = ref_dirs_deg(ind,:);
ref_dirs_rad = ref_dirs_rad(ind,:);
 
%% MAIN LOOP 
rec_path = 'recordings';
if ~exist(rec_path, 'dir'), mkdir(rec_path); end

rir_name = [file_name '_' roomType '_' num2str(srcIndices) ];
rec_name = [rir_name '_' srcAudioOption '_' configuration]; 
ref_name = ['reference_' roomType '_' num2str(srcIndices) '_' srcAudioOption]; 

% Create recording 
switch roomType
    case 'anechoic'
        room = [5 5 5];
        rt60 = 0.1; 
    case 'medium'
       % src_sigs = randn(fs,nSrc);
        room = [10 7 4];
        rt60 = 0.5;    
    case 'medium2'
       % src_sigs = randn(fs,nSrc);
        room = [20 14 8];
        rt60 = 0.9; 
    case 'high'
       % src_sigs = randn(fs,nSrc);
        room = [10 7 4];
        rt60 = 2; 
end 
nBands = length(rt60);
if strcmp(roomType, 'anechoic')
    abs_wall = ones(1,6);
else 
    abs_wall_ratios = [0.75 0.86 0.56 0.95 0.88 1];
    abs_wall = findAbsCoeffsFromRT(room, rt60);%,abs_wall_ratios);
    [RT60_sabine,  d_critical] = room_stats(room, abs_wall);
end

% Place receiver in centre of room, and the source positions 1 metre from the receiver
rec = room/2; 
src = rec + unitSph2cart(grid_dirs_rad(srcIndices,:)); % [1 1 3].*

% Generate room IRs  
type = 'maxTime'; % 'maxTime' 'maxOrder' 
maxlim = max(rt60); % just cut if it's longer than that ( or set to max(rt60) )   
src2 = [src(:,1) room(2)-src(:,2) src(:,3)]; % change y coord for src/rec due to convention inside the IMS function
rec2 = [rec(:,1) room(2)-rec(:,2) rec(:,3)]; % change y coord for src/rec due to convention inside the IMS function
abs_echograms_ref = compute_echograms_arrays(room, src2, rec2, abs_wall, maxlim);
abs_echograms_array = compute_echograms_arrays(room, src2, rec2, abs_wall, maxlim);

% render RIRs
abs_bands = fs/2; % ignored anyway, can be anything
grid_cell{1} = ref_dirs_rad;
array_irs{1} = h_ref;
ref_rir = render_array_rirs(abs_echograms_ref, abs_bands, fs, grid_cell, array_irs);  
ref_rir = ref_rir{1}; % remove rec cell
grid_cell{1} = grid_dirs_rad;
array_irs{1} = h_array;
x_rir = render_array_rirs(abs_echograms_array, abs_bands, fs, grid_cell, array_irs); 
x_rir = x_rir{1}; % remove rec cell

% Applying air absorption
nSrc = size(src2,1); 
if APPLY_AIR_ABS
    disp('Applying air absorption - DIRECT BINAURAL')
    for ns=1:nSrc 
        for nc=1:size(ref_rir,2)
            temp_rir = applyAirAbsorption(ref_rir(:,nc,ns), fs);
            temp_rir = temp_rir.';
            ref_rir_tmp(:,nc,ns) = temp_rir(1:size(ref_rir,1));
        end 
    end
    ref_rir = ref_rir_tmp;
    clear ref_rir_air;
    
    x_rir_tmp = zeros(size(x_rir,1), size(x_rir,2), 1, size(x_rir,3));
    disp('Applying air absorption - Array') 
    for ns=1:nSrc 
        for nc = 1:size(x_rir,2)
            temp_rir = applyAirAbsorption(x_rir(:,nc,ns), fs);
            temp_rir = temp_rir.';
            x_rir_tmp(:,nc,ns) = temp_rir(1:size(x_rir,1));

           %bin_rirs_tmp(:,nc,nm,ns) =  bin_rirs{nm}(:,nc,ns);
        end 
    end 
    
    x_rir = x_rir_tmp;
    clear x_rir_tmp;
end

% Convolve RIRs
sigLength = size(srcSigs,1);
ref = zeros(sigLength,2);
x = zeros(sigLength,nMics);
for ns=1:nSrc
    ref = ref + fftfilt(ref_rir(:,:,ns), repmat(srcSigs(:,ns),[1 2]));      % Ideal direct to binaural signals
    x = x + fftfilt(x_rir(:,:,ns), repmat(srcSigs(:,ns),[1 nMics]));    % Simulated mic array recording 
end
ref = 0.99.*ref./max(abs(ref(:)));    
x = 0.99.*x./max(abs(x(:)));    

audiowrite([rec_path filesep rec_name '.wav'], x,  fs, 'BitsPerSample', 32); 
audiowrite([rec_path filesep ref_name '.wav'], ref, fs, 'BitsPerSample', 32);
audiowrite(['output' filesep ref_name '.wav'], ref, fs, 'BitsPerSample', 32);
 


  
