%% run_simple_wedge
% Compute analystic solution for transmission loss as a function of range
% across the slope using a simple 3-D wedge. This wedge is similar to
% the ASA benchmark, but the bottom reflection coefficient has been 
% simplified to +1, which allows us to compute an analytic solution.
%
% This implementation also oomputes an analystic solution for an equivalent
% 2-D geometry.  The comparison between the 2-D and 3-D solutions
% illustrates the value of using a 3-D model like WaveQ3D.
%
% Unlike the ASA benchmark, which computes results at 25 Hz, this
% implementation computes results at 2000 Hz for comparison to WaveQ3D. The
% 2000 Hz choice was made to match the conditions of the Pedersen shadow
% zone tests for a strong downwardly refracting n^2 linear environment.
%
% ref: F. B. Jensen and C. M. Ferla, "Numerical solutions of 
% range?dependent benchmark problems in ocean acoustics," 
% J. Acoust. Soc. Am., vol. 87, no. 4, pp. 1499-1510, 1990.
%
% ref: M. A. Pedersen, D. F. Gordon, Normal-Mode and Ray Theory Applied to 
% Underwater Acoustic conditions of Extreme Downward Refraction, 
% J. Acoust. Soc. Amer. 51 (1972) 323.
%
clear ; % close all

%% define scenerio geometry

freq = 2000 ;                       % frequency in Hz (Pedersen)
sound_speed = 1500 ;                % speed of sound in water (ASA value)
wave_number = 2 * pi * freq / sound_speed ;
angle_wedge = 2.86 * pi / 180.0 ;   % wedge angle (ASA value)
range_src = sqrt(4000^2+100^2) ;    % source slant range from apex (ASA value)
angle_src = angle_wedge / 2.0 ;     % source half way to bottom (ASA value)
range_rcv = range_src ;             % receiver angle matches source
cross_rcv = 1000:1000:4000 ;        % distance across slope
angle_rcv = angle_src ;             % receiver angle matches source

%% compute wedge transmission loss

[ pressure, eigenrays ] = simple_wedge( wave_number, angle_wedge, ...
    range_rcv, angle_rcv, cross_rcv, range_src, angle_src ) ;
wedge = 20 * log10( abs(pressure) ) ;
eigenrays(:,1:2) = abs( eigenrays(:,1:2) ) ;
csvwrite('run_simple_wedge.csv',eigenrays) ;

%% compute flat bottom transmission loss with 20logR removed

pressure = simple_flat( wave_number, angle_wedge, ...
    range_rcv, angle_rcv, cross_rcv, range_src, angle_src  ) ;
flat = 20 * log10( abs(pressure) ) ;

%% plot results

figure;
h = plot( cross_rcv/1e3, flat, 'k:', cross_rcv/1e3, wedge, 'k-' ) ;
grid;
xlabel('Range (km)');
ylabel('Transmission Loss (dB)');
title(sprintf('%.0f Hz cross-slope',freq));
set(gca,'YLim',[-100 -40]);
legend('Flat Bottom','2.86^o Wedge','Location','Best')
