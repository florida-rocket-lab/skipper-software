function now_pos_target = purePursuitMap(initial_state, reference_state)
    % PURE_PURSUIT Generates a reference trajectory for a VTVL rocket hopper
    % Inputs:
    %   initial_state - 12x1 vector [x, y, z, vx, vy, vz, ax, ay, az, roll, pitch, yaw]
    %   target_state  - 12x1 vector [x, y, z, vx, vy, vz, ax, ay, az, roll, pitch, yaw]
    % Output:
    %   ref_trajectory - Nx12 matrix containing the reference trajectory
    
    % Extract initial and target positions
    initial_position = initial_state(1:3);
    target_position = reference_state(1:3);
    max_dif = ceil(abs(max((target_position-initial_position))));

    target_step_div = max_dif;
    if target_step_div == 0
        now_pos_target = cat(1, target_position, reference_state(4:12));
        return;
    end
    target_step = target_position/target_step_div;
    current_target = initial_position+target_step;
    now_pos_target = current_target;
    for ii = 1:1:max_dif
        if current_target + target_step >= target_position
            current_target = target_position - target_step;
            now_pos_target = cat(2, now_pos_target, current_target+target_step);
            break;
        end
        now_pos_target = cat(2, now_pos_target, current_target+target_step);
        current_target = current_target + target_step;
    end
    return;
end