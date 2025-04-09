% ________________________________________________________________________
%
%                              state_space.m
% ________________________________________________________________________
% 
% BY:       FLORIDA ROCKET LAB - GNC SUBTEAM
% EDITORS:  N. HIRSCH
% DATE:     1/20/25
%
% OVERVIEW: Calculates A, B, C, D, Q, and R matrices as well as feedforward
% control and constant input disturbances for use in 'skipper_lqr_2.slx' or
% 'skipper_lqi.slx' Simulink models. Run this before the models to add all
% the nessecary variables for the model to use into the workspace.
    
    clear; clc;
    
    
    % Define known parameters of system.
    M = 1.71;
    g = 1;
    rho = 5.58/12;
    Ixx = 8.78;
    Iyy = 30.37;
    Izz = 30.93;

    is_lqi = true; % Disable if running 'skipper_lqr_2.slx'.
    
    % Define equillibrium point. When trimming for hovering all parameters
    % should be zero except thrust, T, as defined by the state, X, and
    % control, U, vectors of the form:
    %   X: state (= [x y z u v w phi theta psi p q r])
    %   U: control (= [T tauR xi zeta])
    % A good equillibirum thrust is M*g as this counteracts gravity.
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
    T0 = M*g; % Thrust
    tauR0 = 0; % Reaction torque.
    xi0 = 0; % Upper gimbal angle.
    zeta0 = 0; % Lower gimbal angle.

    % Define state space matrices. These are auto-generated from the EOM by
    % the Python script 'converter.py'. Three variables are auto-defined:
    %   A: state matrix
    %   B: input matrix
    %   d: disturbance (ie. disturbing forces such as gravity which are
    %   indepdendent of X or U)
    % In addition two other variables are manually generated:
    %   C: output matrix (= I by default)
    %   D: passthrough matrix (= 0 by default)
    % NOTE: DO NOT MODIFY THE LINES IN THIS SECTION AS THE PYTHON SCRIPT
    % LOOKS FOR THEM.
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

    C = eye(12);
    
    D = zeros(size(B));
   
    % Quick check to make sure linearized system is controllable (ie. the
    % controllability matrix as defined by 'ctrb()' is full rank). If it
    % fails change the equillibrium point.
    if rank(ctrb(A, B)) == 12
        fprintf('FULLY CONTROLLABLE!\n')
    else
        fprintf('NAH UR COOKED BUDDY >:| FIX YO SHIT\n')
    end
    
    % Define feedfoward control as equivallent to the disturbance.
    % Equivallent to shifting the equillbirium point for the control. Note
    % that in doing so we aren't shifting state equillibrium and hence end
    % up with an overdefined system as U has less components than d.
    % Nessicitates the use of the pseudoinverse, but for trimming for
    % hovering it is exact.
    Uff = -pinv(B)*d;
    
    % Define maximum values for each state and control to use with Bryson's
    % rule to tune LQR gain matrix, K.
    x_max = 10;
    y_max = 5;
    z_max = 5;
    u_max = 10;
    v_max = 10;
    w_max = 10;
    phi_max = 1;
    theta_max = pi/3;
    psi_max = pi/3;
    p_max = 1/10;
    q_max = 1/10;
    r_max = 1/10;
    T_max = M*g*4;
    tauR_max = 1;
    xi_max = pi/22;
    zeta_max = pi/22;
    
    % Form LQR weighting matrices using Bryson's rule:
    %   Q: LQR weight matrix on state
    %   R: LQR weight matrix on control
    % and then use 'lqr()' to calculate the LQR gain.
    Q = diag([ ...
        1/x_max^2 1/y_max^2 1/z_max^2 ...
        1/u_max^2 1/v_max^2 1/w_max^2 ...
        1/phi_max^2 1/theta_max^2 1/psi_max^2 ...
        1/p_max^2 1/q_max^2 1/r_max^2 ...
        ]);
    R = diag([1/T_max^2 1/tauR_max^2 1/xi_max^2 1/zeta_max^2]);

    K = lqr(A, B, Q, R);

    % Augment state space for LQI if using it.
    if is_lqi == true

        % First define extraction matrix, E, which seperates states subject
        % to integral error from rest of system. Then use that to augment
        % state space to include error terms.
        E = [diag([1 1 1]) zeros([3 9])];

        Ai = [A zeros([12 3]); -E zeros([3 3])]; % Augmented A matrix.
        Bi = [B; zeros([3, 4])]; % Augmented B matrix.

        % Define maximum values for error and then weight these according
        % to Bryson's rule. Apply this to Q matrix to augment it.
        % NOTE: Error never decreases, it only over grows, so the maximum
        % values allowed for it naturally need to be large.
        x_error_max = 500;
        y_error_max = 500;
        z_error_max = 500;

        Qi = diag([ ...
            diag(Q)' ...
            1/x_error_max^2 ...
            1/y_error_max^2 ...
            1/z_error_max^2 ...
            ]); % Augmented Q matrix.

        % Use 'lqr()' to compute gain for augmented system, Ki. Split into
        % two seperate feedback gain matrices, K1 for the state and K2 for
        % the error terms.
        Ki = lqr(Ai, Bi, Qi, R);

        K1 = Ki(:, 1:12);
        K2 = Ki(:, 13:end);
    end