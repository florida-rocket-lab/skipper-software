% ________________________________________________________________________
%
%                              state_space.m
% ________________________________________________________________________
% 
% BY:       FLORIDA ROCKET LAB - GNC SUBTEAM
% EDITORS:  N. HIRSCH
% DATE:     1/20/25
%
% OVERVIEW: Calculates A, B, C, D, Q, and R matices for use in a state
% space Simulink model and LQR controller.
    
    clear; clc;
    
    % Initialize state space matricies. Since they are sparse simply use zeros
    % and then input non-zero terms individually.
    % X = [x y z u v w phi theta psi p q r]
    % U = [T tauR xi zeta]
    
    M = 1.71;
    g = 1;
    rho = 5.58/12;
    Ixx = 8.78;
    Iyy = 30.37;
    Izz = 30.93;
    
    % Equillibrium point.
    x0 = 0;
    y0 = 0;
    z0 = 0;
    u0 = 0;
    v0 = 0;
    w0 = 0;
    phi0 = 0;
    theta0 = 0;
    psi0 = 0;
    p0 = 0;
    q0 = 0;
    r0 = 0;
    T0 = M*g;
    tauR0 = 0;
    xi0 = 0;
    zeta0 = 0;


    % BEGIN "A"; Do not modify this. The python script looks for this line.
    A = [
        0 0 0 cos(psi0)*cos(theta0) sin(phi0)*sin(theta0)*cos(psi0)-sin(psi0)*cos(phi0) sin(phi0)*sin(psi0)+sin(theta0)*cos(phi0)*cos(psi0) v0*sin(phi0)*sin(psi0)+v0*sin(theta0)*cos(phi0)*cos(psi0)-w0*sin(phi0)*sin(theta0)*cos(psi0)+w0*sin(psi0)*cos(phi0) -u0*sin(theta0)*cos(psi0)+v0*sin(phi0)*cos(psi0)*cos(theta0)+w0*cos(phi0)*cos(psi0)*cos(theta0) -u0*sin(psi0)*cos(theta0)-v0*sin(phi0)*sin(psi0)*sin(theta0)-v0*cos(phi0)*cos(psi0)+w0*sin(phi0)*cos(psi0)-w0*sin(psi0)*sin(theta0)*cos(phi0) 0 0 0;
        0 0 0 sin(psi0)*cos(theta0) sin(phi0)*sin(psi0)*sin(theta0)+cos(phi0)*cos(psi0) -sin(phi0)*cos(psi0)+sin(psi0)*sin(theta0)*cos(phi0) -v0*sin(phi0)*cos(psi0)+v0*sin(psi0)*sin(theta0)*cos(phi0)-w0*sin(phi0)*sin(psi0)*sin(theta0)-w0*cos(phi0)*cos(psi0) -u0*sin(psi0)*sin(theta0)+v0*sin(phi0)*sin(psi0)*cos(theta0)+w0*sin(psi0)*cos(phi0)*cos(theta0) u0*cos(psi0)*cos(theta0)+v0*sin(phi0)*sin(theta0)*cos(psi0)-v0*sin(psi0)*cos(phi0)+w0*sin(phi0)*sin(psi0)+w0*sin(theta0)*cos(phi0)*cos(psi0) 0 0 0;
        0 0 0 -sin(theta0) sin(phi0)*cos(theta0) cos(phi0)*cos(theta0) v0*cos(phi0)*cos(theta0)-w0*sin(phi0)*cos(theta0) -u0*cos(theta0)-v0*sin(phi0)*sin(theta0)-w0*sin(theta0)*cos(phi0) 0 0 0 0;
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
        0 cos(xi0)*cos(zeta0)/Ixx -tauR0*sin(xi0)*cos(zeta0)/Ixx -tauR0*sin(zeta0)*cos(xi0)/Ixx;
        -rho*sin(xi0)*cos(zeta0)/Iyy sin(zeta0)/Iyy -T0*rho*cos(xi0)*cos(zeta0)/Iyy T0*rho*sin(xi0)*sin(zeta0)/Iyy+tauR0*cos(zeta0)/Iyy;
        -rho*sin(zeta0)/Izz -sin(xi0)*cos(zeta0)/Izz -tauR0*cos(xi0)*cos(zeta0)/Izz -T0*rho*cos(zeta0)/Izz+tauR0*sin(xi0)*sin(zeta0)/Izz;
    ];
    % END "B"; Do not modify this. The python script looks for this line.

    % BEGIN "d"; Do not modify this. The python script looks for this line.
    d = [
        -phi0*v0*sin(phi0)*sin(psi0) - phi0*v0*sin(theta0)*cos(phi0)*cos(psi0) + phi0*w0*sin(phi0)*sin(theta0)*cos(psi0) - phi0*w0*sin(psi0)*cos(phi0) + psi0*u0*sin(psi0)*cos(theta0) + psi0*v0*sin(phi0)*sin(psi0)*sin(theta0) + psi0*v0*cos(phi0)*cos(psi0) - psi0*w0*sin(phi0)*cos(psi0) + psi0*w0*sin(psi0)*sin(theta0)*cos(phi0) + theta0*u0*sin(theta0)*cos(psi0) - theta0*v0*sin(phi0)*cos(psi0)*cos(theta0) - theta0*w0*cos(phi0)*cos(psi0)*cos(theta0);
        phi0*v0*sin(phi0)*cos(psi0) - phi0*v0*sin(psi0)*sin(theta0)*cos(phi0) + phi0*w0*sin(phi0)*sin(psi0)*sin(theta0) + phi0*w0*cos(phi0)*cos(psi0) - psi0*u0*cos(psi0)*cos(theta0) - psi0*v0*sin(phi0)*sin(theta0)*cos(psi0) + psi0*v0*sin(psi0)*cos(phi0) - psi0*w0*sin(phi0)*sin(psi0) - psi0*w0*sin(theta0)*cos(phi0)*cos(psi0) + theta0*u0*sin(psi0)*sin(theta0) - theta0*v0*sin(phi0)*sin(psi0)*cos(theta0) - theta0*w0*sin(psi0)*cos(phi0)*cos(theta0);
        -phi0*v0*cos(phi0)*cos(theta0) + phi0*w0*sin(phi0)*cos(theta0) + theta0*u0*cos(theta0) + theta0*v0*sin(phi0)*sin(theta0) + theta0*w0*sin(theta0)*cos(phi0);
        -g*psi0*sin(psi0)*cos(theta0) - g*theta0*sin(theta0)*cos(psi0) - g*cos(psi0)*cos(theta0) + q0*w0 - r0*v0 + T0*xi0*sin(xi0)*cos(zeta0)/M + T0*zeta0*sin(zeta0)*cos(xi0)/M;
        g*phi0*sin(phi0)*sin(psi0) + g*phi0*sin(theta0)*cos(phi0)*cos(psi0) - g*psi0*sin(phi0)*sin(psi0)*sin(theta0) - g*psi0*cos(phi0)*cos(psi0) + g*theta0*sin(phi0)*cos(psi0)*cos(theta0) - g*sin(phi0)*sin(theta0)*cos(psi0) + g*sin(psi0)*cos(phi0) - p0*w0 + r0*u0 - T0*zeta0*cos(zeta0)/M;
        -g*phi0*sin(phi0)*sin(theta0)*cos(psi0) + g*phi0*sin(psi0)*cos(phi0) + g*psi0*sin(phi0)*cos(psi0) - g*psi0*sin(psi0)*sin(theta0)*cos(phi0) + g*theta0*cos(phi0)*cos(psi0)*cos(theta0) - g*sin(phi0)*sin(psi0) - g*sin(theta0)*cos(phi0)*cos(psi0) + p0*v0 - q0*u0 + T0*xi0*cos(xi0)*cos(zeta0)/M - T0*zeta0*sin(xi0)*sin(zeta0)/M;
        -phi0*q0*cos(phi0)*tan(theta0) + phi0*r0*sin(phi0)*tan(theta0) - q0*theta0*sin(phi0)*tan(theta0)^2 - q0*theta0*sin(phi0) - r0*theta0*cos(phi0)*tan(theta0)^2 - r0*theta0*cos(phi0);
        phi0*q0*sin(phi0) + phi0*r0*cos(phi0);
        -phi0*q0*cos(phi0)/cos(theta0) + phi0*r0*sin(phi0)/cos(theta0) - q0*theta0*sin(phi0)*sin(theta0)/cos(theta0)^2 - r0*theta0*sin(theta0)*cos(phi0)/cos(theta0)^2;
        -Iyy*q0*r0/Ixx + Izz*q0*r0/Ixx + tauR0*xi0*sin(xi0)*cos(zeta0)/Ixx + tauR0*zeta0*sin(zeta0)*cos(xi0)/Ixx;
        Ixx*p0*r0/Iyy - Izz*p0*r0/Iyy + T0*rho*xi0*cos(xi0)*cos(zeta0)/Iyy - T0*rho*zeta0*sin(xi0)*sin(zeta0)/Iyy - tauR0*zeta0*cos(zeta0)/Iyy;
        -Ixx*p0*q0/Izz + Iyy*p0*q0/Izz + T0*rho*zeta0*cos(zeta0)/Izz + tauR0*xi0*cos(xi0)*cos(zeta0)/Izz - tauR0*zeta0*sin(xi0)*sin(zeta0)/Izz;
    ];
    % END "d"; Do not modify this. The python script looks for this line.
   
    if rank(ctrb(A, B)) == 12
        fprintf('FULLY CONTROLLABLE!\n')
    else
        fprintf('NAH UR COOKED BUDDY >:| FIX YO SHIT\n')
    end

    Uff = -pinv(B)*d;

    C = eye(12);
    
    D = zeros(size(B));

    Q = diag([1 1 1 1 1 1 1 1 1 1 10 10]);
    R = diag([1 1 1 1]);

    K = lqr(A, B, Q, R);

