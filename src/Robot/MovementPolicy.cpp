/*
 * MovementPolicy.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "MovementPolicy.h"

MovementPolicy::MovementPolicy() {
	this->robot = NULL;
}


void MovementPolicy::setRobot(Robot* robot){
	this->robot = robot;
}

MovementPolicy::~MovementPolicy() {
	// TODO Auto-generated destructor stub
}

