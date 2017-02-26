/*
 * LocationDelta.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_LOCATIONDELTA_H_
#define SRC_LOCATIONDELTA_H_

#include "Location.h"

class LocationDelta: public Location {

public:
	LocationDelta(int x, int y, int yaw);
	virtual ~LocationDelta();
};

#endif /* SRC_LOCATIONDELTA_H_ */
