% decide the #rows & #columns 
% dim: 2D dimension of X & Y
% aspect_ratios: W:H
function [n_rows, n_cols] = func_subplot(n_figs, dim, aspect_ratios)
    n_cols = ceil(sqrt(n_figs));
    n_rows = ceil(n_figs/n_cols);
    alpha = aspect_ratios(1) * dim(2) / (aspect_ratios(2) * dim(1));
    min_N = max([1, sqrt(n_figs/alpha), (1 - sqrt(1 + 4 * alpha * n_figs))/(2 * alpha)], [], 2);
    max_N = (1 + sqrt(1 + 4 * alpha * n_figs))/(2 * alpha);
    test_Ns = floor(min_N):ceil(max_N);
    for test_N = test_Ns
        min_M = n_figs/test_N;
        max_M = min_M + 1;
        test_Ms = ceil(min_M):floor(max_M);
        if( ~isempty(test_Ms) )
            if( test_Ms(1) * (test_N - 1) < n_figs && n_figs <= test_Ms(1) * test_N ) 
                n_cols = test_Ms(1);
                n_rows = test_N;
                break;
            end
        end
    end
end