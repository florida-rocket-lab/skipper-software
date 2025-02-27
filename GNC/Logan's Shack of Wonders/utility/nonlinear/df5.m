function grad = df5(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	-1.0*r;
	0;
	1.0*p;
	9.81*sin(phi)*sin(psi) - 9.81*cos(phi)*cos(psi)*sin(theta);
	-9.81*cos(psi)*cos(theta)*sin(phi);
	9.81*sin(phi)*sin(psi)*sin(theta) - 9.81*cos(phi)*cos(psi);
	1.0*w;
	0;
	-1.0*u;
	(1.0*cos(xi)*sin(zeta))/M;
	-(1.0*T*sin(xi)*sin(zeta))/M;
	(1.0*T*cos(xi)*cos(zeta))/M;
	0;
];

end