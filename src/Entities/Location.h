/*
 * Location.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_LOCATION_H_
#define SRC_LOCATION_H_

#include <cmath>
#include "LocationDelta.h"
#include "../properties.h"

class Location {
private:
	int x,y;
	int yaw;
public:

	Location(int x, int y, int yaw);
	Location();

	Location operator+(Location delta);
	int getX();
	int getY();
	int getYaw();
	void setX(int x);
	void setY(int y);
	void setYaw(int yaw);

	void updateLocation(struct LocationDelta delta);

	virtual ~Location();
};

#endif /* SRC_LOCATION_H_ */
