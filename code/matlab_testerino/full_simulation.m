clear
load('ECG.mat');


L = size(EKG1,1);     % Length of signal
Fs = 1000;         % Sampling frequency
T = 1/Fs;          % Sampling period
t = (0:(L-1))*T;   % Time vector

f = Fs*(0:(L-1))/L;

x = zeros(1,L);
x = EKG1;
% x(1) = 1;

% subplot(2,1,1);
% hold on
% plot(t, x,'black');
% plot(f,20*log(abs(fft(x))));
% xlabel('f (Hz)');
% xlabel('t (s)');
%%%%%%%%%%%%%%%%%%%

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
% norm = max(dc_bloqued);
% plot(t, dc_bloqued./norm,'black');

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

% plot(f,10*log(abs(fft(lp_filtered_151hz))));
% norm = max(lp_filtered_151hz);
% plot(t, lp_filtered_151hz./norm,'green');
% hold off

% subplot(2,1,2);
hold on

% norm = max(x);
% p1 = plot(t, x(1:L/8)./norm,'black');

lp_order_41hz = 146;
lp_folder_coef_41hz = lp_order_41hz /2;

low_pass_coef_41hz = [1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0,...
    1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 0, -1, -2, -3, -4, -4, -5, -4, -4, -3, -2,...
    -1, 1, 3, 5, 6, 8, 8, 9, 8, 7, 5, 2, -1, -4, -8, -11, -14, -17, -18, -18,...
    -16, -13, -8, -1, 7, 16, 27, 37, 48, 59, 68, 76, 83, 87, 90, 90, 87, 83, ...
    76, 68, 59, 48, 37, 27, 16, 7, -1, -8, -13, -16, -18, -18, -17, -14, -11,...
    -8, -4, -1, 2, 5, 7, 8, 9, 8, 8, 6, 5, 3, 1, -1, -2, -3, -4, -4, -5, -4,...
    -4, -3, -2, -1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, -1, -1, -1, -1,...
    -1, -1, -1, -1, 0, 0, 0, 0, 0, 1];
lp_filtered = zeros(1,L);

lp_buffer = zeros(1,lp_order_41hz);

for i = 1:L
    y_n = 0;
    lp_buffer(1) = x(i);
    for j = 1:lp_folder_coef_41hz
        y_n = y_n + ((low_pass_coef_41hz(j) * (lp_buffer(j) + lp_buffer(lp_order_41hz + 1 - j)))/1024); 
    end
    lp_filtered(i) = y_n;
    
    for j = lp_order_41hz:-1:2
        lp_buffer(j) = lp_buffer(j-1);
    end  
end

% plot(f,20*log(abs(fft(lp_filtered))),'black');
% norm = max(lp_filtered_151hz);
% p2 = plot(t, lp_filtered_151hz((1+lp_folder_coef_151hz):(L/8+lp_folder_coef_151hz))./norm,'green');

dif_order = 10;
dif_folder_coef = dif_order /2;

all_pass_diff_coef = [20, -40, 56, -148, 1306, -1306, 148, -56, 40, -20];

dif_filtered = zeros(1,L);

dif_buffer = zeros(1,dif_order);

for i = 1:L
    y_n = 0;
%     dif_buffer(1) = lp_filtered_151hz(i);
    dif_buffer(1) = lp_filtered(i);
    for j = 1:dif_folder_coef
        y_n = y_n + ((all_pass_diff_coef(j) * (dif_buffer(j) - dif_buffer(dif_order + 1 - j)))/1024);
    end
    dif_filtered(i) = 4*y_n;
    
    for j = dif_order:-1:2
        dif_buffer(j) = dif_buffer(j-1);
    end  
end

% plot(f,20*log(abs(fft(dif_filtered))),'green');
% norm = max(dif_filtered);
% p3 = plot(t, dif_filtered./norm,'blue');

absolute_value = zeros(1,L);
prev_max = 1000;
max_value = 1;
for i = 1:L
    absolute_value(i) = abs(dif_filtered(i));
    prev_max = max_value;

end

% plot(f,20*log(abs(fft(squared))),'red');
% norm = max(absolute_value);
% p4 = plot(t, absolute_value./norm,'red');

integrated = zeros(1,L);
integration_length = 16;

int_buffer = zeros(1,integration_length);

for i = 1:L
    int_buffer(1) = absolute_value(i);
    y_n = 0;
    for j =1:integration_length
        y_n = y_n + int_buffer(j);
    end
    integrated(i) = y_n/integration_length;
    for j =integration_length:-1:2
        int_buffer(j) = int_buffer(j - 1);
    end
end

% plot(f,20*log(abs(fft(integrated))),'blue');
norm = max(integrated);
p5 = plot(t, integrated./norm,'yellow');

threshold = max(integrated);
maxerino = 0;
prev_value = 0;
maxerino_pos = 0;
sample_counter = 0;
bpm = 0;

bpm_log_counter_1 = 1;
threshold_log = zeros(1,L);
for i = 1:L
    ecg_lead = integrated(i);
    if(ecg_lead > threshold)

        if (ecg_lead > maxerino)
            maxerino =         ecg_lead;
            maxerino_pos =     sample_counter;
        end
    elseif (prev_value > threshold)
%         bpm =              (60 * Fs) / maxerino_pos;
        bpm = maxerino_pos;
        if bpm_log_counter_1 > 3
            bpm_log_1(bpm_log_counter_1 - 3) = i;
        end
        bpm_log_counter_1 = bpm_log_counter_1 + 1;
        sample_counter =   sample_counter - maxerino_pos - 1;
        threshold =        (threshold + maxerino*3) /4;
        maxerino =         0;
        ecg_lead =         0;
    else
        threshold = (threshold*1023)/1024;
    end
    threshold_log(i) = threshold;
    sample_counter = sample_counter+1;
    prev_value = ecg_lead;
end

norm = max(threshold_log);
p5 = plot(t, threshold_log./norm,'red');

threshold = max(dc_bloqued);
maxerino = 0;
prev_value = 0;
maxerino_pos = 0;
sample_counter = 0;
bpm = 0;

bpm_log_counter_2 = 1;
threshold_log = zeros(1,L);
for i = 1:L
    ecg_lead = dc_bloqued(i);
    if(ecg_lead > threshold)

        if (ecg_lead > maxerino)
            maxerino =         ecg_lead;
            maxerino_pos =     sample_counter;
        end
        
    elseif (prev_value > threshold)
%         bpm =              (60 * Fs) / maxerino_pos;
        bpm = maxerino_pos;
        if bpm_log_counter_2 > 3
            bpm_log_2(bpm_log_counter_2 - 3) = i;
        end
        bpm_log_counter_2 = bpm_log_counter_2 + 1;
        sample_counter =   sample_counter - maxerino_pos - 1;
        threshold =        (threshold + maxerino*3) /4;
        maxerino =         0;
        ecg_lead =         0;

    else
        threshold = (threshold*1023)/1024;
    end
    threshold_log(i) = threshold;
    sample_counter = sample_counter+1;
    prev_value = ecg_lead;
end

norm = max(threshold_log);
plot(t,threshold_log./norm,'green')

index = min(bpm_log_counter_2,bpm_log_counter_1);

[acor,lag] = xcorr(bpm_log_1,bpm_log_2);

[~,I] = max(abs(acor));
lagDiff = lag(I)
timeDiff = lagDiff/Fs

% plot(lag,acor)

for i = 1:min(size(bpm_log_1,2),size(bpm_log_2,2))
   diff_log(i) =  bpm_log_1(i) - bpm_log_2(i);
end
average_diff = mean(diff_log)
% %  RETRASO TOTAL ENTRE DC_BLOQUED Y INTEGRATED 103
hold off