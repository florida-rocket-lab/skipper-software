function [A, B, c] = calculatecoefficients(~, ~, ~, u0, v0, w0, phi0, theta0, psi0, p0, q0, r0, T0, xi0, zeta0, ~, M, Ixx, Iyy, Izz, rho)
%CALCULATECOEFFICIENTS Returns the taylor expansion for the Skipper
% dynamics around the passed-in points.
% 
%   Inputs: 
%       x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS, 
%       M, Ixx, Iyy, Izz, rho
% 
%   Returns: A, B, c
%   A := the state matrix -> x. = Ax + ...
%   B := the control matrix -> x. = ... + Bu
%   c := the inhomogeneous part

c_psi = cos(psi0);
c_theta = cos(theta0);
c_phi = cos(phi0);
s_psi = sin(psi0);
s_theta = sin(theta0);
s_phi = sin(phi0);

c_xi = cos(xi0);
c_zeta = cos(zeta0);
s_xi = sin(xi0);
s_zeta = sin(zeta0);

% sinusoid functions are actually expensive, and I do not know if MATLAB
% is smart enough to memoize the values; I force the memoization myself for
% computational speed-up :)

A = [
	0 0 0 0 0 0 0 0 0 0 0 0;
	0 0 0 0 0 0 0 0 0 0 0 0;
	0 0 0 0 0 0 0 0 0 0 0 0;
	c_psi*c_theta -1.0*c_theta*s_psi s_theta 0 -1.0*r0 q0 0 0 0 0 0 0;
	c_phi*s_psi+c_psi*s_phi*s_theta c_phi*c_psi-1.0*s_phi*s_psi*s_theta -1.0*c_theta*s_phi r0 0 -1.0*p0 0 0 0 0 0 0;
	s_phi*s_psi-c_phi*c_psi*s_theta c_psi*s_phi+c_phi*s_psi*s_theta c_phi*c_theta -1.0*q0 p0 0 0 0 0 0 0 0;
	w0*c_phi*s_psi-1.0*v0*s_phi*s_psi+v0*c_phi*c_psi*s_theta+w0*c_psi*s_phi*s_theta w0*c_phi*c_psi-v0*c_psi*s_phi-v0*c_phi*s_psi*s_theta-w0*s_phi*s_psi*s_theta -c_theta*(v0*c_phi+w0*s_phi) 0 9.81*s_phi*s_psi-9.81*c_phi*c_psi*s_theta -9.81*c_phi*s_psi-9.81*c_psi*s_phi*s_theta (s_theta*(q0*c_phi+r0*s_phi))/c_theta r0*c_phi-q0*s_phi -(1.0*(q0*c_phi+r0*s_phi))/c_theta 0 0 0;
	-c_psi*(u0*s_theta+w0*c_phi*c_theta-1.0*v0*c_theta*s_phi) s_psi*(u0*s_theta+w0*c_phi*c_theta-1.0*v0*c_theta*s_phi) u0*c_theta-1.0*w0*c_phi*s_theta+v0*s_phi*s_theta 9.81*c_psi*s_theta -9.81*c_psi*c_theta*s_phi 9.81*c_phi*c_psi*c_theta -(1.0*(r0*c_phi-q0*s_phi))/c_theta^2 0 (s_theta*(r0*c_phi-q0*s_phi))/c_theta^2 0 0 0;
	v0*c_phi*c_psi+w0*c_psi*s_phi-u0*c_theta*s_psi+w0*c_phi*s_psi*s_theta-v0*s_phi*s_psi*s_theta w0*c_phi*c_psi*s_theta-v0*c_phi*s_psi-w0*s_phi*s_psi-u0*c_psi*c_theta-v0*c_psi*s_phi*s_theta 0 9.81*c_theta*s_psi 9.81*s_phi*s_psi*s_theta-9.81*c_phi*c_psi -9.81*c_psi*s_phi-9.81*c_phi*s_psi*s_theta 0 0 0 0 0 0;
	0 0 0 0 w0 -1.0*v0 1 0 0 0 -r0*(Ixx-1.0*Izz) q0*(Ixx-1.0*Iyy);
	0 0 0 -1.0*w0 0 u0 (s_phi*s_theta)/c_theta c_phi -(1.0*s_phi)/c_theta r0*(Iyy-1.0*Izz) 0 p0*(Ixx-1.0*Iyy);
	0 0 0 v0 -1.0*u0 0 -(1.0*c_phi*s_theta)/c_theta s_phi c_phi/c_theta q0*(Iyy-1.0*Izz) -p0*(Ixx-1.0*Izz) 0;
];
B = [
	0 0 0 (c_xi*c_zeta)/M;
	(c_xi*s_zeta)/M -(1.0*s_xi)/M 0 0;
	0 0 -1.0*rho*s_xi -1.0*rho*c_xi*s_zeta;
	0 0 0 -(1.0*T0*c_zeta*s_xi)/M;
	-(1.0*T0*s_xi*s_zeta)/M -(1.0*T0*c_xi)/M 0 0;
	0 0 -1.0*T0*rho*c_xi T0*rho*s_xi*s_zeta;
	0 0 0 -(1.0*T0*c_xi*s_zeta)/M;
	(T0*c_xi*c_zeta)/M 0 0 0;
	0 0 0 -1.0*T0*rho*c_xi*c_zeta;
	0 0 0 0;
	0 0 0 0;
	0 1.0 0 0;
];
c = [
	psi0*v0*c_phi*c_psi-1.0*theta0*u0*c_psi*s_theta-1.0*phi0*v0*s_phi*s_psi-1.0*psi0*u0*c_theta*s_psi+phi0*w0*c_phi*s_psi+psi0*w0*c_psi*s_phi-1.0*theta0*w0*c_phi*c_psi*c_theta+phi0*v0*c_phi*c_psi*s_theta+theta0*v0*c_psi*c_theta*s_phi+phi0*w0*c_psi*s_phi*s_theta+psi0*w0*c_phi*s_psi*s_theta-1.0*psi0*v0*s_phi*s_psi*s_theta;
	theta0*u0*s_psi*s_theta-psi0*w0*s_phi*s_psi+phi0*w0*c_phi*c_psi-psi0*u0*c_psi*c_theta-phi0*v0*c_psi*s_phi-psi0*v0*c_phi*s_psi+psi0*w0*c_phi*c_psi*s_theta+theta0*w0*c_phi*c_theta*s_psi-phi0*v0*c_phi*s_psi*s_theta-psi0*v0*c_psi*s_phi*s_theta-theta0*v0*c_theta*s_phi*s_psi-phi0*w0*s_phi*s_psi*s_theta;
	theta0*u0*c_theta-phi0*w0*c_theta*s_phi-theta0*w0*c_phi*s_theta+theta0*v0*s_phi*s_theta-phi0*v0*c_phi*c_theta;
	9.81*c_psi*c_theta-1.0*q0*w0+r0*v0+9.81*psi0*c_theta*s_psi+9.81*theta0*c_psi*s_theta-(1.0*T0*xi0*c_zeta*s_xi)/M-(1.0*T0*zeta0*c_xi*s_zeta)/M;
	9.81*c_phi*s_psi+p0*w0-1.0*r0*u0-9.81*psi0*c_phi*c_psi+9.81*phi0*s_phi*s_psi+9.81*c_psi*s_phi*s_theta-9.81*phi0*c_phi*c_psi*s_theta-9.81*theta0*c_psi*c_theta*s_phi+9.81*psi0*s_phi*s_psi*s_theta+(T0*zeta0*c_xi*c_zeta)/M-(1.0*T0*xi0*s_xi*s_zeta)/M;
	9.81*s_phi*s_psi-1.0*p0*v0+q0*u0-9.81*phi0*c_phi*s_psi-9.81*psi0*c_psi*s_phi-9.81*c_phi*c_psi*s_theta+9.81*theta0*c_phi*c_psi*c_theta-9.81*phi0*c_psi*s_phi*s_theta-9.81*psi0*c_phi*s_psi*s_theta-(1.0*T0*xi0*c_xi)/M;
	(q0*theta0*s_phi-1.0*r0*theta0*c_phi+phi0*q0*c_phi*c_theta*s_theta+phi0*r0*c_theta*s_phi*s_theta)/c_theta^2;
	1.0*phi0*(r0*c_phi-q0*s_phi);
	-(1.0*(q0*theta0*s_phi*s_theta-1.0*r0*theta0*c_phi*s_theta+phi0*q0*c_phi*c_theta+phi0*r0*c_theta*s_phi))/c_theta^2;
	q0*r0*(Iyy-1.0*Izz);
	Izz*p0*r0-Ixx*p0*r0-T0*rho*xi0*c_xi;
	Ixx*p0*q0-1.0*Iyy*p0*q0-1.0*T0*rho*zeta0*c_xi*c_zeta+T0*rho*xi0*s_xi*s_zeta;
];
end

