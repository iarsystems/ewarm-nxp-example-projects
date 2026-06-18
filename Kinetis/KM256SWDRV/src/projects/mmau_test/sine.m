function y = sine(x)
% Input arguments: x = -1 to 1, corresponds to the angle -pi to pi.
% Returns sine of input angle in range -1 to 1.
% Example: k=1;for i=-1:0.01:1 e(k)=sin(i*pi)-sine(i);k=k+1; end;plot((-1:0.01:1).*pi,floor(e*2^31));xlabel('Angle [rad]');ylabel('LSB');title ('Function Error - y=sine(x)'); grid
% Freescale Semiconductor, 2015
a = [1/2 -pi^2/FACTORIAL(3)/2 pi^4/FACTORIAL(5)/2 -pi^6/FACTORIAL(7)/2 pi^8/FACTORIAL(9)/2 -pi^10/FACTORIAL(11)/2 pi^12/FACTORIAL(13)/2];
if     x >  0.5
   x = 1.0-x;
elseif x < -0.5
   x = -1.0-x;
end
y = pi*2*(((((((((((((a(7)*x)*x+a(6))*x)*x+a(5))*x)*x+a(4))*x)*x+a(3))*x)*x+a(2))*x)*x+a(1))*x);