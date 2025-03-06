function grad = df12(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	0;
	0;
	0;
	0;
	0;
	0;
	Ixx*q - 1.0*Iyy*q;
	Ixx*p - 1.0*Iyy*p;
	0;
	-1.0*rho*cos(xi)*sin(zeta);
	1.0*T*rho*sin(xi)*sin(zeta);
	-1.0*T*rho*cos(xi)*cos(zeta);
	0;
];

end