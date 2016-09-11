clear
load('ECG.mat');

L = max(size(EKG2));
Fs = 1000;         % Sampling frequency
T = 1/Fs;          % Sampling period
t = (0:(L-1))*T;   % Time vector

f = Fs*(0:(L-1))/L;

x = EKG2;
x = zeros( 1,L);
x(1) = 1;

hold on
% plot(t, x, 'green');
% plot(f,20*log(abs(fft(x))));
xlabel('f (Hz)');

sowh = show_filtering(x);
% plot(t, sowh(:), 'BLUE');

SOWH = fft(sowh);
plot(f(1:L/2),10*log(abs(SOWH(1:L/2))));
plot(f(1:L/2),unwrap(angle(SOWH(1:L/2))));

[bhp,ahp] = ellip(3,0.1,30,0.5/500,'high');

F = 0.01:0.001:0.5;
g = grpdelay(bhp,ahp,F,2); % We will only equalize the passband.
Gdhp = max(g)-g;
[numhp,denhp]=iirgrpdelay(8, F, [0.01 0.5], g);


xhp = filter(bhp,ahp,x);
Xhp = fft(xhp);

xhps = filter(numhp,denhp,xhp);
Xhps = fft(xhps);

% plot(xhps,'red');

% [b50s,a50s] = ellip(3,0.1,30,[59.5/500 60.5/500]);
% 
% F = 0.01:0.001:0.5;
% g = grpdelay(b50s,a50s,F,2); % We will only equalize the passband.
% Gd = max(g)-g;
% [num50sup,den50s]=iirgrpdelay(8, F, [0.01 0.5], g);
% 
% fvtool(b50s,a50s);
% 
% x50s = filter(b50s,a50s,x);
% X50s = fft(x50s);
% 
% x50ss = filter(num50s,num50s,x50s);
% X50ss = fft(x50ss);

[blp,alp] = ellip(6,0.1,30,150/500);

F = 0.01:0.001:0.25;
g = grpdelay(blp,alp,F,2); % We will only equalize the passband.
Gdhp = max(g)-g;
[numlp,denlp]=iirgrpdelay(8, F, [0.01 0.25], g);


xlp = filter(blp,alp,xhps);
Xlp = fft(xlp);

xlps = filter(numlp,denlp,xlp);
Xlps = fft(xlps);

% plot(xlps,'blue');


xave = zeros(1,length(xlps));


for i = 4:length(xlps)
   for j = 1:3
       xave(i - 3) = xave(i - 3) + xlps(i - j);
   end
   xave(i - 3) = xave(i - 3)/4;
end

% plot(xave,'black');

hold off



