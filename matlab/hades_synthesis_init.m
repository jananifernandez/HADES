function synthesis_pars = hades_synthesis_init(analysis_pars, synthesis_pars)
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

% Assumes that the hrtfs have been measured (or at least interpolated) to
% have the same grid as the array grid

%%% Copy parameters relavant to the synthesis struct from the analysis struct 
synthesis_pars.hopsize = analysis_pars.hopsize;
synthesis_pars.blocksize = analysis_pars.blocksize;  
synthesis_pars.LDmode = analysis_pars.LDmode;  
synthesis_pars.hybridMode = analysis_pars.hybridMode;  
synthesis_pars.centreFreq = analysis_pars.centreFreq;
synthesis_pars.timeslots = analysis_pars.timeslots; 
synthesis_pars.grid_dirs_deg = analysis_pars.grid_dirs_deg;
synthesis_pars.H_grid = analysis_pars.H_grid;

% Convert HRIRs to filterbank coefficients
assert(size(synthesis_pars.grid_dirs_deg,1)==size(synthesis_pars.H_grid,3));
hrtfs = afSTFTprocessFIRs(synthesis_pars.hrirs, synthesis_pars.hopsize, analysis_pars.LDmode, analysis_pars.hybridMode);  

% Interpolate so that the HRIR grid is the same as the scanning grid 
synthesis_pars.hrtfs = permute(hrtfs, [2 3 1]);
 
end

