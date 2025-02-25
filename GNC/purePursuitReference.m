function new_target = purePursuitReference(current_state, current_target, reference_state_map)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
tolerance = 1;
ii = 1;
while ~all(current_target == reference_state_map(:,ii))
    ii = ii+1;
end

if (tolerance >= norm(current_state - current_target)) && (~all(current_target == reference_state_map(:,end)))
    ii = ii+1;
    new_target = reference_state_map(:,ii);
    return;
elseif all(current_target == reference_state_map(:,end))
    new_target = reference_state_map(:, end);
    return;
end