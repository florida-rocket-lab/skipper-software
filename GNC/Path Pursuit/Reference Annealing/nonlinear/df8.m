function grad = df8(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	0;
	0;
	0;
	r*cos(phi) - q*sin(phi);
	0;
	0;
	0;
	cos(phi);
	sin(phi);
	0;
	0;
	0;
	0;
];

end