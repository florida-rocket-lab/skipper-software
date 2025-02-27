function out_mat = eulermat(axis, angle)
%EULERMAT Produces a *intrinsic* Euler rotation matrix.
%   Thanks to Nicholas Hirsch for the code inspiration.
    switch (axis)
        case 1
            out_mat = [
                1 0 0 ;
                0 cos(angle) -sin(angle) ;
                0 sin(angle) cos(angle)
            ];
        case 2
            out_mat = [
                cos(angle) 0 sin(angle) ;
                0 1 0;
                -sin(angle) 0 cos(angle)
            ];
        case 3
            out_mat = [
                cos(angle) -sin(angle) 0 ;
                sin(angle) cos(angle) 0 ;
                0 0 1
            ];
        otherwise
            error("The axis provided to eulermat(axis, angle) must be 1, 2, or 3.");
    end
end

