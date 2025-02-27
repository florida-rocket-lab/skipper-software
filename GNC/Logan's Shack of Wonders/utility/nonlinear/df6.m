function grad = df6(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	1.0*q;
	-1.0*p;
	0;
	- 9.81*cos(phi)*sin(psi) - 9.81*cos(psi)*sin(phi)*sin(theta);
	9.81*cos(phi)*cos(psi)*cos(theta);
	- 9.81*cos(psi)*sin(phi) - 9.81*cos(phi)*sin(psi)*sin(theta);
	-1.0*v;
	1.0*u;
	0;
	-(1.0*sin(xi))/M;
	-(1.0*T*cos(xi))/M;
	0;
	0;
];

end