/*
 * MovementPolicy.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "MovementPolicy.h"

MovementPolicy::MovementPolicy(HamsterAPI::Hamster * hamster) {
	this->hamster = hamster;
}

void MovementPolicy::setHamster(HamsterAPI::Hamster * hamster){
	this->hamster = hamster;
}

MovementPolicy::~MovementPolicy() {
	// TODO Auto-generated destructor stub
}

