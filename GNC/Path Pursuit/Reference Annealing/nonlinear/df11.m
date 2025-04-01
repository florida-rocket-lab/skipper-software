function grad = df11(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

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
	Izz*r - Ixx*r;
	0;
	Izz*p - Ixx*p;
	-rho*sin(xi);
	-T*rho*cos(xi);
	0;
	0;
];

end