



particleCount = 64;


x = zeros(particleCount,2);


% Adding A push button
f = figure;
h = uicontrol('Position',[-20 -20 200 40],'String','Continue',...
              'Callback','uiresume(gcbf)');

for measurement = [1:15]
    
    % Generate New random positions
    for i = [1:particleCount]
        x(i,1) = floor(255*rand());
        x(i,2) = floor(255*rand());
    end
    
    % Wait for button Press
    uiwait(gcf) %Pressing the button will let stuff keep going
    
    % Update the graph
    hold off
    axis([0 255 0 255]);
    plot(x(:,1),x(:,2),'ob');       % Plot the particles
    hold on 
    plot(100,100,'xr')              % Plot the objects positon
end

close;

