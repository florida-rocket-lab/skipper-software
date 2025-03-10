function grad = df3(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	sin(theta);
	-1.0*cos(theta)*sin(phi);
	cos(phi)*cos(theta);
	- 1.0*v*cos(phi)*cos(theta) - w*cos(theta)*sin(phi);
	u*cos(theta) - w*cos(phi)*sin(theta) + 1.0*v*sin(phi)*sin(theta);
	0;
	0;
	0;
	0;
	0;
	0;
	0;
	0;
];

end