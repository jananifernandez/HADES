clear all, dbstop if error%, close all %#ok 
% An open-source DSP framework for binaural Hearing-Assistive Devices (HADES)
% HADES employs a parametric sound-field model, which assumes a single source
% and a diffuse component per time-frequency tile. The balance between them
% is dictated via diffuseness parameter estimates

% Make sure to first run "CREATE_SIMULATED_ARRAY_RECORDINGS.m" before
% running this script!

% Change this path to where the external libraries are
externals_path = '../../resources_/';

% This script requires the following external libraries
% Obtain from: https://github.com/polarch/Spherical-Harmonic-Transform
addpath([externals_path 'Spherical-Harmonic-Transform'])
% Obtain from: https://github.com/jvilkamo/afSTFT
addpath([externals_path 'afSTFT'])

addpath('./resources_/')
addpath('./utils/')
addpath('./recordings/') 

ENABLE_PLOTS = 1;

if ~exist('renders','dir'), mkdir('renders'); end

%% STEP 1: Prep work
% Global parameters:
%   h_array:       time-domain array measurements; filterLength x nChannels x nMeasurements 
%   grid_dirs_deg: directions for each measurement, in radians; nMeasurements x 2 
%   fs:            sample rate of the measurements and run-time audio  
%   mic_inds:      indices into h_array(:,mic_inds,:) corresponding to all left+right device sensors 
%   ref_inds:      indices into h_array(:,mic_inds(ref_inds),:) corresponding to the left+right reference sensors
%
load('h_array.mat')
grid_dirs_deg = grid_dirs_rad*180/pi;
fs = 48e3;  
configuration = '4+4'; % 

% Determine which indices correspond to which device (and not all have to be used):
switch configuration
    case '2+2'
        left_inds = [2 4];      % Left device sensors (indexing into "1:nChannels")
        right_inds = [6 8];     % Right device sensors (indexing into "1:nChannels")
    case '3+3'
        left_inds = [2 3 4];    % Left device sensors (indexing into "1:nChannels")
        right_inds = [6 7 8];   % Right device sensors (indexing into "1:nChannels")
    case '4+4'
        left_inds = [1,2,3,4];  % Left device sensors (indexing into "1:nChannels")
        right_inds = [5,6,7,8]; % Right device sensors (indexing into "1:nChannels")
end
mic_inds = [left_inds right_inds]; % Both devices

% Determine which "mic_inds" are to be used as the left+right reference sensors
switch configuration    
    case '2+2'
        ref_inds = [1 3]; % Reference sensors [left_ind right_ind], (indexing into "mic_inds")
    case '3+3'
        ref_inds = [1 4]; % Reference sensors [left_ind right_ind], (indexing into "mic_inds") 
    case '4+4'
        ref_inds = [2 6]; % Reference sensors [left_ind right_ind], (indexing into "mic_inds") 
end


%% STEP 2: load simulated recording, or load a real recording using the array
scene_name = 'h_array.mat_medium_8  2_4+4';
[insig, wav_fs] = audioread([scene_name '.wav']); 


%% STEP 3: Configure and Apply HADES Analysis 
% Parameters for the analysis: 
%   analysis_pars.fs:                        sampling rate
%   analysis_pars.hopsize:                   hopsize in time-domain samples
%   analysis_pars.blocksize:                 blocksize in time-domain samples
%   analysis_pars.grid_dirs_deg              directions for each measurement, in degrees; nMeasurements x 2 
%   analysis_pars.h_array:                   time-domain array measurements; filterLength x nChannels x nMeasurements 
%   analysis_pars.DOA_ESTIMATOR:             {'MUSIC'}
%   analysis_pars.DIFFUSENESS_ESTIMATOR:     {'SDDIFF'}
%   analysis_pars.FREQUENCY_AVERAGING_OPTION: {'none', 'octave', 'ERB', 'broad_band} (Only used if steering_direction_deg = 'DoA')
%     none:       the estimated parameters are not averaged over frequency
%     octave:     the estimated parameters are averaged over frequency bands grouped according to octave bands
%     ERB:        the estimated parameters are averaged over frequency bands grouped according to equivalent rectangular bandwidths (ERB)
%     broad-band: the estimated parameters are averaged over all frequency bands
%   analysis_pars.temporal_avg_coeff:       [0..1] averaging coefficient for the covariance matrices
%
analysis_pars.fs = fs;
analysis_pars.hopsize = 128;    
analysis_pars.blocksize = 512;  
analysis_pars.grid_dirs_deg = grid_dirs_deg; 
analysis_pars.h_array = h_array(:,[left_inds right_inds],:); 
analysis_pars.DOA_ESTIMATOR = 'MUSIC'; 
analysis_pars.DIFFUSENESS_ESTIMATOR = 'SDDIFF';
analysis_pars.FREQUENCY_AVERAGING_OPTION = 'none';
analysis_pars.temporal_avg_coeff = 1 - 1/(4096/analysis_pars.blocksize);% 0.97; % 512: 0.77, 256: 0.92

%%% Initialise HADES Analysis
analysis_pars = hades_analysis_init(analysis_pars); 

%%% Apply HADES Analysis   
% This analysis module will process the input time-domain signals and return the signal
% and parameter containers. The former contains the resulting time-frequency domain
% signals and signal statistics, while the latter holds the estimated spatial parameters (i.e. DoAs and diffuseness).
[signal_container, parameter_container] = hades_analysis(insig, analysis_pars);

% Plots
if ENABLE_PLOTS
    total_time = size(signal_container.inTF,2)*size(signal_container.inTF,4); %#ok
    figure, subplot(4,1,1), h=pcolor(1:total_time, analysis_pars.centreFreq, ...
         reshape(permute(20*log10(abs(signal_container.inTF(1,:,:,:))), [3 2 4 1]), [size(signal_container.inTF,3) total_time]));
    colorbar, axis xy, caxis([-60 0]), title('Energy channel 1 (dB)')  
    h.EdgeColor = 'none'; set(gca,'YScale','log','YDir','normal','YTick',[500,1000,2000,5000,10e3,20e3]);
    subplot(4,1,2), h=pcolor(1:size(signal_container.inTF,4), analysis_pars.centreFreq, ...
        reshape(grid_dirs_deg(parameter_container.doa_idx(:),1), size(parameter_container.doa_idx)));
    colorbar, axis xy, caxis([-180 180]), title('azimuth (degrees)')
    h.EdgeColor = 'none'; set(gca,'YScale','log','YDir','normal','YTick',[500,1000,2000,5000,10e3,20e3]);
    subplot(4,1,3), h=pcolor(1:size(signal_container.inTF,4), analysis_pars.centreFreq, ...
        reshape(grid_dirs_deg(parameter_container.doa_idx(:),2), size(parameter_container.doa_idx)));
    colorbar, axis xy, caxis([-90 90]), title('elevation (degrees)')
    h.EdgeColor = 'none'; set(gca,'YScale','log','YDir','normal','YTick',[500,1000,2000,5000,10e3,20e3]);
    subplot(4,1,4), h=pcolor(1:size(signal_container.inTF,4), analysis_pars.centreFreq, parameter_container.diffuseness);
    colorbar, axis xy, caxis([0 1]), title('diffuseness')
    h.EdgeColor = 'none'; set(gca,'YScale','log','YDir','normal','YTick',[500,1000,2000,5000,10e3,20e3]);
end

%% STEP 4: Configure and Apply HADES Synthesis 
%   synthesis_pars.ENABLE_COVARIANCE_MATCHING:  Flag, 0 or 1
%   synthesis_pars.SOURCE_BEAMFORMING_OPTION: {'none', 'FaS', 'BMVDR', 'RLCMV'}
%     none:   the reference sensor signals are passed through, but with a equalisation curve that brings the 
%             diffuse-field response of the array to instead be inline with the diffuse-field response of the
%             HRIR set
%     FaS:    filter-and-sum, mixed with reference signals based on diffuseness
%     BMVDR:  minimum-variance distortionless response (MVDR) beamformer steered towards the DoA estimates 
%             per band. The diffuseness parameter balances between these direct source beamformer signals and
%             the diffuse reference signals (i.e. no beamforming). The MVDR gain constraint is instead based
%             the HRIRs, so that the monaural/pinna cues are also preserved
%     RLCMV:  using the robust linearly-constrained minimum variance (LCMV) approach detailed in [2]. Beam 
%             steering constraints are based on +/- 5 degrees around the estimated DoAs. Otherwise, this option 
%             is equivalent to the BMVDR option.
%   synthesis_pars.hrirs:         HRIR measurements; lHRIR x 2 x nHRIRs (set to [] if no HRIR convolution is wanted)
%   synthesis_pars.hrir_dirs_deg: HRIR directions; nHRIRs x 2 (not needed if hrirs = [])
%   synthesis_pars.hrir_fs:       HRIR sample rate (not needed if hrirs = [])
%
% [2] As' ad, H., Bouchard, M. and Kamkar-Parsi, H., 2019. A robust target
%     linearly constrained minimum variance beamformer with spatial cues 
%     preservation for binaural hearing aids. IEEE/ACM Transactions on 
%     Audio, Speech, and Language Processing, 27(10), pp.1549-1563.
%
load('hrirs.mat')
synthesis_pars.ENABLE_COVARIANCE_MATCHING = 1;
synthesis_pars.SOURCE_BEAMFORMING_OPTION = 'none'; 
synthesis_pars.hrirs = hrirs(:,:,findClosestGridPoints(hrir_dirs_rad, grid_dirs_rad)); 
synthesis_pars.hrir_fs = h_array_fs;
synthesis_pars.ref_inds = ref_inds;
synthesis_pars.temporal_avg_coeff = analysis_pars.temporal_avg_coeff;  

% Initialise
synthesis_pars = hades_synthesis_init(analysis_pars, synthesis_pars);

% Apply HADES Synthesis   
%  signal_container:    obtained using 'hades_analysis()' 
%  parameter_container: obtained using 'hades_analysis()' 
outsig = hades_synthesis(synthesis_pars, signal_container, parameter_container);
outsig = 0.99.*outsig./max(abs(outsig(:)));
audiowrite(['renders/' scene_name '_' synthesis_pars.SOURCE_BEAMFORMING_OPTION '_' ...
    num2str(synthesis_pars.ENABLE_COVARIANCE_MATCHING ) '.wav'], outsig, fs, 'BitsPerSample', 32);
