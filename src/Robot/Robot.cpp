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

	this->loc.setX(hamster->getPose().getX());
	this->loc.setY(hamster->getPose().getY());
	this->loc.setYaw((hamster->getPose().getHeading()));
}


LocationDelta Robot::move(){
	movementPolicy->move();
	return this->updatePose();
}

HamsterAPI::LidarScan Robot::getLidarScan(){

	return this->hamster->getLidarScan();
}

HamsterAPI::OccupancyGrid Robot::getOccupancyGridMap(){

	return this->hamster->getSLAMMap();
}

LocationDelta Robot::updatePose()
{
	LocationDelta retVal(0,0,0);
	retVal.setX(this->loc.getX()-hamster->getPose().getX());
	retVal.setY(this->loc.getY()-hamster->getPose().getY());
	retVal.setYaw(hamster->getPose().getHeading());

			this->loc.setX(hamster->getPose().getX());
			this->loc.setY(hamster->getPose().getY());
			this->loc.setYaw((hamster->getPose().getHeading()));

			return retVal;
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

