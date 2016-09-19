function bpm_log = bpm_decision_module( signal )
    Fs = 1000;  
    T = 1/Fs;          % Sampling period
    L = max(size(signal));
%     norm = max(signal);
%     norm_signal = 2*signal./norm;
    t = (0:(L-1))*T;   % Time vector
%     threshold = 0.3;
%     threshold_log = zeros(1,L);
%     maxerino = 0;
%     prev_value = 0;
%     maxerino_pos = 0;
%     sample_counter = 0;
%     bpm_log_counter = 1;
% 
%     for i = 1:L
%         ecg_lead = signal(i);
%         if(ecg_lead > threshold)
% 
%             if (ecg_lead > maxerino)
%                 maxerino =         ecg_lead;
%                 maxerino_pos =     sample_counter;
%             end
%         elseif (prev_value > threshold)
%             bpm =              (60 * Fs) / maxerino_pos;
%             threshold =        (threshold + maxerino*3) /4;
%             if( maxerino_pos > 200)
%                 bpm_log(bpm_log_counter) = bpm;
%                 bpm_log_counter = bpm_log_counter + 1;
%                 sample_counter =   sample_counter - maxerino_pos - 1;
%             end
%                 
%                 maxerino =         0;
%         elseif threshold > 0.1
%             threshold = (threshold*511)/512;
%         end
%         threshold_log(i) = threshold;
%         sample_counter = sample_counter+1;
%         prev_value = ecg_lead;
%     end

    % norm = max(threshold_log);
    % p5 = plot(t, threshold_log./norm,'red');
    
    threshold_high = 1;
    threshold_low = 1;
    threshold_high_log = zeros(1,L);
    threshold_low_log = zeros(1,L);
    bpm_detected = zeros(1,L);
    maxerino = 0;
    sample_counter = 0;
    bpm_log_counter = 1;
    flag_qrs_zone = 0;
    for i = 1:L
        ecg_lead = signal(i);
        if (ecg_lead > threshold_high)&&(flag_qrs_zone == 0)
            flag_qrs_zone = 1;
        elseif flag_qrs_zone == 1
            if (ecg_lead <= 0)
                bpm =              (60 * Fs) / sample_counter;
                threshold_high =        (threshold_high + maxerino*3) /4;
                threshold_low =        (threshold_low + maxerino*3) *6/40;
                maxerino = 0;
                flag_qrs_zone = 0;
                if( sample_counter > 200)
                    bpm_log(bpm_log_counter) = bpm;
                    bpm_log_counter = bpm_log_counter + 1;
                    sample_counter =   0;
                    
                end
            end
        elseif threshold_high > threshold_low
            threshold_high = threshold_high *511/512;
        end
        if ecg_lead > maxerino
            maxerino = ecg_lead;
        end
        sample_counter = sample_counter + 1;
        threshold_high_log(i) = threshold_high;
        threshold_low_log(i) = threshold_low;
    end
%     norm = max(threshold_high_log);
%     plot(t, threshold_high_log./norm,'red');
%     plot(t, threshold_low_log./norm,'blue');
%     plot(t, bpm_detected,'yellow');
%     plot(t, threshold_high_log,'red');
%     plot(t, threshold_low_log,'blue');
%     plot(t, bpm_detected,'green');
end

