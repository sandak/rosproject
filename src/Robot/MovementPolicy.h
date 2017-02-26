/*
 * MovementPolicy.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_MOVEMENTPOLICY_H_
#define SRC_MOVEMENTPOLICY_H_
#include "Robot.h"
#include "../Entities/LocationDelta.h"

class MovementPolicy {
protected:

	HamsterAPI::Hamster *hamster;

public:

	MovementPolicy(HamsterAPI::Hamster * hamster);
	void setHamster(HamsterAPI::Hamster* hamster);

	virtual LocationDelta move() = 0;
	virtual ~MovementPolicy();
};

#endif /* SRC_MOVEMENTPOLICY_H_ */
