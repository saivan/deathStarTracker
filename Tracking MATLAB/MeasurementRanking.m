
% Adding A push button
f = figure;
h = uicontrol('Position',[-20 -20 200 40],'String','Continue',...
              'Callback','uiresume(gcbf)');

% Dealing with the ball and the servos position
azim = 1;
elev = 2;
ballPosition = [150 200];   % Azim, Elev position of the ball
servoPosition = [100 100];  % Azim, Elev position of the servo
         
% Initialising the particles
particleCount = 64;
x = zeros(particleCount,2);

% Generate random starting positions for the particles
for i = [1:particleCount]
    x(i,1) = floor(255*rand());
    x(i,2) = floor(255*rand());
end

for measurement = [1:15]    
    
    % Generate a measurement based on particles position
    
    % Rank each particle [0-7] based on its agreement with the measurement
    
    % Generate new particles
    
    % Calculate the expected position
    
    
    
    
    
    
    
    % Wait for button Press
    uiwait(gcf) % Pressing the button will let stuff keep going
    
    % Update the graph
    hold off
    axis([0 255 0 255]);
    plot(x(:,1),x(:,2),'ob');       % Plot the particles
    hold on 
    plot(ballPosition(azim),ballPosition(elev),'or')          % Plot the balls Position
    plot(servoPosition(azim),servoPosition(elev),'og')        % Plot the servos Position
    % Plot the objects position
    
end

close;
