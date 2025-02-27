% +----------------------------------------------------------------------+
% |                Equations of Motion Linearization                     |
% +----------------------------------------------------------------------+
% | FOR USE BY FRL GNC AND AVIONICS SUBTEAM FOR SKIPPER VTOL MONOCOPTER  |
% | EDITORS:  LOGAN DAPP                                                 |
% | DATE:     26.2.2025                                                  |
% +----------------------------------------------------------------------+

eol;

A_vars = [x y z u v w phi theta psi p q r];
B_vars = [T xi zeta tauRCS];
A = equationsToMatrix(lin_eqns, A_vars);
B = equationsToMatrix(lin_eqns, B_vars);
[i_, c] = equationsToMatrix(lin_eqns, expansion);
fprintf("A = [\n"); fprintf("\t%s %s %s %s %s %s %s %s %s %s %s %s;\n", erase(string(simplify(A)), " ") ); fprintf("];\n");
fprintf("B = [\n"); fprintf("\t%s %s %s %s;\n", erase(string(simplify(B)), " ") ); fprintf("];\n");
fprintf("c = [\n"); fprintf("\t%s;\n", erase(string(simplify(c)), " ") ); fprintf("];\n");

% A = [
% 	0 0 0 0 0 0 0 0 0 0 0 0;
% 	0 0 0 0 0 0 0 0 0 0 0 0;
% 	0 0 0 0 0 0 0 0 0 0 0 0;
% 	cos(psi0)*cos(theta0) -1.0*cos(theta0)*sin(psi0) sin(theta0) 0 -1.0*r0 q0 0 0 0 0 0 0;
% 	cos(phi0)*sin(psi0)+cos(psi0)*sin(phi0)*sin(theta0) cos(phi0)*cos(psi0)-1.0*sin(phi0)*sin(psi0)*sin(theta0) -1.0*cos(theta0)*sin(phi0) r0 0 -1.0*p0 0 0 0 0 0 0;
% 	sin(phi0)*sin(psi0)-cos(phi0)*cos(psi0)*sin(theta0) cos(psi0)*sin(phi0)+cos(phi0)*sin(psi0)*sin(theta0) cos(phi0)*cos(theta0) -1.0*q0 p0 0 0 0 0 0 0 0;
% 	w0*cos(phi0)*sin(psi0)-1.0*v0*sin(phi0)*sin(psi0)+v0*cos(phi0)*cos(psi0)*sin(theta0)+w0*cos(psi0)*sin(phi0)*sin(theta0) w0*cos(phi0)*cos(psi0)-v0*cos(psi0)*sin(phi0)-v0*cos(phi0)*sin(psi0)*sin(theta0)-w0*sin(phi0)*sin(psi0)*sin(theta0) -cos(theta0)*(v0*cos(phi0)+w0*sin(phi0)) 0 9.81*sin(phi0)*sin(psi0)-9.81*cos(phi0)*cos(psi0)*sin(theta0) -9.81*cos(phi0)*sin(psi0)-9.81*cos(psi0)*sin(phi0)*sin(theta0) (sin(theta0)*(q0*cos(phi0)+r0*sin(phi0)))/cos(theta0) r0*cos(phi0)-q0*sin(phi0) -(1.0*(q0*cos(phi0)+r0*sin(phi0)))/cos(theta0) 0 0 0;
% 	-cos(psi0)*(u0*sin(theta0)+w0*cos(phi0)*cos(theta0)-1.0*v0*cos(theta0)*sin(phi0)) sin(psi0)*(u0*sin(theta0)+w0*cos(phi0)*cos(theta0)-1.0*v0*cos(theta0)*sin(phi0)) u0*cos(theta0)-1.0*w0*cos(phi0)*sin(theta0)+v0*sin(phi0)*sin(theta0) 9.81*cos(psi0)*sin(theta0) -9.81*cos(psi0)*cos(theta0)*sin(phi0) 9.81*cos(phi0)*cos(psi0)*cos(theta0) -(1.0*(r0*cos(phi0)-q0*sin(phi0)))/cos(theta0)^2 0 (sin(theta0)*(r0*cos(phi0)-q0*sin(phi0)))/cos(theta0)^2 0 0 0;
% 	v0*cos(phi0)*cos(psi0)+w0*cos(psi0)*sin(phi0)-u0*cos(theta0)*sin(psi0)+w0*cos(phi0)*sin(psi0)*sin(theta0)-v0*sin(phi0)*sin(psi0)*sin(theta0) w0*cos(phi0)*cos(psi0)*sin(theta0)-v0*cos(phi0)*sin(psi0)-w0*sin(phi0)*sin(psi0)-u0*cos(psi0)*cos(theta0)-v0*cos(psi0)*sin(phi0)*sin(theta0) 0 9.81*cos(theta0)*sin(psi0) 9.81*sin(phi0)*sin(psi0)*sin(theta0)-9.81*cos(phi0)*cos(psi0) -9.81*cos(psi0)*sin(phi0)-9.81*cos(phi0)*sin(psi0)*sin(theta0) 0 0 0 0 0 0;
% 	0 0 0 0 w0 -1.0*v0 1 0 0 0 -r0*(Ixx-1.0*Izz) q0*(Ixx-1.0*Iyy);
% 	0 0 0 -1.0*w0 0 u0 (sin(phi0)*sin(theta0))/cos(theta0) cos(phi0) -(1.0*sin(phi0))/cos(theta0) r0*(Iyy-1.0*Izz) 0 p0*(Ixx-1.0*Iyy);
% 	0 0 0 v0 -1.0*u0 0 -(1.0*cos(phi0)*sin(theta0))/cos(theta0) sin(phi0) cos(phi0)/cos(theta0) q0*(Iyy-1.0*Izz) -p0*(Ixx-1.0*Izz) 0;
% ];

% B = [
% 	0 0 0 (cos(xi0)*cos(zeta0))/M;
% 	(cos(xi0)*sin(zeta0))/M -(1.0*sin(xi0))/M 0 0;
% 	0 0 -1.0*rho*sin(xi0) -1.0*rho*cos(xi0)*sin(zeta0);
% 	0 0 0 -(1.0*T0*cos(zeta0)*sin(xi0))/M;
% 	-(1.0*T0*sin(xi0)*sin(zeta0))/M -(1.0*T0*cos(xi0))/M 0 0;
% 	0 0 -1.0*T0*rho*cos(xi0) T0*rho*sin(xi0)*sin(zeta0);
% 	0 0 0 -(1.0*T0*cos(xi0)*sin(zeta0))/M;
% 	(T0*cos(xi0)*cos(zeta0))/M 0 0 0;
% 	0 0 0 -1.0*T0*rho*cos(xi0)*cos(zeta0);
% 	0 0 0 0;
% 	0 0 0 0;
% 	0 1.0 0 0;
% ];

% c = [
% 	psi0*v0*cos(phi0)*cos(psi0)-1.0*theta0*u0*cos(psi0)*sin(theta0)-1.0*phi0*v0*sin(phi0)*sin(psi0)-1.0*psi0*u0*cos(theta0)*sin(psi0)+phi0*w0*cos(phi0)*sin(psi0)+psi0*w0*cos(psi0)*sin(phi0)-1.0*theta0*w0*cos(phi0)*cos(psi0)*cos(theta0)+phi0*v0*cos(phi0)*cos(psi0)*sin(theta0)+theta0*v0*cos(psi0)*cos(theta0)*sin(phi0)+phi0*w0*cos(psi0)*sin(phi0)*sin(theta0)+psi0*w0*cos(phi0)*sin(psi0)*sin(theta0)-1.0*psi0*v0*sin(phi0)*sin(psi0)*sin(theta0);
% 	theta0*u0*sin(psi0)*sin(theta0)-psi0*w0*sin(phi0)*sin(psi0)+phi0*w0*cos(phi0)*cos(psi0)-psi0*u0*cos(psi0)*cos(theta0)-phi0*v0*cos(psi0)*sin(phi0)-psi0*v0*cos(phi0)*sin(psi0)+psi0*w0*cos(phi0)*cos(psi0)*sin(theta0)+theta0*w0*cos(phi0)*cos(theta0)*sin(psi0)-phi0*v0*cos(phi0)*sin(psi0)*sin(theta0)-psi0*v0*cos(psi0)*sin(phi0)*sin(theta0)-theta0*v0*cos(theta0)*sin(phi0)*sin(psi0)-phi0*w0*sin(phi0)*sin(psi0)*sin(theta0);
% 	theta0*u0*cos(theta0)-phi0*w0*cos(theta0)*sin(phi0)-theta0*w0*cos(phi0)*sin(theta0)+theta0*v0*sin(phi0)*sin(theta0)-phi0*v0*cos(phi0)*cos(theta0);
% 	9.81*cos(psi0)*cos(theta0)-1.0*q0*w0+r0*v0+9.81*psi0*cos(theta0)*sin(psi0)+9.81*theta0*cos(psi0)*sin(theta0)-(1.0*T0*xi0*cos(zeta0)*sin(xi0))/M-(1.0*T0*zeta0*cos(xi0)*sin(zeta0))/M;
% 	9.81*cos(phi0)*sin(psi0)+p0*w0-1.0*r0*u0-9.81*psi0*cos(phi0)*cos(psi0)+9.81*phi0*sin(phi0)*sin(psi0)+9.81*cos(psi0)*sin(phi0)*sin(theta0)-9.81*phi0*cos(phi0)*cos(psi0)*sin(theta0)-9.81*theta0*cos(psi0)*cos(theta0)*sin(phi0)+9.81*psi0*sin(phi0)*sin(psi0)*sin(theta0)+(T0*zeta0*cos(xi0)*cos(zeta0))/M-(1.0*T0*xi0*sin(xi0)*sin(zeta0))/M;
% 	9.81*sin(phi0)*sin(psi0)-1.0*p0*v0+q0*u0-9.81*phi0*cos(phi0)*sin(psi0)-9.81*psi0*cos(psi0)*sin(phi0)-9.81*cos(phi0)*cos(psi0)*sin(theta0)+9.81*theta0*cos(phi0)*cos(psi0)*cos(theta0)-9.81*phi0*cos(psi0)*sin(phi0)*sin(theta0)-9.81*psi0*cos(phi0)*sin(psi0)*sin(theta0)-(1.0*T0*xi0*cos(xi0))/M;
% 	(q0*theta0*sin(phi0)-1.0*r0*theta0*cos(phi0)+phi0*q0*cos(phi0)*cos(theta0)*sin(theta0)+phi0*r0*cos(theta0)*sin(phi0)*sin(theta0))/cos(theta0)^2;
% 	1.0*phi0*(r0*cos(phi0)-q0*sin(phi0));
% 	-(1.0*(q0*theta0*sin(phi0)*sin(theta0)-1.0*r0*theta0*cos(phi0)*sin(theta0)+phi0*q0*cos(phi0)*cos(theta0)+phi0*r0*cos(theta0)*sin(phi0)))/cos(theta0)^2;
% 	q0*r0*(Iyy-1.0*Izz);
% 	Izz*p0*r0-Ixx*p0*r0-T0*rho*xi0*cos(xi0);
% 	Ixx*p0*q0-1.0*Iyy*p0*q0-1.0*T0*rho*zeta0*cos(xi0)*cos(zeta0)+T0*rho*xi0*sin(xi0)*sin(zeta0);
% ];