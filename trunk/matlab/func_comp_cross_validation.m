function cv = func_comp_cross_validation(H, w)
    n = nansum(H(:)); % n: #samples
    P = H / n;  % P: pmf
 
    cv_nom = 2 - nansum(P.^2) * (n + 1);
    cv_denom = (n - 1) * w;
    cv  = cv_nom / cv_denom;
end