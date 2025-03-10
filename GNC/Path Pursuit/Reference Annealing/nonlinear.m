function f_out = nonlinear(~, ~, ~, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS, M, Ixx, Iyy, Izz, rho)

c_psi = cos(psi);
c_theta = cos(theta);
c_phi = cos(phi);
s_psi = sin(psi);
s_theta = sin(theta);
s_phi = sin(phi);

c_xi = cos(xi);
s_xi = sin(xi);
s_zeta = sin(zeta);

f_out = [
	u*c_psi*c_theta+v*c_phi*s_psi+w*s_phi*s_psi-1.0*w*c_phi*c_psi*s_theta+v*c_psi*s_phi*s_theta;
	v*c_phi*c_psi+w*c_psi*s_phi-u*c_theta*s_psi+w*c_phi*s_psi*s_theta-v*s_phi*s_psi*s_theta;
	u*s_theta+w*c_phi*c_theta-1.0*v*c_theta*s_phi;
	1.0*r*v-1.0*q*w-9.81*c_psi*c_theta+(1.0*T*c_xi*cos(zeta))/M;
	1.0*p*w-9.81*c_phi*s_psi-1.0*r*u-9.81*c_psi*s_phi*s_theta+(1.0*T*c_xi*s_zeta)/M;
	1.0*q*u-1.0*p*v-9.81*s_phi*s_psi-(1.0*T*s_xi)/M+9.81*c_phi*c_psi*s_theta;
	(p*c_theta-1.0*r*c_phi*s_theta+q*s_phi*s_theta)/c_theta;
	q*c_phi+r*s_phi;
	(r*c_phi-q*s_phi)/c_theta;
	tauRCS+Iyy*q*r-1.0*Izz*q*r;
	Izz*p*r-Ixx*p*r-T*rho*s_xi;
	Ixx*p*q-1.0*Iyy*p*q-1.0*T*rho*c_xi*s_zeta;
];

end
