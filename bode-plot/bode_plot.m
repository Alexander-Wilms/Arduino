clc; close all;
% measurements of a PT1
R = 10^3;
C = 10^-6;
f = [62500 62500 62500 62500 62500];
x = [1 8 64 256 1024];
a = f./x
w = a*2*3.14
y = [517 533 637 886 1022];
y = 20 * log (y./1022)

s = tf('s');
a = R * C
sys = 1 / (1+a*s)
bodemag(sys)
hold on
semilogx(w,y,'r');
f_eck=1/(R*C); %Eckfrequenz
hold on
line([f_eck f_eck], get(gca,'YLim'))