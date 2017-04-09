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
	float x,y;
	int yaw;
public:

	Location(float x, float y, int yaw);
	Location();

	Location operator+(Location delta);
	float getX();
	float getY();
	int getYaw();
	void setX(float x);
	void setY(float y);
	void setYaw(int yaw);

	void updateLocation(struct LocationDelta delta);

	virtual ~Location();
};

#endif /* SRC_LOCATION_H_ */
