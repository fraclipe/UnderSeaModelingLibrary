/**
 * @file wave_queue_reverb.h
 */

#ifndef USML_WAVEQ3D_WAVE_QUEUE_REVERB_H
#define USML_WAVEQ3D_WAVE_QUEUE_REVERB_H

#include <usml/waveq3d/wave_queue.h>
#include <usml/waveq3d/reverberation_model.h>

namespace usml {
namespace waveq3d {
class reflection_model ;
class spreading_model ;

class USML_DECLSPEC wave_queue_reverb : public wave_queue {

    public:

        /**
         * Initialize a propagation scenario.
         *
         * @param  ocean        Reference to the environmental parameters.
         * @param  freq         Frequencies over which to compute propagation (Hz).
         * @param  pos          Location of the wavefront source in spherical
         *                      earth coordinates.
         * @param  de           Initial depression/elevation angles at the
         *                      source location (degrees, positive is up).
         * @param  az           Initial azimuthal angle at the source location
         *                      (degrees, clockwise from true north).
         *                      Ray fans that wrap around all azimuths should
         *                      include rays for both 0 and 360 degrees.
         * @param  time_step    Propagation step size (seconds).
         * @param  pulse        Pulse length of the signal (seconds).
         * @param  num_bins     Number of time bins on the reverberation curce.
         * @param  max_time     Maximum listening time for reverberation (seconds).
         * @param  origin       Type of wave_queue, source/receiver, used in bistatic
         * @param  targets      List of acoustic targets.
         * @param  type         Type of spreading model to use: CLASSIC_RAY
         *                      or HYBRID_GAUSSIAN.
         */
        wave_queue_reverb(
            ocean_model& ocean,
            const seq_vector& freq,
            const wposition1& pos,
            const seq_vector& de, const seq_vector& az,
            double time_step, double pulse,
            unsigned num_bins, double max_time,
            reverberation_model* reverb=NULL,
            const wposition* targets=NULL,
            spreading_type type=HYBRID_GAUSSIAN
            ) ;

        /** Destroy all temporary memory. */
        virtual ~wave_queue_reverb() {}

        /**
         * Used to get the type of spreading model that is being used
         * by the wavefront. This is used exclusively by reverberation
         * models.
         * @return          pointer to the spreading model
         */
        spreading_model* getSpreading_Model() ;

        /**
         * Used to get the type of reverberation model that is being used
         * by the wavefront. This is used exclusively by reverberation
         * models.
         * @return          pointer to the reverberation model
         */
        reverberation_model* getReverberation_Model() ;

        const unsigned getFreqSize() ;

    protected:

        /**
         * Detect and process boundary reflections and caustics.  Loops through all
         * of the "next" wavefront elements to see if any are on the wrong side of
         * a boundary.
         *
         * Relies on detect_reflections_surface() and detect_reflections_bottom()
         * to do the actual work of detecting and processing reflections.
         * These routines work recursively with their opposite so that multiple
         * reflections can take place in a single time step.  This is critical
         * in very shallow water where the reflected position may already be
         * beyond the opposing boundary.
         *
         * At the end of this process, the wave_front::find_edges()
         * routine is used to break the wavefront down into ray families.
         * A ray family is defined by a set of rays that have the same
         * surface, bottom, or caustic count.
         */
        void detect_reflections() ;

        /**
         * Specialized call within wave_queue reverberation calculations. This call
         * searches the volume layers of the ocean for layer collisions and sends the
         * appropriate data to the reverberation model to be used in volume reverberation
         * contributions.
         *
         * The function checks every point along the wavefront if the current altitude
         * and the next time step altitude of the point cross a layer. If a point crosses
         * a boundary in a time step, collide_from_above or collide_from_below are called.
         */
        void detect_volume_reflections() ;

        /**
         * A modified version of the function reflection_model::bottom_reflection used
         * to determine the infromation needed to produce a volume reverberation calculation
         * from this layer when colliding from above the layer.
         */
        void collide_from_above( unsigned de, unsigned az, double depth, unsigned layer ) ;

        /**
         * A modified version of the function reflection_model::surface_reflection used
         * to determine the infromation needed to produce a volume reverberation calculation
         * from this layer when colliding from below the layer.
         *      @todo need to rectify this code, it mimics the bottom_reflection but signs
         *            should be changed to adjust for approaching from below.
         */
        void collide_from_below( unsigned de, unsigned az, double depth, unsigned layer ) ;

        /**
         * Computes a refined location and direction at the point of collision.
         * Uses a second order Taylor series around the current location to
         * estimate these values.
         *
         * @param de            D/E angle index number.
         * @param az            AZ angle index number.
         * @param dtime         The distance (in time) from the "current"
         *                      wavefront to the boundary collision.
         * @param position      Refined position of the reflection (output).
         * @param ndirection    Normalized direction at the point
         *                      of reflection (output).
         * @param speed         Speed of sound at the point of reflection (output).
         */
        void collision_location(
            unsigned de, unsigned az, double dtime,
            wposition1* position, wvector1* ndirection, double* speed ) const ;

};

}   // end of namespace waveq3d
}   // end of namespace usml

#endif
