/*
 * Location.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_LOCATION_H_
#define SRC_LOCATION_H_

class Location {
private:
	int x,y;
	float yaw;
public:

	Location(int x, int y, float yaw);
	Location();

	Location operator+(Location delta);
	int getX();
	int getY();
	float getYaw();
	void setX(int x);
	void setY(int y);
	void setYaw(float yaw);


	virtual ~Location();
};

#endif /* SRC_LOCATION_H_ */
