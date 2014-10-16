

particleCount = 64;

x = zeros(particleCount,2);

% Adding A push button
f = figure;
h = uicontrol('Position',[-20 -20 200 40],'String','Continue',...
              'Callback','uiresume(gcbf)');

ballPosition = [150 200];
servoPosition = [100 100];                  
          
for measurement = [1:15]
    
    % Generate New random positions
    for i = [1:particleCount]
        x(i,1) = floor(255*rand());
        x(i,2) = floor(255*rand());
    end
    
    % Wait for button Press
    uiwait(gcf) % Pressing the button will let stuff keep going
    
    % Update the graph
    hold off
    axis([0 255 0 255]);
    plot(x(:,1),x(:,2),'ob');       % Plot the particles
    hold on 
    plot(ballPosition(1), ballPosition(2),'or');         % Plot the objects positon
    plot(servoPosition(1), servoPosition(2), 'og');
   
end

close;

