close all ; clear all ;
inc = 1 ;
theta = (-90:inc:90)*pi/180.0 ;
theta = theta + pi/2 ;
phi = (0:inc:360)*pi/180.0 ;
t = -pi:pi/36:pi ;
dt = theta(2) - theta(1) ;
dp = phi(2) - phi(1) ;
pitch = (90+21)*pi/180 ;
yaw = (90+57)*pi/180 ;

[theta_grid, phi_grid] = meshgrid(theta,phi) ;

%% Cosine Beam Pattern
pitch = 21 ; 
yaw = 57 ;
roll = 33 ;
% pitch = 0 ; 
% yaw = -20 ;
% roll = 45 ;
R = rotation_matrix(pitch,yaw,roll) ;
vec = R*[0,1,0]' ;
vec = cart_2_sphere(vec) ;
pitch = vec(2) ;
yaw = vec(3) ;
sint = sin( (theta_grid - pitch) / 2 ) ;
sinp = sin( (phi_grid + yaw) / 2 ) ;
level = 1 - 2 * ( sint.*sint ...
    + sin(theta_grid) .* sin(pitch) .* sinp .* sinp ) ;
% level = sin(phi_grid+yaw) .* sin(theta_grid) ;
DI_cosine = abs(level.*level) .* cos(theta_grid) * dt * dp ;
DI_cosine = sum(sum(DI_cosine)) ;
DI_cosine = 10.0*log10( (4*pi) / DI_cosine ) ;
level = 30 + 20*log10(abs(level)) ;
m = find( level < 0 ) ;
level(m) = 0 ;

x = level .* sin(theta_grid) .* cos(phi_grid) ;
y = level .* sin(theta_grid) .* sin(phi_grid) ;
z = level .* cos(theta_grid) ;

figure ;
surf( x, y, z, level, 'edgecolor', 'none', 'facecolor', 'interp' ) ;
colormap(jet) ;
view([0 90]) ;
set(gca,'Xlim',[-30 30]) ;
set(gca,'Ylim',[-30 30]) ;
set(gca,'Zlim',[-30 30]) ;
xlabel('East(+)/West(-)') ;
ylabel('North(+)/South(-)') ;
zlabel('Up') ;
grid on ;
title(sprintf('Cosine Beam Pattern, DI: %f', DI_cosine)) ;
data = xlsread('beam_pattern_cosine.csv');
db = 20.0 * log10( abs(data) ) ;
db = db + 30 ;
m = find( db < 0 ) ;
db(m) = 0 ;

theta = (0:180)*pi/180 ;    % D/E angles where to evaluate
phi = (-180:180)*pi/180 ;   % AZ angles where to evaluate
[theta_grid,phi_grid] = meshgrid(theta,phi) ;

xx = db .* cos(phi_grid) .* sin(theta_grid) ;
yy = db .* sin(phi_grid) .* sin(theta_grid) ;
zz = db .* cos(theta_grid) ;
figure ;
surf( xx, yy, zz, db, 'EdgeColor','none','FaceColor','interp') ;
colormap(jet) ;
view([0 90]) ;
set(gca,'Xlim',[-30 30]) ;
set(gca,'Ylim',[-30 30]) ;
set(gca,'Zlim',[-30 30]) ;
xlabel('East(+)/West(-)') ;
ylabel('North(+)/South(-)') ;
zlabel('Up') ;
title('Cosine C++ Implementation') ;

%% Sine Beam Pattern
pitch = 62 ; 
yaw = 31 ;
roll = 57 ;
% pitch = 45 ; 
% yaw = 15 ;
% roll = 0 ;
R = rotation_matrix(pitch,yaw,roll) ;
vec = R*[1,0,0]' ;
vec = cart_2_sphere(vec) ;
pitch = vec(2) ;
yaw = vec(3) ;
sint = sin( (theta_grid - pitch) / 2 ) ;
sinp = sin( (phi_grid + yaw) / 2 ) ;
level = 1 - 2 * ( sint.*sint ...
    + sin(theta_grid) .* sin(pitch) .* sinp .* sinp ) ;
% level = cos(phi_grid+yaw) .* sin(theta_grid) ;
DI_sine = abs(level) .* cos(theta_grid) * dt * dp ;
DI_sine = sum(sum(DI_sine)) ;
DI_sine = 10.0*log10( (4*pi) / DI_sine ) ;
t = sprintf('DI sine: %f', DI_sine) ;
disp(t) ;
level = 30 + 20*log10(abs(level)) ;
m = find( level < 0 ) ;
level(m) = 0 ;

x = level .* sin(theta_grid) .* cos(phi_grid) ;
y = level .* sin(theta_grid) .* sin(phi_grid) ;
z = level .* cos(theta_grid) ;

figure ;
surf( x, y, z, level, 'edgecolor', 'none', 'facecolor', 'interp' ) ;
colormap(jet) ;
% view([40 20]) ;
view(0,90) ;
set(gca,'Xlim',[-30 30]) ;
set(gca,'Ylim',[-30 30]) ;
set(gca,'Zlim',[-30 30]) ;
xlabel('x') ;
ylabel('y') ;
zlabel('z') ;
grid on ;
title(sprintf('Sine Beam Pattern, DI: %f', DI_sine)) ;

data = xlsread('beam_pattern_sine.csv');
db = 20.0 * log10( abs(data) ) ;
db = db + 30 ;
m = find( db < 0 ) ;
db(m) = 0 ;

theta = (0:180)*pi/180 ;    % D/E angles where to evaluate
phi = (-180:180)*pi/180 ;   % AZ angles where to evaluate
[theta_grid,phi_grid] = meshgrid(theta,phi) ;

xx = db .* cos(phi_grid) .* sin(theta_grid) ;
yy = db .* sin(phi_grid) .* sin(theta_grid) ;
zz = db .* cos(theta_grid) ;
figure ;
surf( xx, yy, zz, db, 'EdgeColor','none','FaceColor','interp') ;
colormap(jet) ;
view([0 90]) ;
set(gca,'Xlim',[-30 30]) ;
set(gca,'Ylim',[-30 30]) ;
set(gca,'Zlim',[-30 30]) ;
xlabel('x') ;
ylabel('y') ;
zlabel('z') ;
title('Sine C++ Implementation') ;
