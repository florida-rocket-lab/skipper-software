function intermediate_target = calculatetarget(current, final_target, max_error)

displacement = final_target - current;
direction = displacement / norm(displacement);
direction(isnan(direction)) = 0;

error_mag = calculatemodelerror(current);
intermediate_target = current + direction * max_error / error_mag;

end

