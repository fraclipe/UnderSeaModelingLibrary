%% plot_wavefront
% Plot the wavefront generated by WaveQ3D model.  Used as a debugging
% tool during scenario development.
%
clear ; close all
deg2km = ( 1852.0 * 60.0 ) / 1e3 ;

wavefront = load_wavefront( 'simple_wedge_wavefront.nc' ) ;

%% plot vertical slice through wavefront at AZ=90 launch angle

[p,az_index] = min(abs( wavefront.source_az - 90.0 ) ) ;
wlat = squeeze(wavefront.latitude(:,:,az_index)) * deg2km ;
wlng = squeeze(wavefront.longitude(:,:,az_index)) * deg2km ;
walt = squeeze(wavefront.altitude(:,:,az_index))  ;

xtarget  = 1:1:4 ;
ytarget = 4 * ones(size(xtarget)) ;
ztarget = -100.0 * ones(size(xtarget)) ;

figure;
plot3( wlng, wlat, walt, ...
       xtarget, ytarget, ztarget, 'ko' ) ;
xlabel('Cross Slope Range (km)');
ylabel('Down Slope Range (km)');
zlabel('Depth (m)');
view(0,0);

%% plot horizontal slice through wavefront at DE=-10 launch angle

[p,de_index] = min(abs( wavefront.source_de + 10.0 ) ) ;
wlng = squeeze(wavefront.longitude(:,de_index,:)) * deg2km ;
wlat = squeeze(wavefront.latitude(:,de_index,:)) * deg2km ;
walt = squeeze(wavefront.altitude(:,de_index,:)) ;

figure;
plot3( wlng, wlat, walt, ...
       xtarget, ytarget, ztarget, 'ko' ) ;
xlabel('Cross Slope Range (km)');
ylabel('Down Slope Range (km)');
zlabel('Depth (m)');
view(0,90);