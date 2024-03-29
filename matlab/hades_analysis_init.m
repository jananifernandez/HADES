function analysis_pars = hades_analysis_init(analysis_pars)
%
% This file is part of HADES
% Copyright (c) 2021 - Janani Fernandez & Leo McCormack
%
% HADES is free software; you can redistribute it and/or modify it under the
% terms of the GNU General Public License as published by the Free Software
% Foundation; either version 2 of the License, or (at your option) any later
% version.
%
% HADES is distributed in the hope that it will be useful, but WITHOUT ANY
% WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
% A PARTICULAR PURPOSE. See the GNU General Public License for more details.
%
% See <http://www.gnu.org/licenses/> for a copy of the GNU General Public
% License.
%

% An open-source DSP framework for Hearing-Assistive Devices (HADES) 

%%% INITIALISATIONS
% afSTFT configuration
analysis_pars.timeslots = ceil(analysis_pars.blocksize/analysis_pars.hopsize); % number of down-sampled time slots per block
analysis_pars.LDmode = 0;
analysis_pars.hybridMode = 1;
f = afSTFT(analysis_pars.hopsize,1,1);  % Just for the purpose of obtaining the frequency vector
afSTFT(); % free
analysis_pars.centreFreq = f*analysis_pars.fs/2;
load('afSTFT_centerFreqs133.mat')
analysis_pars.centreFreq = afCenterFreq48000;
nBands = length(analysis_pars.centreFreq);

% Convert the measurements into filterbank spectral coefficients
analysis_pars.h_array = analysis_pars.h_array./max(abs(analysis_pars.h_array(:)));
H_grid = afSTFTprocessFIRs(analysis_pars.h_array, analysis_pars.hopsize, analysis_pars.LDmode, analysis_pars.hybridMode);  
nMics = size(H_grid,2);
analysis_pars.H_grid = H_grid./nMics;

% Compute diffuse-field covariance matrix per frequency band
nGrid = size(H_grid,3);
nMics = size(H_grid,2);
analysis_pars.DFCmtx = zeros(nMics, nMics, nBands);
for nb=1:size(H_grid,1) 
    tmp = squeeze(H_grid(nb,:,:));
    analysis_pars.DFCmtx(:,:,nb) = (1/nGrid).*tmp*tmp';
end 

% Initialisations for DoA estimation and source detection 
...

% Frequency grouping 
switch analysis_pars.FREQUENCY_AVERAGING_OPTION
    case 'none'
    case 'octave'
    case 'ERB'
    case 'broad-band'
end


end
