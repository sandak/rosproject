/*
 * MovementPolicy.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_MOVEMENTPOLICY_H_
#define SRC_MOVEMENTPOLICY_H_
//#include <HamsterAPIClientCPP/Hamster.h>
#include "../Entities/LocationDelta.h"
#include "Robot.h"

class Robot;
class MovementPolicy {
protected:

	Robot * robot;

public:

	MovementPolicy();
	void setRobot(Robot * robot);

	virtual struct LastCommand move() = 0;
	virtual ~MovementPolicy();
};

#endif /* SRC_MOVEMENTPOLICY_H_ */
