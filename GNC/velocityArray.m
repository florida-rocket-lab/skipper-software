clf

v = 0:0.1:5
V = zeros(length(v), 12);
V(:, 5) = v;
Vcell = num2cell(V, 2);

colors = parula(length(v));
hold on
for ii = 1:length(v)
    plot(out(1, ii).logsout{4}.Values, "Color", colors(ii, :))
end
c = colorbar;

c.Label.String = 'Longitudinal Wind Velocity, ft/s';
clim([0, v(end)]);
timeLabel   = xlabel('Time, s');
stateLabel  = ylabel('Lower Gimbal Angle, deg');
set(timeLabel,'Interpreter','LaTeX','FontSize',16);
set(stateLabel,'Interpreter','LaTeX','FontSize',16);
set(gca,'TickLabelInterpreter','LaTeX','FontSize',14);
set(c.Label,'Interpreter','LaTeX', 'FontSize', 16);
set(c,'TickLabelInterpreter','LaTeX', 'FontSize', 14);
grid on
hold off