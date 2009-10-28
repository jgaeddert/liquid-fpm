%
% Piecewise approximation to besseli0 using low-order polynomials
%
% Takes advantage of the following properties of I0(z):
%   (1) log(log( I0(z) )) ~ 1.993*log(z) - 1.409    for z < 1
%   (2) log(log( I0(z) )) ~ 1.081*log(z) - 0.382    for z > 10
%

clear all;
close all;

% region boundaries
r0 = -6.6;
r1 = -0.5;
r2 =  2.3;
r3 =  6.6;

dr = 0.02;

% polynomial orders
k1 = 3;
k2 = 3;
k3 = 3;

% evaluate region 1
z1 = 2.^[r0:dr:r1];
t1 = log2(z1);
y1 = log2(log2(besseli(0,z1)));
pR1 = polyfit(t1,y1,k1);
y1_prime = polyval(pR1,t1);

% evaluate region 2
z2 = 2.^[r1:dr:r2];
t2 = log2(z2);
y2 = log2(log2(besseli(0,z2)));
pR2 = polyfit(t2,y2,k2);
y2_prime = polyval(pR2,t2);

% evaluate region 3
z3 = 2.^[r2:dr:r3];
t3 = log2(z3);
y3 = log2(log2(besseli(0,z3)));
pR3 = polyfit(t3,y3,k3);
y3_prime = polyval(pR3,t3);

z = [z1 z2 z3];
t = [t1 t2 t3];
y = [y1 y2 y3];
y_prime = [y1_prime y2_prime y3_prime];

e = y_prime - y;
[emax ei] = max(abs(e));
rmse = sqrt(mean(e.^2))

figure;
subplot(2,1,1);
    semilogx(z,y,z,y_prime);
    xlabel('z');
    ylabel('log_2(log_2( I_0(z) ) )');
    grid on;
subplot(2,1,2);
    semilogx(z,y-y_prime,z(ei),y(ei)-y_prime(ei),'or'); % error
    xlabel('z');
    ylabel('absolute error');
    title(['error: max: ' num2str(emax) ', rmse : ' num2str(rmse)]);
    grid on;

disp('regions (t = log2(z))');
r1
r2

disp('polynomials:');
pR1
pR2
pR3

