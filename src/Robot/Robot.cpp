/*
 * Robot.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Robot.h"

Robot::Robot(HamsterAPI::Hamster * hamster,MovementPolicy * movementPolicy) {
	this->hamster = hamster;
	this->movementPolicy = movementPolicy;
}


LocationDelta Robot::move(){

	return movementPolicy->move();
}

HamsterAPI::LidarScan Robot::getLidarScan(){

	return this->hamster->getLidarScan();
}

HamsterAPI::OccupancyGrid Robot::getOccupancyGridMap(){

	return this->hamster->getSLAMMap();
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

