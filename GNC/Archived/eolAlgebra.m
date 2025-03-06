
% clear; clc;

% 3-2-1 Euler Angles; psi-theta-phi
% 2-3 Euler Angles; beta-alpha
% 2-3 Euler Angles; xi-zeta

syms psi theta phi real;
Ti2i1 = euler2rMatrix(psi, 3);
Ti2i2 = euler2rMatrix(theta, 2);
Ti2i3 = euler2rMatrix(phi, 1);
Ti2b = Ti2i3 * Ti2i2 * Ti2i1;

% qi2i1 = euler2quat(psi, 3);
% qi2i2 = euler2quat(theta, 2);
% qi2i3 = euler2quat(phi, 1);
% qi2b = quatMul(quatMul(qi2i3, qi2i2), qi2i1);
Tb2i = Ti2b.';

syms xi zeta real;
Tb2t = euler2rMatrix(zeta, 3) * euler2rMatrix(xi, 2);
Tt2b = Tb2t.';

syms alpha beta real;
Tb2w = euler2rMatrix(alpha, 3) * euler2rMatrix(beta, 2);
Tw2b = Tb2w.';

syms M g real;
% -x is down
F_gi = [-M*g; 0; 0];  % gravity
F_gb = Ti2b * F_gi;

syms T real;
F_tt = [T; 0; 0];  % thrust
F_tb = Tt2b * F_tt;

% r(t) = x*u_x + y*u_y + z*u_z;
% v(t) = x.*u_x + x*ωxu_x + y.*u_y + y*ωxu_y + z.*u_z + z*ωxu_z
syms p q r real;
omega = [
    0 -r  q;
    r  0 -p;
   -q  p  0;
];

syms x y z real;
syms u v w real;
vel = [u; v; w] + omega * [x; 0; 0] + omega * [0; y; 0] + omega * [0; 0; z];

%     [qz-ry+u]   [v(1)]
% v = [rx-pz+v] = [v(2)]
%     [py-qx+w]   [v(3)]

% a(t) = v(1).*u_x + v(1)*ωxu_x + v(2).*u_y + v(2)*ωxu_y + v(3).*u_z + v(3)*ωxu_z
syms uD vD wD real;
a_der = simplify([uD; vD; wD] + omega * [vel(1); 0; 0] + omega * [0; vel(2); 0] + omega * [0; 0; vel(3)]);
a_for = simplify(F_gb / M + F_tb / M);

syms Ixx Iyy Izz real;
I = [
    Ixx 0 0;
    0 Iyy 0;
    0 0 Izz;
];
syms pD qD rD real;
H = I*[p; q; r];
torque_der = simplify(I*[pD; qD; rD] + omega * H);

syms rho2 tauR real;
torque_sum = cross([-rho2; 0; 0], F_tb) + [tauR; 0; 0];

% quatTrans = @(q) [q(1) -q(2) -q(3) -q(4)];
% quatNorm = @(q) sum(quatMul(q, quatTrans(q)));
% quatRot = @(q, x) quatMul(quatMul(q, x), quatTrans(q));
syms psiD thetaD phiD real;
% qrot = quatRot(qi2i1, [0 0 0 psiD]);
% qrot = quatRot(qi2i2, qrot + [0 0 thetaD 0]);
% qrot = quatRot(qi2i3, qrot + [0 phiD 0 0]);

new_rot = Ti2i1 * [0; 0; psiD];
new_rot = Ti2i2 * (new_rot + [0; thetaD; 0]);
new_rot = Ti2i3 * (new_rot + [phiD; 0; 0]);


eul2pqr = [
    1 0 -sin(theta);
    0 cos(phi) cos(theta)*sin(phi);
    0 -sin(phi) cos(phi)*cos(theta);
];
pqr2eul = simplify(inv(eul2pqr));

ex1 = [phiD; thetaD; psiD];
in1 = pqr2eul * [p; q; r];

syms xD yD zD real;

ex2 = [xD; yD; zD];
in2 = Tb2i * [u; v; w] - [-r*y+q*z; -p*z+r*x; -q*x+p*y];

syms x0 y0 z0 xD0 yD0 zD0 u0 v0 w0 uD0 vD0 wD0 p0 q0 r0 pD0 qD0 rD0 ...
    psi0 theta0 phi0 psiD0 thetaD0 phiD0 T0 tauR0 xi0 zeta0 real


locations = [x0 y0 z0 xD0 yD0 zD0 u0 v0 w0 uD0 vD0 wD0 p0 q0 r0 pD0 qD0 rD0 ...
    psi0 theta0 phi0 psiD0 thetaD0 phiD0 T0 tauR0 xi0 zeta0];
vars = [x y z xD yD zD u v w uD vD wD p q r pD qD rD psi theta phi ...
    psiD thetaD phiD T tauR xi zeta];

eqns = [
    a_der(1);
    a_for(1);
    a_der(2);
    a_for(2);
    a_der(3);
    a_for(3);
    torque_der(1);
    torque_sum(1);
    torque_der(2);
    torque_sum(2);
    torque_der(3);
    torque_sum(3);
    ex1(1);
    in1(1);
    ex1(2);
    in1(2);
    ex1(3);
    in1(3);
    ex2(1);
    in2(1);
    ex2(2);
    in2(2);
    ex2(3);
    in2(3);
];
linearized = simplify(taylor(eqns, vars, locations, Order=2));
linear_eqns = linearized(1:2:end) == linearized(2:2:end);
solved = solve(linear_eqns, [xD yD zD uD vD wD phiD thetaD psiD pD qD rD]);
% state_space = [x y z u v w phi theta psi p q r]
% sub out: sub [xD yD zD] for [u v w]
% sub out: sub [phiD thetaD psiD] for [p q r]

disp("system = [");
disp(strcat('    "', string(solved.xD), '", '));
disp(strcat('    "', string(solved.yD), '", '));
disp(strcat('    "', string(solved.zD), '", '));
disp(strcat('    "', string(solved.uD), '", '));
disp(strcat('    "', string(solved.vD), '", '));
disp(strcat('    "', string(solved.wD), '", '));
disp(strcat('    "', string(solved.phiD), '", '));
disp(strcat('    "', string(solved.thetaD), '", '));
disp(strcat('    "', string(solved.psiD), '", '));
disp(strcat('    "', string(solved.pD), '", '));
disp(strcat('    "', string(solved.qD), '", '));
disp(strcat('    "', string(solved.rD), '"'));
disp("]");
