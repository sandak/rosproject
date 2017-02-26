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
	int x,y,yaw;

public:
	Location(int x, int y, int yaw);

	Location operator+(Location delta);
	int getX();
	int getY();
	int getYaw();
	void setX(int x);
	void setY(int y);
	void setYaw(int yaw);


	virtual ~Location();
};

#endif /* SRC_LOCATION_H_ */
