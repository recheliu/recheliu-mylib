% decide the #rows & #columns 
% dim: x x y, 2D dimension of X & Y
% aspect_ratios: W:H

% n_rows x n_cols: M x N

% the derivation: Find the M and N such that 
% (1)  (x * N) / (y * M) ~ W /H
% and
% (2)  (M - 1) N < F <= MN.
%
% From (1), we get 
% 
% (3) M = N (x H) / (y W). 
%
% By plugging (3) to (2), we get
%
% F <= N^2 (x H ) / (y W) 
% and
% N^2 (x H) / (y W) - N < F
%
% By writting alpha = (x H) / (y W), we get
% 
% F / alpha <= N^2
% (1 - sqrt(1 + 4 alpha F))/(2 alpha) < N < (1 + sqrt(1 + 4 alpha F))/(2 alpha)
% 
function [n_rows, n_cols] = func_subplot(n_figs, dim, aspect_ratios)
    n_cols = ceil(sqrt(n_figs));
    n_rows = ceil(n_figs/n_cols);
    % MOD-BY-LEETEN 07/22/2012-FROM:        % alpha = aspect_ratios(1) * dim(2) / (aspect_ratios(2) * dim(1));
    alpha = aspect_ratios(2) * dim(1) / (aspect_ratios(1) * dim(2));
    % MOD-BY-LEETEN 07/22/2012-END
    min_N = max([1, sqrt(n_figs/alpha), (1 - sqrt(1 + 4 * alpha * n_figs))/(2 * alpha)], [], 2);
    max_N = (1 + sqrt(1 + 4 * alpha * n_figs))/(2 * alpha);
    test_Ns = floor(min_N):ceil(max_N);
    test_pairs = []; % ADD-BY-LEETEN 07/22/2012
    for test_N = test_Ns
        min_M = n_figs/test_N;
        max_M = min_M + 1;
        % MOD-BY-LEETEN 07/22/2012-FROM:
            % test_Ms = ceil(min_M):floor(max_M);
            % if( ~isempty(test_Ms) )
                % if( test_Ms(1) * (test_N - 1) < n_figs && n_figs <= test_Ms(1) * test_N ) 
                    % n_cols = test_Ms(1);
                    % n_rows = test_N;
                    % break;
                % end
            % end
        % MOD-BY-LEETEN 07/22/2012-TO:
        test_Ms = floor(min_M):ceil(max_M);
        for test_M = test_Ms
            if( test_M * (test_N - 1) < n_figs && n_figs <= test_M * test_N )
                test_pairs(end+1, :) = [test_M, test_N];
            end
        end
        % MOD-BY-LEETEN 07/22/2012-END
    end
    % ADD-BY-LEETEN 07/22/2012-BEGIN
    test_scores = ((test_pairs(:, 2) * dim(1)) ./ (test_pairs(:, 1) * dim(2)) - aspect_ratios(1) / aspect_ratios(2)) .^ 2;
    [min_test_score, min_test_scorei] = min(test_scores);
    n_cols = test_pairs(min_test_scorei, 2);
    n_rows = test_pairs(min_test_scorei, 1);
    % ADD-BY-LEETEN 07/22/2012-END
end