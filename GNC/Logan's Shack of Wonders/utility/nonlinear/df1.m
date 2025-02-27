function grad = df1(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	cos(psi)*cos(theta);
	cos(phi)*sin(psi) + cos(psi)*sin(phi)*sin(theta);
	sin(phi)*sin(psi) - 1.0*cos(phi)*cos(psi)*sin(theta);
	w*cos(phi)*sin(psi) - v*sin(phi)*sin(psi) + v*cos(phi)*cos(psi)*sin(theta) + 1.0*w*cos(psi)*sin(phi)*sin(theta);
	v*cos(psi)*cos(theta)*sin(phi) - 1.0*w*cos(phi)*cos(psi)*cos(theta) - u*cos(psi)*sin(theta);
	v*cos(phi)*cos(psi) + w*cos(psi)*sin(phi) - u*cos(theta)*sin(psi) + 1.0*w*cos(phi)*sin(psi)*sin(theta) - v*sin(phi)*sin(psi)*sin(theta);
	0;
	0;
	0;
	0;
	0;
	0;
	0;
];

end