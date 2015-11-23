
fs = 250;
%t= 0:1/fs:50;

load('ECG.mat');
% aux = ECGwaveGen(100,10,fs,2500);
% EKG1 = aux(:);
L = size(EKG1,1);
% plot(EKG1);

%%%%%%%%%%%%%%%%%%%
bp_buffer_x_0 = [0,0];
bp_buffer_x_1 = [0,0];
bp_buffer_x_2 = [0,0];
bp_buffer_x_3 = [0,0];
bp_buffer_x_4 = [0,0];
bp_buffer_x_5 = [0,0];

bp_buffer_y_0 = [0,0];
bp_buffer_y_1 = [0,0];
bp_buffer_y_2 = [0,0];
bp_buffer_y_3 = [
    0,0];
bp_buffer_y_4 = [0,0];
bp_buffer_y_5 = [0,0];

coef = [1024, -1857, 1024, -1881, 981,...
        1024, -2036, 1024, -2013, 1007, ...
        1024, -1463, 1024, -1878, 928, 	...
        1024, -2044, 1024, -1957, 963, 	...
        1024, -1888, 1024, -1896, 1015, ...
        1024, -2034, 1024, -2029, 1021];
%%%%%%%%%%%%
lp_buffer_x = [0,0,0,0];

lp_coef = [ 2, 1, 0, -1, -2];
%%%%%%%%%%%%
in_buffer = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, ...
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];

%%%%%%%%%%%%
bp_signal = zeros(1,L); 
lp_signal = zeros(1,L); 
sq_signal = zeros(1,L); 
in_signal = zeros(1,L); 

for i = 1:L
    %%%%%%%%%%%%%%%%%%%%
    x_n = EKG1(i);
    y_n = (coef(1) * x_n + coef(2) * bp_buffer_x_0(1) + coef(3) * bp_buffer_x_0(2) ...
				- coef(4) * bp_buffer_y_0(1) - coef(5) * bp_buffer_y_0(2))/1024;
    bp_buffer_y_0(2) = bp_buffer_y_0(1);
    bp_buffer_y_0(1) = y_n;
    bp_buffer_x_0(2) = bp_buffer_x_0(1);
    bp_buffer_x_0(1) = x_n;
    x_n = y_n;

    y_n = (coef(6) * x_n + coef(7) * bp_buffer_x_1(1) + coef(8) * bp_buffer_x_1(2) ...
				- coef(9) * bp_buffer_y_1(1) - coef(10) * bp_buffer_y_1(2))/1024;
    bp_buffer_y_1(2) = bp_buffer_y_1(1);
    bp_buffer_y_1(1) = y_n;
    bp_buffer_x_1(2) = bp_buffer_x_1(1);
    bp_buffer_x_1(1) = x_n;

    x_n = y_n;

    y_n = (coef(11) * x_n + coef(12) * bp_buffer_x_2(1) + coef(13) * bp_buffer_x_2(2) ...
				- coef(14) * bp_buffer_y_2(1) - coef(15) * bp_buffer_y_2(2))/1024;
    bp_buffer_y_2(2) = bp_buffer_y_2(1);
    bp_buffer_y_2(1) = y_n;
    bp_buffer_x_2(2) = bp_buffer_x_2(1);
    bp_buffer_x_2(1) = x_n;

    x_n = y_n;

    y_n = (coef(16) * x_n + coef(17) * bp_buffer_x_3(1) + coef(18) * bp_buffer_x_3(2) ...
				- coef(19) * bp_buffer_y_3(1) - coef(20) * bp_buffer_y_3(2))/1024;
    bp_buffer_y_3(2) = bp_buffer_y_3(1);
    bp_buffer_y_3(1) = y_n;
    bp_buffer_x_3(2) = bp_buffer_x_3(1);
    bp_buffer_x_3(1) = x_n;
    x_n = y_n;

    y_n = (coef(21) * x_n + coef(22) * bp_buffer_x_4(1) + coef(23) * bp_buffer_x_4(2) ...
				- coef(24) * bp_buffer_y_4(1) - coef(25) * bp_buffer_y_4(2))/1024;
    bp_buffer_y_4(2) = bp_buffer_y_4(1);
    bp_buffer_y_4(1) = y_n;
    bp_buffer_x_4(2) = bp_buffer_x_4(1);
    bp_buffer_x_4(1) = x_n;
    x_n = y_n;

    y_n = (coef(26) * x_n + coef(27) * bp_buffer_x_5(1) + coef(28) * bp_buffer_x_5(2) ...
				- coef(29) * bp_buffer_y_5(1) - coef(30) * bp_buffer_y_5(2))/1024;
    bp_buffer_y_5(2) = bp_buffer_y_5(1);
    bp_buffer_y_5(1) = y_n;
    bp_buffer_x_5(2) = bp_buffer_x_5(1);
    bp_buffer_x_5(1) = x_n;
    
    bp_signal(i) = y_n;
    x_n = y_n;
    %%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%
    y_n = (lp_coef(1) * x_n + lp_coef(2) * lp_buffer_x(1) + lp_coef(3) * lp_buffer_x(2) ...
            + lp_coef(4) * lp_buffer_x(3) + lp_coef(5) * lp_buffer_x(4))/8;
    
    lp_buffer_x(4) = lp_buffer_x(3);
    lp_buffer_x(3) = lp_buffer_x(2);
    lp_buffer_x(2) = lp_buffer_x(1);
    lp_buffer_x(1) = y_n;   
    
    lp_signal(i) = y_n;
    x_n = y_n;   
    %%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%
    
    y_n = (x_n * x_n)/256;
    
    sq_signal(i) = y_n;
    x_n = y_n;
    
    %%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%
    
    for j= numel(in_buffer):-1:2
        y_n = y_n + in_buffer(j);
        in_buffer(j) = in_buffer(j-1);
    end
    y_n = y_n + in_buffer(1);
    in_buffer(1) = x_n; 
    
    in_signal(i) = y_n/32;
     x_n = y_n;
    
end

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

% ws = load('ws.mat');
% 
% in_signal = ws.in_signal(:);
% L = size(in_signal,1); 

max_array = zeros(1,10);
max_pos = zeros(1,5);
threshold = 0;

index_1 = 1;
index_2 = 1;

maxerino = 0;
maxerino_pos = 0;
for i = 1:floor(L/fs)
    %%%% Create a windows of at least fs samples
    if i < floor(L/fs)
        window = in_signal(1 + fs*(i-1):(fs*i)); 
    else
        window = in_signal((1 + fs*i):numel(in_signal));
    end
    j = 2;
    
    while (j < numel(window))
        %%%%% Iterates over the windows
        if window(j) > 0.9 * threshold
            %%%% When a sample is higher than threshold
            %%%% stores the index
            %window_indexes(index_1) = j;
            %index_1 = index_1 + 1;
            a = 1;
            while  (window(j) > 0.9 * threshold)&&(j < numel(window))   
                %%%%% Iterates over the windows
                %%%%% while is higher than the TH
                if (maxerino < window(j))&&( window(j) > window(j - 1))
                    %%%% Stores max
                    maxerino = window(j);
                    maxerino_pos = j;
                end
                j = j + 1; 
            end
            %%%%% Stores the end of the sub windows and the 
            %%%%% value and position of the local max.
            %window_indexes(index_1) = (j - 1);
            max_array(index_1) = maxerino;
            if index_1 < 5
                index_1 = index_1 + 1;
            else
                index_1 = 1;
            end
            
            if (maxerino_pos < (numel(window) - 1))&&(maxerino_pos > 0)
                %%% to ensure the max is not recorded
                %%% because the windows ends instead
                %%% of a real max
                max_pos(index_2) = maxerino_pos;
                index_2 = index_2 + 1;
            end
            %index_1 = index_1 + 1;

            maxerino = 0;
            maxerino_pos = 0;
            
        else
            j = j + 1;
        end

    end
    %%%% After went over the window, we calculate 
    %%%% the bpm
    threshold = sum(max_array)/10;
    if index_2 == 1
        %%% no max detected from the previous iter
        threshold = 0.9 * threshold;  
    elseif index_2 == 2
        threshold = 0.9 * threshold; 
        max_pos(1) = max_pos(index_2 - 1) - fs;
    else
        %aux = max_pos(index_3 - 1);
        for j = 1:(index_2 - 2)
            dif = max_pos(j + 1) - max_pos(j);
            %bpm = 60*fs/dif
            if (dif < 600)&&(dif > 70)
               bpm = 60*fs/dif
            end
        end    
        max_pos(1) = max_pos(index_2 - 1) - fs;
        index_2 = 2;
    end

end

