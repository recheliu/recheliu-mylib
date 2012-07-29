% MOD-BY-LEETEN 07/28/2012-FROM:    function [vx, vy, dim] = func_read_vec_2D(filename)
function [vx, vy, dim] = func_read_vec_2D(filename, varargin)
    zoffset = 0;
    switch(length(varargin))
        case 1
            zoffset = varargin{1};
    end           
% MOD-BY-LEETEN 07/28/2012-END
    fid = fopen(filename,'rb');

    dim=fread(fid,3,'int32');
    fseek(fid, 3 * dim(1) * dim(2) * zoffset * 4, 'cof');   % ADD-BY-LEETEN 07/29/2012
    vec= fread(fid, 3 * dim(1)*dim(2), 'single');
    fclose(fid);

    vec = reshape(vec, 3, dim(1), dim(2));
    vec = permute(vec, [1 3 2]);    % ADD-BY-LEETEN 07/22/2012
    vx = squeeze(vec(1, :, :));
    vy = squeeze(vec(2, :, :));
    % DEL-BY-LEETEN 07/22/2012-BEGIN
        % vx = vx';
        % vy = vy';
    % DEL-BY-LEETEN 07/22/2012-END
    dim = size(vx);
end

%
% $Log: func_read_vec_2D.m,v $
% Revision 1.1.1.1  2010/11/05 20:34:36  leeten
%
% [11/05/2010]
% 1. [1ST] First time checkin.
%
%
