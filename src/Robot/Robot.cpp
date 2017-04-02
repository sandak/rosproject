/*
 * Robot.cpp

 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */
#include <iostream>
#include "Robot.h"

using namespace std;


Robot::Robot(HamsterAPI::Hamster * hamster,MovementPolicy * movementPolicy) {
	this->hamster = hamster;
	this->movementPolicy = movementPolicy;

	this->loc.setX(hamster->getPose().getX());
	this->loc.setY(hamster->getPose().getY());
	this->loc.setYaw((hamster->getPose().getHeading()));

	time(&this->lastCommand.time);
	this->lastCommand.speed = 0;
	this->lastCommand.angle = 0;
}


LocationDelta Robot::move(){
	struct LastCommand newCommand;
	newCommand = movementPolicy->move();
	return this->updatePose(newCommand);
}

HamsterAPI::LidarScan Robot::getLidarScan(){

	return this->hamster->getLidarScan();
}

HamsterAPI::OccupancyGrid Robot::getOccupancyGridMap(){

	return this->hamster->getSLAMMap();
}

HamsterAPI::Hamster* Robot::getHamster(){

	return this->hamster;
}

LocationDelta Robot::updatePose(struct LastCommand newCommand)
{

	struct LocationDelta delta;
	//todo fix time calc
	float t = newCommand.time - this->lastCommand.time;

	delta.distance = newCommand.speed*t;
	delta.angle = 90-lastCommand.angle;

	this->loc.updateLocation(delta);

	return delta;
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

