function [A, B, C, D, K, d] = lqrMatrices(x0, y0, z0, theta0, u0, v0, w0, psi0, phi0, p0, q0, r0, T0, xi0, zeta0, M, g, rho2, R, H)
% ________________________________________________________________________
%
%                              LQR MATRICES
% ________________________________________________________________________
% 
% BY:       FLORIDA ROCKET LAB - GNC SUBTEAM
% EDITORS:  N. HIRSCH
% DATE:     1/20/25
%
% OVERVIEW: Calculates A, B, C, D, Q, and R matices for use in a state
% space Simulink model and LQR controller.

    
    % Calculate rotational inertias in body frame.
    Ixx = M*R^2/2;
    Iyy = M*(3*R^2+H^2)/12;
    Izz = Iyy;
    
    % Initialize state space matricies. Since they are sparse simply use zeros
    % and then input non-zero terms individually.
    % x = [x y z u v w phi theta psi p q r]

    % BEGIN "A"; Do not modify this. The python script looks for this line.
    A = [
        0 r0 -q0 1 0 0 0 0 0 0 -z0 y0;
        -r0 0 p0 0 1 0 0 0 0 z0 0 -x0;
        q0 -p0 0 0 0 1 0 0 0 -y0 x0 0;
        0 0 0 0 r0 -q0 0 g*sin(theta0)*cos(psi0) g*sin(psi0)*cos(theta0) 0 -w0 v0;
        0 0 0 -r0 0 p0 -g*sin(phi0)*sin(psi0)-g*sin(theta0)*cos(phi0)*cos(psi0) -g*sin(phi0)*cos(psi0)*cos(theta0) g*sin(phi0)*sin(psi0)*sin(theta0)+g*cos(phi0)*cos(psi0) w0 0 -u0;
        0 0 0 q0 -p0 0 g*sin(phi0)*sin(theta0)*cos(psi0)-g*sin(psi0)*cos(phi0) -g*cos(phi0)*cos(psi0)*cos(theta0) -g*sin(phi0)*cos(psi0)+g*sin(psi0)*sin(theta0)*cos(phi0) -v0 u0 0;
        0 0 0 0 0 0 q0*cos(phi0)*tan(theta0)-r0*sin(phi0)*tan(theta0) q0*sin(phi0)*tan(theta0)^2+q0*sin(phi0)+r0*cos(phi0)*tan(theta0)^2+r0*cos(phi0) 0 1 sin(phi0)*tan(theta0) cos(phi0)*tan(theta0);
        0 0 0 0 0 0 -q0*sin(phi0)-r0*cos(phi0) 0 0 0 cos(phi0) -sin(phi0);
        0 0 0 0 0 0 q0*cos(phi0)/cos(theta0)-r0*sin(phi0)/cos(theta0) q0*sin(phi0)*sin(theta0)/cos(theta0)^2+r0*sin(theta0)*cos(phi0)/cos(theta0)^2 0 0 sin(phi0)/cos(theta0) cos(phi0)/cos(theta0);
        0 0 0 0 0 0 0 0 0 0 Iyy*r0/Ixx-Izz*r0/Ixx Iyy*q0/Ixx-Izz*q0/Ixx;
        0 0 0 0 0 0 0 0 0 -Ixx*r0/Iyy+Izz*r0/Iyy 0 -Ixx*p0/Iyy+Izz*p0/Iyy;
        0 0 0 0 0 0 0 0 0 Ixx*q0/Izz-Iyy*q0/Izz Ixx*p0/Izz-Iyy*p0/Izz 0;
    ];
    % END "A"; Do not modify this. The python script looks for this line.

    % u = [T tauR xi zeta]

    % BEGIN "B"; Do not modify this. The python script looks for this line.
    B = [
        0 0 0 0;
        0 0 0 0;
        0 0 0 0;
        cos(xi0)*cos(zeta0)/M 0 -T0*sin(xi0)*cos(zeta0)/M -T0*sin(zeta0)*cos(xi0)/M;
        sin(zeta0)/M 0 0 T0*cos(zeta0)/M;
        -sin(xi0)*cos(zeta0)/M 0 -T0*cos(xi0)*cos(zeta0)/M T0*sin(xi0)*sin(zeta0)/M;
        0 0 0 0;
        0 0 0 0;
        0 0 0 0;
        0 1/Ixx 0 0;
        -rho2*sin(xi0)*cos(zeta0)/Iyy 0 -T0*rho2*cos(xi0)*cos(zeta0)/Iyy T0*rho2*sin(xi0)*sin(zeta0)/Iyy;
        -rho2*sin(zeta0)/Izz 0 0 -T0*rho2*cos(zeta0)/Izz;
    ];
    % END "B"; Do not modify this. The python script looks for this line.

    % BEGIN "d"; Do not modify this. The python script looks for this line.
    d = [
        q0*z0 - r0*y0;
        -p0*z0 + r0*x0;
        p0*y0 - q0*x0;
        -g*psi0*sin(psi0)*cos(theta0) - g*theta0*sin(theta0)*cos(psi0) - g*cos(psi0)*cos(theta0) + q0*w0 - r0*v0 + T0*xi0*sin(xi0)*cos(zeta0)/M + T0*zeta0*sin(zeta0)*cos(xi0)/M;
        g*phi0*sin(phi0)*sin(psi0) + g*phi0*sin(theta0)*cos(phi0)*cos(psi0) - g*psi0*sin(phi0)*sin(psi0)*sin(theta0) - g*psi0*cos(phi0)*cos(psi0) + g*theta0*sin(phi0)*cos(psi0)*cos(theta0) - g*sin(phi0)*sin(theta0)*cos(psi0) + g*sin(psi0)*cos(phi0) - p0*w0 + r0*u0 - T0*zeta0*cos(zeta0)/M;
        -g*phi0*sin(phi0)*sin(theta0)*cos(psi0) + g*phi0*sin(psi0)*cos(phi0) + g*psi0*sin(phi0)*cos(psi0) - g*psi0*sin(psi0)*sin(theta0)*cos(phi0) + g*theta0*cos(phi0)*cos(psi0)*cos(theta0) - g*sin(phi0)*sin(psi0) - g*sin(theta0)*cos(phi0)*cos(psi0) + p0*v0 - q0*u0 + T0*xi0*cos(xi0)*cos(zeta0)/M - T0*zeta0*sin(xi0)*sin(zeta0)/M;
        -phi0*q0*cos(phi0)*tan(theta0) + phi0*r0*sin(phi0)*tan(theta0) - q0*theta0*sin(phi0)*tan(theta0)^2 - q0*theta0*sin(phi0) - r0*theta0*cos(phi0)*tan(theta0)^2 - r0*theta0*cos(phi0);
        phi0*q0*sin(phi0) + phi0*r0*cos(phi0);
        -phi0*q0*cos(phi0)/cos(theta0) + phi0*r0*sin(phi0)/cos(theta0) - q0*theta0*sin(phi0)*sin(theta0)/cos(theta0)^2 - r0*theta0*sin(theta0)*cos(phi0)/cos(theta0)^2;
        -Iyy*q0*r0/Ixx + Izz*q0*r0/Ixx;
        Ixx*p0*r0/Iyy - Izz*p0*r0/Iyy + T0*rho2*xi0*cos(xi0)*cos(zeta0)/Iyy - T0*rho2*zeta0*sin(xi0)*sin(zeta0)/Iyy;
        -Ixx*p0*q0/Izz + Iyy*p0*q0/Izz + T0*rho2*zeta0*cos(zeta0)/Izz;
    ];
    % END "d"; Do not modify this. The python script looks for this line.
   
    C = diag(ones(1, 12));
    
    D = zeros(size(B));
    
    % Initialize LQR weight matricies.
    Q = diag(ones(1, 12));
    R = diag(ones(1, 4));
    
    % Instantiate state space object. 
    sys = ss(A, B, C, D);
    
    % Solve for gain matrix, K, using 'lqr'.
    K = lqr(sys, Q, R);
end
