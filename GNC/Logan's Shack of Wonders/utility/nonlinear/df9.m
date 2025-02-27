function grad = df9(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	0;
	0;
	0;
	-(q*cos(phi) + r*sin(phi))/cos(theta);
	(sin(theta)*(r*cos(phi) - q*sin(phi)))/cos(theta)^2;
	0;
	0;
	-sin(phi)/cos(theta);
	cos(phi)/cos(theta);
	0;
	0;
	0;
	0;
];

end