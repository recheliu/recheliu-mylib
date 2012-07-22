% Compute the integral histogram.
% The input is a grid of bins. 
function IH = func_comp_integral_hist(B, n_bins)
    B_size = size(B);
    IH = zeros([B_size, n_bins]);
    
    switch(length(B_size))
        case 2
            [X, Y] = meshgrid(1:B_size(2), 1:B_size(1));
            IHI = sub2ind(size(IH), Y(:), X(:), B(:));
        case 3
            [X, Y, Z]  = meshgrid(1:B_size(2), 1:B_size(1), 1:B_Size(3));
            IHI = sub2ind(size(IH), Y(:), X(:), Z(:), B(:));
    end
	IH(IHI) = 1;
    
    for d = 1:length(B_size)
        IH = cumsum(IH, d);
    end
end