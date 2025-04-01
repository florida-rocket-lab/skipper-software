% +----------------------------------------------------------------------+
% |                Equations of Motion Error Estimation                  |
% +----------------------------------------------------------------------+
% | FOR USE BY FRL GNC AND AVIONICS SUBTEAM FOR SKIPPER VTOL MONOCOPTER  |
% | EDITORS:  LOGAN DAPP                                                 |
% | DATE:     26.2.2025                                                  |
% +----------------------------------------------------------------------+

eol;

qua_eqns = expand(taylor(eqns, expansion, constants, 'Order', 3));
qua_error = simplify(expand(lin_eqns - qua_eqns));  % expensive

vars = [(x-x0) (y-y0) (z-z0) (u-u0) (v-v0) (w-0) (phi-phi0) (theta-theta0) ...
    (psi-psi0) (p-p0) (q-q0) (r-r0) (T-T0) (xi-xi0) (zeta-zeta0) (tauRCS-tauRCS0)];
pfix = 2*ones(1, length(vars)^2) - reshape(eye(length(vars), length(vars)), [], length(vars)^2);
pvars = pfix .* reshape(vars.' * vars, [], length(vars)^2);  % taking outer product generates all pairs, although some are redundant, so computation time slows down; however, it makes reshaping into a tensor easier later :)
hessian = [];

i_ = 1;
for pvar = pvars
    [Q, R] = quorem(qua_error, pvar);
    hessian = [hessian; Q];  % VERY EXPENSIVE
    i_ = i_ + 1;
    fprintf("Completed %g/%g computations...\n", i_-1, length(pvars))
end

% hessian is 256 x 12, but really its a 12 x 16 x 16 (1,2) tensor.
% We'll keep it folded to 12 x 256 for easy transfer, but in actual
% implementation, note that it will be reshaped to 12 x 16 x 16.
hessian = hessian.';
fid = fopen("computeerror.m", "W");
fprintf(fid, "hessian = [\n"); fprintf(fid, "\t%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s ;\n", erase(string(simplify(hessian)), " ") ); fprintf(fid, "];\n");


% fprintf("H = [\n"); fprintf("\t%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s;\n", erase(string(simplify(hessian)), " ") ); fprintf("];\n");
