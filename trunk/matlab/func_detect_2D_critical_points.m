% Detect the critical points in a 2D scalr field. The return variables
% represent the local max, min, sadldes as the grid points, and saddles
% within the cells. Each  return variable has 3 columns that represent the
% X and Y coordinates, and the value.
function [C_min, C_saddle, C_wsaddle, C_max] = func_detect_2D_critical_points(V)
V_size = size(V);

%%  
% detect the extrema
V_border = zeros(V_size + 2);
V_border(2:end-1, 2:end-1) = V;
V_border(:, 1) = V_border(:, 2);
V_border(:, end) = V_border(:, end - 1);
V_border(1, :) = V_border(2, :);
V_border(end, :) = V_border(end-1, :);

V_middle = V_border(2:end-1, 2:end-1);
PX = sign(V_border(2:end-1,   3:end)  - V_middle);
NX = sign(V_border(2:end-1,   1:end-2)- V_middle);
PY = sign(V_border(3:end,     2:end-1)- V_middle);
NY = sign(V_border(1:end-2,   2:end-1)- V_middle);

%%
% local min
S_min = (PX >= 0) .* (NX >= 0) .* (PY >= 0) .* (NY >= 0);
I_min = find(S_min);
[Y_min, X_min] = ind2sub(V_size, I_min);
V_min = V_middle(I_min);
C_min = [X_min(:), Y_min(:), V_min];

%%
% local max
S_max = (PX <= 0) .* (NX <= 0) .* (PY <= 0) .* (NY <= 0);
I_min = find(S_max);
[Y_max, X_max] = ind2sub(V_size, I_min);
V_max = V_middle(I_min);
C_max = [X_max(:), Y_max(:), V_max(:)];

%%
% saddle as the grid points
S_saddle = ((PX <= 0) .* (NX <= 0) .* (PY >= 0) .* (NY >= 0)) | ((PX >= 0) .* (NX >= 0) .* (PY <= 0) .* (NY <= 0));
I_saddle = find(S_saddle);
[Y_saddle, X_saddle] = ind2sub(V_size, I_saddle);
V_saddle = V_middle(I_saddle);
C_saddle = [X_saddle(:), Y_saddle(:), V_saddle(:)];

%%
% saddles that are within the cells
L00 = V(1:end-1, 1:end-1);
L01 = V(2:end, 1:end-1);
L10 = V(1:end-1, 2:end);
L11 = V(2:end, 2:end);
L_det = L00 + L11 - L01 - L10;
S_wsaddle = (L00 .* L11 + L01 .* L10) ./ L_det;
LS = (L00 - L01) ./ L_det;
LT = (L00 - L10) ./ L_det;
LI_wsaddle = find(0 <= LS & LS <= 1 & 0 <= LT & LT <= 1 ); % & 0 <= S_wsaddle & S_wsaddle <= log2(n_bins));
[Y_wsaddle, X_wsaddle] = ind2sub(size(L_det), LI_wsaddle);
X_wsaddle = [X_wsaddle(:) + LS(LI_wsaddle)];
Y_wsaddle = [Y_wsaddle(:) + LT(LI_wsaddle)];
V_wsaddle = interp2(V, X_wsaddle, Y_wsaddle);
C_wsaddle = [X_saddle(:), Y_saddle(:), V_saddle(:)];
end
