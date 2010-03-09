function y = liquidfpm_atan_compact(g)
% y = liquidfpm_atan_compact(g)
%   compact form for atan2(y,x)
%   r = y/x
%   t = log2(r)
%   g = 1 / (1+t)
%   f(r) = 2/pi*atan(r)
%   
%   f(g) = 2/pi*atan(exp(1/g -1))
y = (2/pi)*atan(2.^(1./g - 1));
