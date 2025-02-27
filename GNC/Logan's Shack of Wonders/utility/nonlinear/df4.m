function grad = df4(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	0;
	1.0*r;
	-1.0*q;
	0;
	9.81*cos(psi)*sin(theta);
	9.81*cos(theta)*sin(psi);
	0;
	-1.0*w;
	1.0*v;
	(1.0*cos(xi)*cos(zeta))/M;
	-(1.0*T*cos(zeta)*sin(xi))/M;
	-(1.0*T*cos(xi)*sin(zeta))/M;
	0;
];

end