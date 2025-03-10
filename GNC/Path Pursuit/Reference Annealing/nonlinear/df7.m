function grad = df7(x, y, z, u, v, w, phi, theta, psi, p, q, r, T, xi, zeta, tauRCS)

grad = [
	0;
	0;
	0;
	0;
	0;
	0;
	(q*cos(phi)*sin(theta) + 1.0*r*sin(phi)*sin(theta))/cos(theta);
	(sin(theta)*(p*cos(theta) - 1.0*r*cos(phi)*sin(theta) + q*sin(phi)*sin(theta)))/cos(theta)^2 - (p*sin(theta) + 1.0*r*cos(phi)*cos(theta) - q*cos(theta)*sin(phi))/cos(theta);
	0;
	1;
	(sin(phi)*sin(theta))/cos(theta);
	-(1.0*cos(phi)*sin(theta))/cos(theta);
	0;
	0;
	0;
	0;
];

end