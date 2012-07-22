function [vx, vy, dim] = func_read_vec_2D(filename)
    fid = fopen(filename,'rb');

    dim=fread(fid,3,'int32');
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
