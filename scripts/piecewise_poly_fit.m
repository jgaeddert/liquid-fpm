%
% Piecewise polynomial fit
%

close all;
clear all;

% options
func = @liquidfpm_sin;    % approximating function (function handle)
xmin = 0;       % lower absolute boundary
xmax = 1;    % upper absolute boundary
n = 16;         % number of pieces


order = 2;      % polynomial order (at this time it must be 2)

% 
P = zeros(n,order+1);

% bookkeeping variables
dx = (xmax-xmin)/n;
%xn0 = xmin;     % lower local boundary
%xn1 = xn0+dx;   % upper local boundary

% internal options
res = 20;      % search resolution

for i=1:n,
    xn0 = dx*(i-1) + xmin;  % lower local boundary
    xn1 = dx*(i  ) + xmin;  % upper local boundary

    % search for optimum intermediate value
    e = 0;
    emin = 0;
    x_eval = xn0:(dx/res):xn1;
    y_eval = func(x_eval);
    for j=1:res-1,
        xn_tilda = xn0 + dx*(j/res);
        x_test = [xn0 xn_tilda xn1];
        y_test = func(x_test);

        % polynomial fit
        p = polyfit(x_test,y_test,order);
        e = mean([y_eval - polyval(p,x_eval)].^2);
        if (e < emin || j==1),
            emin = e;
            P(i,:) = p;
        end;
    end;

end;


% plot results
k = max(10,round(1000/n)); % oversampling factor
x = zeros(1,n*k);
f = zeros(1,n*k);
f_hat = zeros(1,n*k);
for i=1:n,
    p = P(i,:);

    % storage indices
    v0 = (i-1)*k + 1;
    v1 = (i  )*k;
    v = v0:v1;

    % evaluation
    x_eval = (v-1)*(dx/k) + xmin;
    x(v) = x_eval;
    f(v) = func(x_eval);
    f_hat(v) = polyval(P(i,:),x_eval);
end;

e_abs = f-f_hat;
rmse = sqrt(mean(e_abs.^2));

figure;
subplot(2,1,1);
    plot(x,f,'-k',x,f_hat,'-b',x(1:k:end),f(1:k:end),'xr');
    xlabel('x');
    ylabel('f(x)');
    title(['Piecewise polynomial approximation (' num2str(rmse) ' RMS error)']);
subplot(2,1,2);
    plot(x,e_abs,'-b',x(1:k:end),e_abs(1:k:end),'xr');
    xlabel('x');
    ylabel('f(x) - f''(x) (absolute error)');

