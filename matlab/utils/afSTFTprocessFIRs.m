function hFB = afSTFTprocessFIRs(hIR, hopsize, LOW_DELAY_MODE, HYBRID_OR_UNIFORM)

if nargin<2, HYBRID_OR_UNIFORM=1; end

% dimensions: samples x sensors/microphones x measurements/directions
lIR = size(hIR,1);
nMics = size(hIR,2);
nDirs = size(hIR,3);

% pick a direction to estimate the center of microphone delays
testSet = hIR(:,:,1);
% find median delay for all sensors
for k=1:nMics
    [~,idxMax(k)] = max(testSet(:,k)); 
    grpDel(k) = median(grpdelay(testSet(:,k)));
end
%idxDel = round(median(grpDel)+1);
idxDel = round(mean(idxMax)+1);
% ideal impulse at median delay
centerImpulse = zeros(lIR,1);
centerImpulse(idxDel) = 1;
% analyze impulse with the filterbank
centerImpulseFB = afAnalyze2([centerImpulse;zeros(1024,1)], hopsize, LOW_DELAY_MODE, HYBRID_OR_UNIFORM);  
nBands = size(centerImpulseFB,1);
centerImpulseFB_energy = sum(abs(centerImpulseFB).^2,2);

% initialize FB coefficients
hFB = zeros(nBands, nMics, nDirs);
for nd = 1:nDirs
%    fprintf('%d/%d\n',nd,nDirs);
    ir = hIR(:,:,nd);
    irFB = afAnalyze2([ir; zeros(1024,nMics)], hopsize, LOW_DELAY_MODE, HYBRID_OR_UNIFORM);
    for nm = 1:nMics
        irFB_energy = sum(abs(irFB(:,:,nm)).^2, 2);
        irFB_gain = sqrt(irFB_energy./centerImpulseFB_energy);
        
        cross = sum(irFB(:,:,nm).*conj(centerImpulseFB), 2);
        phase = angle(cross);
        
        hFB(:,nm,nd) = irFB_gain.*exp(1i*phase);
    end
end
end


function out=afAnalyze2(in,hopsize,lowDelayMode,hybMode)
% A modified version of 'afAnalyze', originally written by Juha Vilkamo 
if hybMode > 0
    if lowDelayMode>0, afSTFT(hopsize,size(in,2),1,'hybrid', 'low_delay'); else, afSTFT(hopsize,size(in,2),1,'hybrid'); end
else
    if lowDelayMode>0, afSTFT(hopsize,size(in,2),1, 'low_delay'); else, afSTFT(hopsize,size(in,2),1); end
end
out=afSTFT(in); % process
afSTFT(); % free
end



