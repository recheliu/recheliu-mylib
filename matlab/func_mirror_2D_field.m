% Mirror a 2D vector field. The parameter n is the width of the border to
% be mirrored. 
function mirrored_field = func_mirror_2D_field(v, n)
    if( 0 == n )
        mirrored_field = [v(:, end:-1:1), v, v(:, end:-1:1)];
        mirrored_field = [mirrored_field(end:-1:1, :); mirrored_field; mirrored_field(end:-1:1, :)];
    else
        mirrored_field = [v(:, n:-1:1), v, v(:, end:-1:end-n+1)];
        mirrored_field = [mirrored_field(n:-1:1, :); mirrored_field; mirrored_field(end:-1:end-n+1, :)];
    end
end
