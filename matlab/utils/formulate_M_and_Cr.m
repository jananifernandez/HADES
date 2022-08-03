function [M, Cr] = formulate_M_and_Cr(Cx, Cy, Q, flag, reg)
% FORMULATE_M_AND_CR Computes optimal mixing matrix based on input-output
%                    covariance matrix
%
% J.Vilkamo's "Covariance Domain Framework for Spatial Audio Processing".
%
    if nargin == 4
        reg=0.2;
    end
    % flag = 0: Expect usage of residuals
    % flag = 1: Fix energies instead
    lambda=eye(length(Cy),length(Cx));

    % Decomposition of Cy
    [U_Cy,S_Cy]=svd(Cy);
    Ky=U_Cy*sqrt(S_Cy);

    % Decomposition of Cx
    [U_Cx,S_Cx]=svd(Cx);
    Kx=U_Cx*sqrt(S_Cx);

    %SVD of Kx
    Ux=U_Cx;
    Sx=sqrt(S_Cx);
    % Vx = identity matrix

    % Regularization Sx
    Sx_diag=diag(Sx);
    limit=max(Sx_diag)*reg+1e-20;
    Sx_reg_diag=max(Sx_diag,limit);

    % Formulate regularized Kx^-1
    Kx_reg_inverse=diag(1./Sx_reg_diag)*Ux';

    % Formulate normalization matrix G_hat
    Cy_hat_diag=diag(Q*Cx*Q');
    limit=max(Cy_hat_diag)*0.001+1e-20;
    Cy_hat_diag=real(max(Cy_hat_diag,limit));
    G_hat=diag(real(sqrt(diag(Cy)./Cy_hat_diag)));

    % Formulate optimal P
    [U,~,V]=svd(Kx'*Q'*G_hat'*Ky);
    P=V*lambda*U';

    % Formulate M
    M=Ky*P*Kx_reg_inverse;

    % Formulate residual covariance matrix
    Cy_tilde = M*Cx*M';
    Cr=Cy-Cy_tilde;

    % Use energy compensation instead of residuals
    if flag==1
        adjustment=diag(Cy)./diag(Cy_tilde + 1e-20);
        G=diag(sqrt(adjustment));
        M=G*M;
        Cr='unnecessary';
    end

end