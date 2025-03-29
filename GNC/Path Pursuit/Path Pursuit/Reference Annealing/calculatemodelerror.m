function err = calculatemodelerror(x0, num_iter, tol, eps)

if nargin < 3, num_iter = 100; end
if nargin < 4, tol = 1e-6; end
if nargin < 5, eps = 1e-5; end

err = norm([
    finitehessian(df1, x0, num_iter, tol, eps);
    finitehessian(df2, x0, num_iter, tol, eps);
    finitehessian(df3, x0, num_iter, tol, eps);
    finitehessian(df4, x0, num_iter, tol, eps);
    finitehessian(df5, x0, num_iter, tol, eps);
    finitehessian(df6, x0, num_iter, tol, eps);
    finitehessian(df7, x0, num_iter, tol, eps);
    finitehessian(df8, x0, num_iter, tol, eps);
    finitehessian(df9, x0, num_iter, tol, eps);
    finitehessian(df10, x0, num_iter, tol, eps);
    finitehessian(df11, x0, num_iter, tol, eps);
    finitehessian(df12, x0, num_iter, tol, eps);
]);
end

