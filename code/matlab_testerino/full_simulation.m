clear
load('ECG.mat');

% %  RETRASO MEDIO ENTRE DC_BLOQUED Y INTEGRATED 97
L = max(size(EKG1));     % Length of signal
Fs = 1000;         % Sampling frequency
T = 1/Fs;          % Sampling period
t = (0:(L-1))*T;   % Time vector

f = Fs*(0:(L-1))/L;

x = show_filtering(EKG1);

hold on

% norm = max(x);
% plot(t, x./norm,'black');
% plot(t, x,'black');
xlabel('t (s)');

% plot(f,20*log(abs(fft(x))));
% xlabel('f (Hz)');

%%%%%%%%%%%%%%%%%%%

% hold off

preprocessed_clean = bpm_preprocessing(x);
% norm = max(preprocessed_clean);
% plot(t, preprocessed_clean./norm,'black');
plot(t, preprocessed_clean,'black');

bpm_log_clean = bpm_decision_module(preprocessed_clean);

x_norm = x./max(x);
for i = 1:L
    if x_norm(i) < 0.8
        x_norm(i) = 0;
    end
end

[pks,locs] = findpeaks(x_norm);

for i = 2:max(size(locs))
    bpm_log_clean(i-1) = (60*Fs)/(locs(i) - locs(i-1));
end

x_30db = awgn(x,30,'measured');
% norm = max(x_30db);
% plot(t, x_30db./norm,'green');
preprocessed_30db =  bpm_preprocessing(x_30db);

bpm_log_30db = bpm_decision_module(preprocessed_30db);

for i = 2:min(size(bpm_log_clean,2),size(bpm_log_30db,2))
   diff_log(i) =  bpm_log_clean(i) - bpm_log_30db(i);
   rel_diff_log(i) =  (bpm_log_clean(i) - bpm_log_30db(i))/bpm_log_clean(i);
end
average_diff_30dB = mean(diff_log);
[max_diff_30dB,I] = max(abs(diff_log));
std_diff_30dB = std(diff_log);
average_rel_diff_30dB = mean(rel_diff_log)*100;
max_rel_diff_30dB = max(abs(rel_diff_log))*100;
std_rel_diff_30dB = std(diff_log)*100;

quality_measures_30dB = [average_diff_30dB,max_diff_30dB,std_diff_30dB,average_rel_diff_30dB,...
    max_rel_diff_30dB,std_rel_diff_30dB]
x_20db = awgn(x,20,'measured');
% norm = max(x_20db);
% plot(t, x_20db./norm,'green');

preprocessed_20db =  bpm_preprocessing(x_20db);

bpm_log_20db = bpm_decision_module(preprocessed_20db);

for i = 2:min(size(bpm_log_clean,2),size(bpm_log_20db,2))
   diff_log(i) =  (bpm_log_clean(i) - bpm_log_20db(i));
   rel_diff_log(i) =  ((bpm_log_clean(i) - bpm_log_20db(i))/bpm_log_clean(i));
end
average_diff_20db = mean(diff_log);
[max_diff_20db,I] = max(abs(diff_log));
std_diff_20db = std(diff_log);
average_rel_diff_20db = mean(rel_diff_log)*100;
max_rel_diff_20db = max(abs(rel_diff_log))*100;
std_rel_diff_20db = std(rel_diff_log)*100;

quality_measures_20db = [average_diff_20db,max_diff_20db,std_diff_20db,average_rel_diff_20db,...
    max_rel_diff_20db,std_rel_diff_20db]

x_10db = awgn(x,10,'measured');
% norm = max(x_10db);
% plot(t, x_10db./norm,'green');

preprocessed_10db =  bpm_preprocessing(x_10db);

bpm_log_10db = bpm_decision_module(preprocessed_10db);

for i = 2:min(size(bpm_log_clean,2),size(bpm_log_10db,2))
   diff_log(i) =  (bpm_log_clean(i) - bpm_log_10db(i));
   rel_diff_log(i) =  ((bpm_log_clean(i) - bpm_log_10db(i))/bpm_log_clean(i));
end
average_diff_10db = mean(diff_log);
[max_diff_10db,I] = max(abs(diff_log));
std_diff_10db = std(diff_log);
average_rel_diff_10db = mean(rel_diff_log)*100;
max_rel_diff_10db = max(abs(rel_diff_log))*100;
std_rel_diff_10db = std(rel_diff_log)*100;

quality_measures_10db = [average_diff_10db,max_diff_10db,std_diff_10db,average_rel_diff_10db,...
    max_rel_diff_10db,std_rel_diff_10db]

x_6db = awgn(x,6,'measured');
% norm = max(x_6db);
% plot(t, x_6db./norm,'green');

preprocessed_6db =  bpm_preprocessing(x_6db);

bpm_log_6db = bpm_decision_module(preprocessed_6db);

for i = 2:min(size(bpm_log_clean,2),size(bpm_log_6db,2))
   diff_log(i) =  (bpm_log_clean(i) - bpm_log_6db(i));
   rel_diff_log(i) =  ((bpm_log_clean(i) - bpm_log_6db(i))/bpm_log_clean(i));
end
average_diff_6db = mean(diff_log);
[max_diff_6db,I] = max(abs(diff_log));
std_diff_6db = std(diff_log);
average_rel_diff_6db = mean(rel_diff_log)*100;
max_rel_diff_6db = max(abs(rel_diff_log))*100;
std_rel_diff_6db = std(rel_diff_log)*100;

quality_measures_6db = [average_diff_6db,max_diff_6db,std_diff_6db,average_rel_diff_6db,...
    max_rel_diff_6db,std_rel_diff_6db]

x_3db = awgn(x,3,'measured');
% norm = max(x_3db);
% plot(t, x_3db./norm,'green');

preprocessed_3db =  bpm_preprocessing(x_3db);

bpm_log_3db = bpm_decision_module(preprocessed_3db);

for i = 2:min(size(bpm_log_clean,2),size(bpm_log_3db,2))
   diff_log(i) =  (bpm_log_clean(i) - bpm_log_3db(i));
   rel_diff_log(i) =  ((bpm_log_clean(i) - bpm_log_3db(i))/bpm_log_clean(i));
end
average_diff_3db = mean(diff_log);
[max_diff_3db,I] = max(abs(diff_log));
std_diff_3db = std(diff_log);
average_rel_diff_3db = mean(rel_diff_log)*100;
max_rel_diff_3db = max(abs(rel_diff_log))*100;
std_rel_diff_3db = std(rel_diff_log)*100;

quality_measures_3db = [average_diff_3db,max_diff_3db,std_diff_3db,average_rel_diff_3db,...
    max_rel_diff_3db,std_diff_3db]

x_0db = awgn(x,0,'measured');
% norm = max(x_0db);
% plot(t, x_0db./norm,'green');

preprocessed_0db =  bpm_preprocessing(x_0db);

bpm_log_0db = bpm_decision_module(preprocessed_0db);

for i = 2:min(size(bpm_log_clean,2),size(bpm_log_0db,2))
   diff_log(i) =  (bpm_log_clean(i) - bpm_log_0db(i));
   rel_diff_log(i) =  ((bpm_log_clean(i) - bpm_log_0db(i))/bpm_log_clean(i));
end
average_diff_0db = mean(diff_log);
[max_diff_0db,I] = max(abs(diff_log));
std_diff_0db = std(diff_log);
average_rel_diff_0db = mean(rel_diff_log)*100;
max_rel_diff_0db = max(abs(rel_diff_log))*100;
std_rel_diff_0db = std(rel_diff_log)*100;

quality_measures_0db = [average_diff_0db,max_diff_0db,std_diff_0db,average_rel_diff_0db,...
    max_rel_diff_0db,std_rel_diff_0db]

% [acor,lag] = xcorr(bpm_log_clean,bpm_log_2);

% [~,I] = max(abs(acor));
% lagDiff = lag(I)
% timeDiff = lagDiff/Fs

% plot(lag,acor)
% hold off

