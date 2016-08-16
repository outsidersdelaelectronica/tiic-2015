clear
load('ECG.mat');

x = EKG1;
L = size(x,1);     % Length of signal
Fs = 1000;         % Sampling frequency
T = 1/Fs;          % Sampling period
t = (0:(L-1))*T;   % Time vector

f = Fs*(0:(L-1))/L;

hold on

norm = max(x);
plot(t, x./norm,'black');

prev_x = 0;
prev_y = 0;
dife = 0; 
inte = 0;
dc_bloqued = zeros(1,L);

for i = 1:L
   dife = x(i) - prev_x;
   prev_x = x(i);
   inte = (1022/1024)*prev_y + dife;
   prev_y = inte;  
   dc_bloqued(i) = inte;
end

% plot(f,20*log(abs(fft(dc_bloqued))));
% plot(t, dc_bloqued,'green');

lp_order_151hz = 141;
lp_folder_coef_151hz = (lp_order_151hz-1) /2;

low_pass_coef_151hz = [0, 1, 1, 1, 1, 0, -1, -1, 0, 1, 1, 1, -1, -1, -1, 0,...
    2, 2, 0, -2, -2, -1, 2, 3, 1, -2, -3, -2, 1, 4, 4, 0, -4, -5, -1, 4, 6,...
     3, -3, -7, -5, 2, 8, 8, 0, -8, -10, -3, 8, 13, 7, -7, -16, -12, 4, 19,...
     18, 0, -21, -27, -8, 23, 40, 21, -25, -64, -54, 26, 152, 268, 315, 268,...
     152, 26, -54, -64, -25, 21, 40, 23, -8, -27, -21, 0, 18, 19, 4, -12,...
     -16, -7, 7, 13, 8, -3, -10, -8, 0, 8, 8, 2, -5, -7, -3, 3, 6, 4, -1,...
     -5, -4, 0, 4, 4, 1, -2, -3, -2, 1, 3, 2, -1, -2, -2, 0, 2, 2, 0, -1,...
     -1, -1, 1, 1, 1, 0, -1, -1, 0, 1, 1, 1, 1, 0];
lp_filtered_151hz = zeros(1,L);

lp_buffer = zeros(1,lp_order_151hz);

for i = 1:L
    y_n = 0;
    lp_buffer(1) = dc_bloqued(i);
    for j = 1:lp_folder_coef_151hz
        y_n = y_n + ((low_pass_coef_151hz(j) * (lp_buffer(j) + lp_buffer(lp_order_151hz + 1 - j)))/1024); 
    end
    lp_filtered_151hz(i) = y_n + (low_pass_coef_151hz(lp_folder_coef_151hz + 1) * lp_buffer(lp_folder_coef_151hz + 1))/1024;
    
    for j = lp_order_151hz:-1:2
        lp_buffer(j) = lp_buffer(j-1);
    end  
end

norm = max(lp_filtered_151hz);
plot(t, lp_filtered_151hz./norm,'green');

