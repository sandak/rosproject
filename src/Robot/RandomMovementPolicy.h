/*
 * RandomMovementPolicy.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_RANDOMMOVEMENTPOLICY_H_
#define SRC_RANDOMMOVEMENTPOLICY_H_

#include "MovementPolicy.h"
#include "../Entities/LocationDelta.h"
class RandomMovementPolicy: public MovementPolicy {
private:


	void getScansBetween(double min, double max, std::vector<double> & distances);
	bool willCollide(std::vector<double> distances, int angle_from_center);
	bool isFrontFree();
	bool isLeftFree();
	bool isRightFree();
	bool isBackFree();
	struct LastCommand moveForward();
	struct LastCommand turnLeft();
	struct LastCommand turnRight();
	struct LastCommand moveBackwards();
	struct LastCommand stopMoving();
	float findMinDistance(int minRange, int maxRange);


public:

	virtual struct LastCommand move();
	RandomMovementPolicy();

	virtual ~RandomMovementPolicy();
};

#endif /* SRC_RANDOMMOVEMENTPOLICY_H_ */
