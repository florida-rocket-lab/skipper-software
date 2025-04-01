function grad = df10(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

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
	0;
	Iyy*r - 1.0*Izz*r;
	Iyy*q - 1.0*Izz*q;
	0;
	0;
	0;
	1.0;
];

end