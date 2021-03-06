/**
 * @file beam_pattern_sine.cc
 */

#include <usml/sensors/beam_pattern_sine.h>

using namespace usml::sensors ;

/**
 * Constructor
 */
beam_pattern_sine::beam_pattern_sine( double null )
    : _null( std::pow(10.0, -std::abs(null)/10.0) ),
      _gain( 1.0 - _null )
{
    _reference_axis(0) = 1.0 ;
    _reference_axis(1) = 0.0 ;
    _reference_axis(2) = 0.0 ;
    _beamID = beam_pattern_model::SINE ;
    double n2 = _null * _null ;
    double g2 = _gain * _gain ;
    _directivity_index = 10.0*log10( 3.0 / (3.0*n2 + g2) ) ;
}

/**
 * Destructor
 */
beam_pattern_sine::~beam_pattern_sine()
{

}

/** Calculates the beam level in de, az, and frequency **/
void beam_pattern_sine::beam_level(
        double de, double az,
        orientation& orient,
        const vector<double>& frequencies,
        vector<double>* level )
{
	write_lock_guard(_mutex);
    double theta_prime = M_PI_2 - de ;
    double sint = sin( 0.5 * (theta_prime - orient.theta()) + 1e-10 ) ;
    double sinp = sin( 0.5 * (az + orient.phi()) + 1e-10 ) ;
    double dotnorm = 1.0 - 2.0 * ( sint * sint
                     + sin(theta_prime) * sin(orient.theta()) * sinp * sinp ) ;
    double loss = _null + _gain * dotnorm ;
    noalias(*level) =
            scalar_vector<double>( frequencies.size(), loss*loss ) ;
}

/**
 * Computes the directivity index
 */
void beam_pattern_sine::directivity_index(
        const vector<double>& frequencies,
        vector<double>* level )
{
	write_lock_guard(_mutex);
    noalias(*level) = scalar_vector<double>( frequencies.size(), _directivity_index ) ;
}
