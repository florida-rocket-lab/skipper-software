function grad = df2(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	-cos(theta)*sin(psi);
	cos(phi)*cos(psi) - sin(phi)*sin(psi)*sin(theta);
	cos(psi)*sin(phi) + cos(phi)*sin(psi)*sin(theta);
	w*cos(phi)*cos(psi) - v*cos(psi)*sin(phi) - v*cos(phi)*sin(psi)*sin(theta) - w*sin(phi)*sin(psi)*sin(theta);
	u*sin(psi)*sin(theta) + w*cos(phi)*cos(theta)*sin(psi) - v*cos(theta)*sin(phi)*sin(psi);
	w*cos(phi)*cos(psi)*sin(theta) - v*cos(phi)*sin(psi) - w*sin(phi)*sin(psi) - u*cos(psi)*cos(theta) - v*cos(psi)*sin(phi)*sin(theta);
	0;
	0;
	0;
	0;
	0;
	0;
	0;
];

end