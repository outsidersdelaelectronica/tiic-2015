function bpm_log = bpm_decision_module( signal )
    Fs = 1000;  
    L = max(size(signal));
%     norm = max(signal);
%     norm_signal = 2*signal./norm;

    threshold = 0.3;
    threshold_log = zeros(1,L);
    maxerino = 0;
    prev_value = 0;
    maxerino_pos = 0;
    sample_counter = 0;
    bpm_log_counter = 1;

    for i = 1:L
        ecg_lead = signal(i);
        if(ecg_lead > threshold)

            if (ecg_lead > maxerino)
                maxerino =         ecg_lead;
                maxerino_pos =     sample_counter;
            end
        elseif (prev_value > threshold)
            bpm =              (60 * Fs) / maxerino_pos;
            threshold =        (threshold + maxerino*3) /4;
            if( maxerino_pos > 200)
                bpm_log(bpm_log_counter) = bpm;
                bpm_log_counter = bpm_log_counter + 1;
                sample_counter =   sample_counter - maxerino_pos - 1;
            end
                
                maxerino =         0;
        elseif threshold > 0.1
            threshold = (threshold*511)/512;
        end
        threshold_log(i) = threshold;
        sample_counter = sample_counter+1;
        prev_value = ecg_lead;
    end

    % norm = max(threshold_log);
    % p5 = plot(t, threshold_log./norm,'red');

end

