/**
 * @example studies/analytic_wedge/simple_wedge.cc
 */
#include <usml/waveq3d/waveq3d.h>
#include <usml/waveq3d/eigenray_collection.h>

using namespace usml::waveq3d;

/**
 * Compute transmission loss for simple, analytic wedge where the interface
 * reflection coefficients are limited to +/- 1.
 *
 * <pre>
 * profile:     constant 1500 m/s, no attenuation, round earth
 * surface:			no reflection loss
 * bottom: 			2.86 degree slope, north is down, no reflection loss
 * source:			2000 Hz, 4000 meter from apex, halfway down water column
 * receiver:		0-4000 meters across slope, same down range as source
 * </pre>
 *
 * Writes wavefronts to simple_wedge_wavefront.nc and eigenrays to simple_wedge_eigenray.
 */
int main(int argc, char* argv[]) {
    const double slope = to_radians(20.0) ;	  // bottom slope (rad)
	  const double deg2m = 1852.0 * 60.0 ;
	  const double range = 4000.0 / deg2m ;		  // source range along ocean surface (deg)
	  const double depth = 10.0 / deg2m ;		    // source depth (m)
	  const double cross_max = 4000.0 / deg2m ;  // maximum cross range (deg)
	  const double cross_inc = 1000.0 / deg2m ;	// cross range increment (deg)
	  const double time_step = 0.05 ;				    // propagation time step (sec)
	  const double time_max = 4.0 ;		          // maximum propagation time (sec)
    const char* nc_wavefront = USML_STUDIES_DIR "/simple_wedge/simple_wedge_wavefront.nc";
    const char* nc_eigenray = USML_STUDIES_DIR "/simple_wedge/simple_wedge_eigenray.nc";

    // define ocean characteristics

    profile_model* profile = new profile_linear(1500.0);
    profile->attenuation(new attenuation_constant(0.0));
    boundary_model* surface = new boundary_flat();
    boundary_model* bottom = new boundary_slope(
    		wposition1(0.0,0.0), 0.0, -slope, 0.0,
    		new reflect_loss_constant(1.0) );
    ocean_model ocean(surface, bottom, profile);

    // define source characteristics

    seq_linear freq( 2000.0, 1.0, 1 ) ;	// 2000 Hz
    wposition1 source( range, 0.0, -depth ) ;
    seq_rayfan de( -90.0, 90.0, 181 ) ;
    seq_linear az( 0.0, 15.0, 180.0 ) ;

    // define receiver characteristics

    seq_linear cross( cross_inc, cross_inc, cross_max ) ;
    wposition receiver( 1, cross.size(), source.latitude(), source.longitude(), source.altitude() ) ;
    for ( size_t n=0 ; n < cross.size() ; ++n ) {
    	receiver.longitude(0,n,source.longitude()+cross(n)) ;
    }

    // initialize propagation

    eigenray_collection loss(freq, source, de, az, time_step, &receiver);
    wave_queue wave( ocean, freq, source, de, az, time_step, &receiver ) ;
    wave.add_eigenray_listener(&loss) ;

    // propagate wavefronts

    cout << "writing wavefronts to " << nc_wavefront << endl;
    wave.init_netcdf( nc_wavefront );
    wave.save_netcdf();
    while ( wave.time() < time_max ) {
      wave.step() ;
      wave.save_netcdf();
    }
    wave.close_netcdf();

    // compute coherent propagation loss and write eigenrays to disk

    loss.sum_eigenrays();
    cout << "writing eigenrays to " << nc_eigenray << endl;
    loss.write_netcdf(nc_eigenray);

    return 0;
}
