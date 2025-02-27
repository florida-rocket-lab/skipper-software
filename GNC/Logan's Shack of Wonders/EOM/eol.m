
% +----------------------------------------------------------------------+
% |                Equations of Motion Derivation                        |
% +----------------------------------------------------------------------+
% | FOR USE BY FRL GNC AND AVIONICS SUBTEAM FOR SKIPPER VTOL MONOCOPTER  |
% | EDITORS:  LOGAN DAPP                                                 |
% | DATE:     26.2.2025                                                  |
% +----------------------------------------------------------------------+

% +----------------------------------------------------------------------+
% | NOTE: THIS SCRIPT IS USED BY "eol_o1.m" and "eol_o2.m"!              |
% +----------------------------------------------------------------------+

clear; clc;

% I := inertial frame
% U := body frame (Skipper)
% We use 3-2-1 Euler angle sequence from I -> U; 
syms psi theta phi psiD thetaD phiD real;  % We'll need the derivatives later.
T1 = eulermat(3, psi);
T2 = eulermat(2, theta);
T3 = eulermat(1, phi);
Ti2u = T3 * T2 * T1;
Tu2i = Ti2u.';

% T := thrust frame
% We use 2-3 Euler angle sequence from T -> U;
syms xi zeta real;
Tt2u = eulermat(3, zeta) * eulermat(2, xi);
Tu2t = Tt2u.';

% Our goal is to have x. = Ax + Bu where 
% x = [x y z u v w phi theta psi p q r]^T

% {x, y, z}    := position in I frame;
% {xD, yD, zD} := velocity in I frame;
% {u, v, w}    := velocity in U frame;
% {p, q, r}    := angular velocity in U frame;

syms x y z xD yD zD u v w uD vD wD p q r pD qD rD real;
% d/dt (<u v w>) = <uD vD wD> + <p q r> x <u v w>
vel_I = [xD; yD; zD];
vel_U = [u ; v ; w ];
agv_U = [p ; q ; r ];

% convert from <phiD thetaD psiD> to <p q r>
ptp2pqr = T3*(T2*(T1*[0; 0; psiD] + [0; thetaD; 0]) + [phiD; 0; 0]);

acc_U = [uD; vD; wD] + cross(agv_U, vel_U);
% +x is towards the sky in I and towards the top of Skipper in U
% M := mass of Skipper
% T := thrust
syms M T real;
g = 9.81;  % We assume g is constant.
for_T_U = Tt2u * [T; 0; 0];
for_U = Ti2u * [-M*g; 0; 0] + for_T_U;

% I is inertial tensor along principal axes, which are in U
syms Ixx Iyy Izz real;
I = [Ixx 0 0; 0 Iyy 0; 0 0 Izz];
H = I*agv_U; % H := angular momentum
HD = [pD; qD; rD] + cross(agv_U, H);

% tauRCS := pure torque applied by contra-rotating props.
% rho := distance that thrust is applied from COM.
syms rho tauRCS real;
rad_T_U = [-rho; 0; 0];
tor_U = cross(rad_T_U, for_T_U) + [tauRCS; 0; 0];

% Our goal is to have x. = Ax + Bu where 
%    x  = [x  y  z  u  v  w  phi  theta  psi  p  q  r]^T
% so xD = [xD yD zD uD vD wD phiD thetaD psiD pD qD rD]^T
to_solve = [xD yD zD uD vD wD phiD thetaD psiD pD qD rD];

eqns = vpasolve([
    vel_I == Tu2i * vel_U
    acc_U == for_U / M
    agv_U == ptp2pqr
    HD == tor_U
], to_solve);
eqns = simplify(expand([
    eqns.xD eqns.yD eqns.zD ...
    eqns.uD eqns.vD eqns.wD ...
    eqns.phiD eqns.thetaD eqns.psiD ...
    eqns.pD eqns.qD eqns.rD 
    ]));

% fprintf("f_out = [\n"); fprintf("\t%s;\n", erase(string(simplify(eqns)), " ") ); fprintf("];\n");

syms x0 y0 z0 ...
    u0 v0 w0 ...
    phi0 theta0 psi0 ...
    p0 q0 r0 ...
    T0 xi0 zeta0 tauRCS0 real;
expansion = [x y z u v w phi theta psi p q r T xi zeta tauRCS];
constants = [x0 y0 z0 u0 v0 w0 phi0 theta0 psi0 p0 q0 r0 T0 xi0 zeta0 tauRCS0];

for i_=1:1:12
    grad = gradient(eqns(i_), expansion);
    fd = fopen(strcat("utility\\nonlinear\\df", int2str(i_), ".m"), "W");
    fprintf(fd, strcat("function grad = df", int2str(i_), "(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)\n\ngrad = [\n"));
    fprintf(fd, "\t%s;\n", grad);
    fprintf(fd, "];\n\nend");
    fclose(fd);
end

lin_eqns = expand(taylor(eqns, expansion, constants, 'Order', 2));

