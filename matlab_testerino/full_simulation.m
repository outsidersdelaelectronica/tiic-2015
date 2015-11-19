
fs = 250;
%t= 0:1/fs:50;

%load('ECG.mat');
% aux = ECGwaveGen(70,10,fs,2500);
% EKG1 = aux(:);
% L = size(EKG1,1);
% 
% %%%%%%%%%%%%%%%%%%%
% bp_buffer_x_0 = [0,0];
% bp_buffer_x_1 = [0,0];
% bp_buffer_x_2 = [0,0];
% bp_buffer_x_3 = [0,0];
% bp_buffer_x_4 = [0,0];
% bp_buffer_x_5 = [0,0];
% 
% bp_buffer_y_0 = [0,0];
% bp_buffer_y_1 = [0,0];
% bp_buffer_y_2 = [0,0];
% bp_buffer_y_3 = [
%     0,0];
% bp_buffer_y_4 = [0,0];
% bp_buffer_y_5 = [0,0];
% 
% coef = [1024, -1857, 1024, -1881, 981,...
%         1024, -2036, 1024, -2013, 1007, ...
%         1024, -1463, 1024, -1878, 928, 	...
%         1024, -2044, 1024, -1957, 963, 	...
%         1024, -1888, 1024, -1896, 1015, ...
%         1024, -2034, 1024, -2029, 1021];
% %%%%%%%%%%%%
% lp_buffer_x = [0,0,0,0];
% 
% lp_coef = [ 2, 1, 0, -1, -2];
% %%%%%%%%%%%%
% in_buffer = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, ....
%              0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
% 
% %%%%%%%%%%%%
% bp_signal = zeros(1,L); 
% lp_signal = zeros(1,L); 
% sq_signal = zeros(1,L); 
% in_signal = zeros(1,L); 
% 
% for i = 1:L
%     %%%%%%%%%%%%%%%%%%%%
%     x_n = EKG1(i);
%     y_n = (coef(1) * x_n + coef(2) * bp_buffer_x_0(1) + coef(3) * bp_buffer_x_0(2) ...
% 				- coef(4) * bp_buffer_y_0(1) - coef(5) * bp_buffer_y_0(2))/1024;
%     bp_buffer_y_0(2) = bp_buffer_y_0(1);
%     bp_buffer_y_0(1) = y_n;
%     bp_buffer_x_0(2) = bp_buffer_x_0(1);
%     bp_buffer_x_0(1) = x_n;
%     x_n = y_n;
% 
%     y_n = (coef(6) * x_n + coef(7) * bp_buffer_x_1(1) + coef(8) * bp_buffer_x_1(2) ...
% 				- coef(9) * bp_buffer_y_1(1) - coef(10) * bp_buffer_y_1(2))/1024;
%     bp_buffer_y_1(2) = bp_buffer_y_1(1);
%     bp_buffer_y_1(1) = y_n;
%     bp_buffer_x_1(2) = bp_buffer_x_1(1);
%     bp_buffer_x_1(1) = x_n;
% 
%     x_n = y_n;
% 
%     y_n = (coef(11) * x_n + coef(12) * bp_buffer_x_2(1) + coef(13) * bp_buffer_x_2(2) ...
% 				- coef(14) * bp_buffer_y_2(1) - coef(15) * bp_buffer_y_2(2))/1024;
%     bp_buffer_y_2(2) = bp_buffer_y_2(1);
%     bp_buffer_y_2(1) = y_n;
%     bp_buffer_x_2(2) = bp_buffer_x_2(1);
%     bp_buffer_x_2(1) = x_n;
% 
%     x_n = y_n;
% 
%     y_n = (coef(16) * x_n + coef(17) * bp_buffer_x_3(1) + coef(18) * bp_buffer_x_3(2) ...
% 				- coef(19) * bp_buffer_y_3(1) - coef(20) * bp_buffer_y_3(2))/1024;
%     bp_buffer_y_3(2) = bp_buffer_y_3(1);
%     bp_buffer_y_3(1) = y_n;
%     bp_buffer_x_3(2) = bp_buffer_x_3(1);
%     bp_buffer_x_3(1) = x_n;
%     x_n = y_n;
% 
%     y_n = (coef(21) * x_n + coef(22) * bp_buffer_x_4(1) + coef(23) * bp_buffer_x_4(2) ...
% 				- coef(24) * bp_buffer_y_4(1) - coef(25) * bp_buffer_y_4(2))/1024;
%     bp_buffer_y_4(2) = bp_buffer_y_4(1);
%     bp_buffer_y_4(1) = y_n;
%     bp_buffer_x_4(2) = bp_buffer_x_4(1);
%     bp_buffer_x_4(1) = x_n;
%     x_n = y_n;
% 
%     y_n = (coef(26) * x_n + coef(27) * bp_buffer_x_5(1) + coef(28) * bp_buffer_x_5(2) ...
% 				- coef(29) * bp_buffer_y_5(1) - coef(30) * bp_buffer_y_5(2))/1024;
%     bp_buffer_y_5(2) = bp_buffer_y_5(1);
%     bp_buffer_y_5(1) = y_n;
%     bp_buffer_x_5(2) = bp_buffer_x_5(1);
%     bp_buffer_x_5(1) = x_n;
%     
%     bp_signal(i) = y_n;
%     x_n = y_n;
%     %%%%%%%%%%%%%%%%
%     
%     %%%%%%%%%%%%%%%%
%     y_n = (lp_coef(1) * x_n + lp_coef(2) * lp_buffer_x(1) + lp_coef(3) * lp_buffer_x(2) ...
%             + lp_coef(4) * lp_buffer_x(3) + lp_coef(5) * lp_buffer_x(4))/8;
%     
%     lp_buffer_x(4) = lp_buffer_x(3);
%     lp_buffer_x(3) = lp_buffer_x(2);
%     lp_buffer_x(2) = lp_buffer_x(1);
%     lp_buffer_x(1) = y_n;   
%     
%     lp_signal(i) = y_n;
%     x_n = y_n;   
%     %%%%%%%%%%%%%%%%
%     
%     %%%%%%%%%%%%%%%%
%     
%     y_n = (x_n * x_n)/256;
%     
%     sq_signal(i) = y_n;
%     x_n = y_n;
%     
%     %%%%%%%%%%%%%%%
%     
%     %%%%%%%%%%%%%%%
%     
%     for j= 32:-1:2
%         y_n = y_n + in_buffer(j);
%         in_buffer(j) = in_buffer(j-1);
%     end
%     y_n = y_n + in_buffer(1);
%     in_buffer(1) = x_n; 
%     
%     in_signal(i) = y_n/32;
%      x_n = y_n;
%     
% end
% 
% subplot(2,2,1)
% plot(bp_signal)
% 
% subplot(2,2,2)
% plot(lp_signal)
% 
% subplot(2,2,3)
% plot(sq_signal)
% 
% subplot(2,2,4)
% plot(in_signal)

ws = load('ws.mat');

in_signal = ws.in_signal(:);
L = size(in_signal,1); 

max_array = zeros(1,10);
window_indexes = zeros(1,8);
max_pos = zeros(1,10);
threshold = 100000;

index_1 = 1;
index_2 = 1;
index_3 = 1;

maxerino = 0;
maxerino_pos = 0;
for i = 1:floor(L/fs)
    %%%% Create a windows of at least fs samples
    if i < floor(L/fs)
        window = in_signal((1 + fs*i):(fs*(i+1)));  
    else
        window = in_signal((1 + fs*i):numel(in_signal));
    end
    j = 1;
    hold on
    plot(window)
    plot(threshold * ones(size(window)))

    while (j < numel(window))
        %%%%% Iterates over the windows
        if window(j) > 0.9 * threshold
            %%%% When a sample is higher than threshold
            %%%% stores the index
            window_indexes(index_1) = j;
            index_1 = index_1 + 1;
            
            while  (window(j) > 0.9 * threshold)&&(j < numel(window))   
                %%%%% Iterates over the windows
                %%%%% while is higher than the TH
                if (maxerino < window(j))
                    %%%% Stores max
                    maxerino = window(j);
                    maxerino_pos = j;
                end
                j = j + 1; 
            end
            %%%%% Stores the end of the sub windows and the 
            %%%%% value and position of the local max.
            window_indexes(index_1) = (j - 1);
            max_array(index_2) = maxerino;
            max_pos(index_3) = maxerino_pos;
            
            index_1 = index_1 + 1;
            
            if index_2 < 10
                index_2 = index_2 + 1;
            else
                index_2 = 1;
            end
            
            index_3 = index_3 + 1;
            
            maxerino = 0;
            theshold = sum(max_array)/10;
            
        else
            j = j + 1;
        end

    end
    %%%% After went over the window, we calculate 
    %%%% the bpm
    if (mod(index_3,2) == 0)&&(index_3 > 1)
    %%% If the index is even means that and odd number of maximun are stored
        for j = 1:(index_3 - 3)
            dif = max_pos(j + 1) - max_pos(j);
            bpm = 60*fs/dif
        end
        max_pos(1) = max_pos(index_3 - 1) - fs;
        index_3 = 2;
    elseif (index_3 < 2)
        index_3 = 2;
    else
        for j = 1:(index_3 - 2)
            dif = max_pos(j + 1) - max_pos(j);
            bpm = 60*fs/dif
        end
        index_3 = 1;
    end
end





%     hold off
%     max = 0;
%     j = 1;
%     while (j < numel(window))   
%         % Iterates over the window to extract points 
%         % above the threshold
%         if window(j) > 0.9 * threshold
%             % when a point pass the threshold
%             % is stored as the beggining of a 
%             % threshold window
%             window_indexes(index_1) = j;
%             index_1 = index_1 + 1;
%             while  window(j) > 0.9 * threshold
%                 % looks for the end of the th windows
%                 % and meanwhile finds the local max
%                 % storing its position
%                 j = j + 1;
%                 if (max < window(j))&&(j < fs)
%                     max = window(j);
%                     max_pos(index_3) = j;
% 
%                 end
%                 if j > fs - 1
%                     break
%                 end
%             end
%             if j < fs
%                 window_indexes(index_1) = j;
%                 index_1 = index_1 + 1;  
%                 max_array(index_2) = max;
%                 threshold = sum(max_array)/10;
%             end
%         else
%             j = j + 1;
%         end
% 
%         index_3 = index_3 + 1;
%         if index_2 > 9 
%             index_2 = 1;
%         else
%             index_2 = index_2 + 1;
%         end
%         max = 0;
%     end 
%     
%     for j = 1:(index_3 - 2)
%         dif = max_pos(j + 1) - max_pos(j);
%         bpm = 60*fs/dif
%     end
%     
%     if mod(index_1,2) > 0
%         window_indexes(1) = window_indexes(index_1) - fs;
%         index_1 = 2;
%     else
%         index_1 = 1;
%     end
%     
%     if mod(index_3,2) == 0
%         max_pos(1) = max_pos(index_2);
%         index_3 = 2;
%     else
%         index_3 = 1;
%     end