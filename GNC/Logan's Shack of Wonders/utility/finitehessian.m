function sigma_max = finitehessian(df, x0, num_iter, tol, eps)
    if nargin < 3, num_iter = 100; end
    if nargin < 4, tol = 1e-6; end
    if nargin < 5, eps = 1e-5; end

    n = length(x0);
    v = randn(n, 1); % Random initial vector
    v = v / norm(v); % Normalize
    ieps = 1.0 / eps;

    grad_f = df(x0);
    for k = 1:num_iter
        Hv = ieps * (df(x0+eps*v) - grad_f);
        v_new = Hv / norm(Hv); % Normalize
        
        % Check for convergence
        if norm(v_new - v) < tol
            break;
        end
        
        v = v_new;
    end

    sigma_max = norm(Hv); % Approximate largest singular value
end
