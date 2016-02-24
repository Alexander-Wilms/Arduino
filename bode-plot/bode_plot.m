clc; close all;
% measurements of a PT1
R = 10^3;
C = 10^-6;
f = [62500 62500 62500 62500 62500];
x = [1 8 64 256 1024];
a = f./x
y = [520 534 633 885 1022];
semilogx(a,y);
f_eck=1/(R*C); %Eckfrequenz
hold on
line([f_eck f_eck], get(gca,'YLim'))