/**
 * @file sensor_pair.cc
 * Container for one sensor pair instance.
 */
#include <usml/sensors/sensor_pair.h>

using namespace usml::sensors;

/**
 * Notification that new fathometer data is ready.
 *
 * @param	from	Sensor that issued the notification.
 */
void sensor_pair::update_fathometers(sensor::reference& from) {
	if (from != NULL) {
		#ifdef USML_DEBUG
			cout << "sensor_pair::update_fathometers("
				 << from->sensorID() << ")" << endl ;
		#endif

		// TODO
//        if (from == _source) {
//            _src_fathometers = from->fathometers();
//        }
//        if (from == _receiver) {
//            _rcv_fathometers = from->fathometers();
//        }
	}
}

/**
 * Updates the sensors eigenverb_collection
 */
void sensor_pair::update_eigenverbs(sensor::reference& from) {
	if (from != NULL) {
		#ifdef USML_DEBUG
			cout << "sensor_pair::update_eigenverbs("
				 << from->sensorID() << ")" << endl ;
		#endif

// TODO
//        if (from == _source) {
//            _src_eigenverbs = from->eigenverbs();
//        }
//        if (from == _receiver) {
//            _rcv_eigenverbs = from->eigenverbs();
//        }
	}
}

/**
 * Sends message to remove the sensor from the sensor_pair_manager
 */
void sensor_pair::remove_sensor(sensor::reference& from) {
	#ifdef USML_DEBUG
		cout << "sensor_pair::remove_sensor("
			 << from->sensorID() << ")" << endl ;
	#endif
}

/**
 * Queries for the sensor pair complements of this sensor.
 */
sensor::reference sensor_pair::sensor_complement(sensor::reference& from) const {
	if (from.get() != NULL) {
		if (from == _source) {
			return _receiver;
		} else {
			return _source;
		}
	} else {
		return sensor::reference();
	}
}
