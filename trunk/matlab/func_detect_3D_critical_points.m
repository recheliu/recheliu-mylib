function [C_min, C_saddle, C_wsaddle, C_max] = func_detect_3D_critical_points(V)
    V_size = size(V);

    % find the critical points
    V_border = zeros(V_size + 2);
    V_border(2:end-1, 2:end-1, 2:end-1) = V;
    V_border(1, :, :)       =   V_border(2, :, :);
    V_border(end-1, :, :)   =   V_border(end-2, :, :);
    V_border(:, 1, :)       =   V_border(:, 2, :);
    V_border(:, end-1, :)   =   V_border(:, end-2, :);
    V_border(:, :, 1)       =   V_border(:, :, 2);
    V_border(:, :, end-1)   =   V_border(:, :, end-2);

    % Nx: the volume of flags about the comparison with the neighbors in X
    % direction
    % Ny and Nz: similar to Nx
    Px = sign(V_border(2:end-1, 3:end, 2:end-1) - V);
    Nx = sign(V_border(2:end-1, 1:end-2, 2:end-1) - V);
    Py = sign(V_border(3:end, 2:end-1, 2:end-1) - V);
    Ny = sign(V_border(1:end-2, 2:end-1, 2:end-1) - V);
    Pz = sign(V_border(2:end-1, 2:end-1, 3:end) - V);
    Nz = sign(V_border(2:end-1, 2:end-1, 1:end-2) - V);

    I_min = find( ...
        0 < Py + Ny & ...
        0 < Px + Nx & ...
        0 < Pz + Nz ...
        );
    [Y_min, X_min, Z_min] = ind2sub(size(V), I_min);
    V_min = V(I_min);
    C_min = [X_min(:), Y_min(:), Z_min(:), V_min(:)];

    I_max = find( ...
        0 > Py + Ny & ...
        0 > Px + Nx & ...
        0 > Pz + Nz ...
        );
    [Y_max, X_max, Z_max] = ind2sub(size(V), I_max);
    V_max = V(I_max);
    C_max = [X_max(:), Y_max(:), Z_max(:), V_max(:)];

    I_critical = find( ...
        0 <= Py .* Ny & 0 ~= Py + Ny & ...
        0 <= Px .* Nx & 0 ~= Px + Nx & ...
        0 <= Pz .* Nz & 0 ~= Pz + Nz ...
        );

    I_saddle = setxor(I_critical(:), [I_min(:); I_max(:)]);
    [Y_saddle, X_saddle, Z_saddle] = ind2sub(size(V), I_saddle);
    V_saddle= V(I_saddle);
    C_saddle = [X_saddle(:), Y_saddle(:), Z_saddle(:), V_saddle(:)];

    clear V_border Px Py Pz Nx Ny Nz;    % ADD-BY-LEETEN 07/06/2012

    %% detect the saddles within the cells
    [CX, CY, CZ] = meshgrid(0:1, 0:1, 0:1);
    T = cell(2, 2, 2);
    T{1, 1, 1} = V(1:end-1, 1:end-1,1:end-1);
    T{2, 1, 1} = V(2:end,   1:end-1,1:end-1);
    T{1, 1, 2} = V(1:end-1, 1:end-1,2:end);
    T{2, 1, 2} = V(2:end,   1:end-1,2:end);
    T{1, 2, 1} = V(1:end-1, 2:end,  1:end-1);
    T{2, 2, 1} = V(2:end,   2:end,  1:end-1);
    T{1, 2, 2} = V(1:end-1, 2:end,  2:end);
    T{2, 2, 2} = V(2:end,   2:end,  2:end);

    Va = func_get_poly_coef(T, 1, 1, 1);
    Vb = func_get_poly_coef(T, 1, 1, 0);
    Vc = func_get_poly_coef(T, 1, 0, 1); 
    Vd = func_get_poly_coef(T, 0, 1, 1);
    Ve = func_get_poly_coef(T, 0, 1, 0);
    Vf = func_get_poly_coef(T, 1, 0, 0);
    Vg = func_get_poly_coef(T, 0, 0, 1);
    Vh = func_get_poly_coef(T, 0, 0, 0);
    clear T;    % ADD-BY-LEETEN 07/06/2012

    Vax = (Va .* Ve - Vb .* Vd);
    Vay = (Va .* Vf - Vb .* Vc);
    Vaz = (Va .* Vg - Vc .* Vd);
    Vaxyz = Vax .* Vay .* Vaz;
    Vbcd = Vb .* Vc .* Vd;
    V_GT = Vax .* Vay .* Vaz;
    V_DisCT = ...
        (Va .* Vh).^2 + (Vb .* Vg).^2 + (Vc .* Ve).^2 + (Vd .* Vf).^2 ...
        - 2 * Va .* Vb .* Vg .* Vh ...
        - 2 * Va .* Vc .* Ve .* Vh ...
        - 2 * Va .* Vd .* Vf .* Vh ...
        - 2 * Vb .* Vc .* Ve .* Vg ...
        - 2 * Vb .* Vd .* Vf .* Vg ...
        - 2 * Vc .* Vd .* Ve .* Vf ...
        + 4 * Va .* Ve .* Vf .* Vg ...
        + 4 * Vb .* Vc .* Vd .* Vh;

    %% find the case of 2 saddles
    Y_wsaddle = [];
    X_wsaddle = [];
    Z_wsaddle = [];
    V_wsaddle = [];
    for n_saddles = [1 2]
        switch n_saddles
            case 2
                candidate_cell_inds = find( V_DisCT > 0 & 0 ~= V_GT & 0 ~= Va & 0 > Vaxyz );

            case 1
                candidate_cell_inds = find( V_DisCT > 0 & 0 ~= V_GT & 0 == Va & 0 ~= Vbcd );
        end

        n_candidate_cells = length(candidate_cell_inds);
        [yis, xis, zis] = ind2sub(V_size - 1, candidate_cell_inds);
        for cci = 1:n_candidate_cells
            candidate_cell_ind = candidate_cell_inds(cci);
            xi = xis(cci);
            yi = yis(cci);
            zi = zis(cci);
            a = Va(candidate_cell_ind);
            b = Vb(candidate_cell_ind);
            c = Vc(candidate_cell_ind);
            d = Vd(candidate_cell_ind);
            e = Ve(candidate_cell_ind);
            f = Vf(candidate_cell_ind);
            g = Vg(candidate_cell_ind);
            h = Vh(candidate_cell_ind);
            ax = Vax(candidate_cell_ind);
            ay = Vay(candidate_cell_ind);
            az = Vaz(candidate_cell_ind);
            axyz = Vaxyz(candidate_cell_ind);
            GT = V_GT(candidate_cell_ind);
            DisCT = V_DisCT(candidate_cell_ind);

            C = V(yi + (0:1), xi + (0:1), zi + (0:1));

            switch n_saddles
                case 2
                    x = -c / a + sqrt(-axyz) / (a * ax); 
                    y = -d / a + sqrt(-axyz) / (a * ay); 
                    z = -b / a + sqrt(-axyz) / (a * az); 
                    if( 0 <= x && x <= 1 && 0 <= y && y <= 1 && 0 <= z && z <= 1 )
                        saddle = interp3(CX, CY, CZ, C, x, y, z);
                        X_wsaddle(end+1) = xi + x;
                        Y_wsaddle(end+1) = yi + y;
                        Z_wsaddle(end+1) = zi + z;
                        V_wsaddle(end+1) = saddle;
                    end
                    x = -c / a - sqrt(-axyz) / (a * ax); 
                    y = -d / a - sqrt(-axyz) / (a * ay); 
                    z = -b / a - sqrt(-axyz) / (a * az); 
                    if( 0 <= x && x <= 1 && 0 <= y && y <= 1 && 0 <= z && z <= 1 )
                        saddle = interp3(CX, CY, CZ, C, x, y, z);
                        X_wsaddle(end+1) = xi + x;
                        Y_wsaddle(end+1) = yi + y;
                        Z_wsaddle(end+1) = zi + z;
                        V_wsaddle(end+1) = saddle;
                    end
                case 1
                    x = (c * e - b * g - d * f) / (2 * b * d);
                    y = (d * f - b * g - c * e) / (2 * b * c);
                    z = (b * g - c * e - d * f) / (2 * c * d);
                    if( 0 <= x && x <= 1 && 0 <= y && y <= 1 && 0 <= z && z <= 1 )
                        saddle = interp3(CX, CY, CZ, C, x, y, z);
                        X_wsaddle(end+1) = xi + x;
                        Y_wsaddle(end+1) = yi + y;
                        Z_wsaddle(end+1) = zi + z;
                        V_wsaddle(end+1) = saddle;
                    end
            end
        end
    end
    C_wsaddle = [X_wsaddle(:), Y_wsaddle(:), Z_wsaddle(:), V_wsaddle(:)];
end

function c = func_get_poly_coef(T, yf, xf, zf)
    if( 0 == xf && 0 == yf && 0 == zf )
        c = T{1, 1, 1};
    end

    if( 1 == xf && 0 == yf && 0 == zf )
        c = -T{1, 1, 1} + T{1, 2, 1};
    end

    if( 0 == xf && 1 == yf && 0 == zf )
        c = -T{1, 1, 1} + T{2, 1, 1};
    end

    if( 0 == xf && 0 == yf && 1 == zf )
        c = -T{1, 1, 1} + T{1, 1, 2};
    end

    if( 0 == xf && 1 == yf && 1 == zf )
        c = T{1, 1, 1} - T{2, 1, 1} - T{1, 1, 2} + T{2, 1, 2};
    end

    if( 1 == xf && 0 == yf && 1 == zf )
        c = T{1, 1, 1} - T{1, 2, 1} - T{1, 1, 2} + T{1, 2, 2};
    end

    if( 1 == xf && 1 == yf && 0 == zf )
        c = T{1, 1, 1} - T{1, 2, 1} - T{2, 1, 1} + T{2, 2, 1};
    end

    if( 1 == xf && 1 == yf && 1 == zf )
        c = -T{1, 1, 1} + T{1, 1, 2} + T{1, 2, 1} + T{2, 1, 1} - T{1, 2, 2} - T{2, 1, 2} - T{2, 2, 1} + T{2, 2, 2};
    end
end