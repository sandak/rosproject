/*
 * Robot.cpp

 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */
#include <iostream>

#include "Robot.h"

using namespace std;

Robot::Robot(HamsterAPI::Hamster * hamster, MovementPolicy * movementPolicy) {
	this->hamster = hamster;
	this->movementPolicy = movementPolicy;

	this->loc.setX(hamster->getPose().getX());
	this->loc.setY(hamster->getPose().getY());
	this->loc.setYaw((hamster->getPose().getHeading()));




	this->lastCommand.time = getTimeMil();
	this->lastCommand.speed = 0;
	this->lastCommand.angle = 0;
}

LocationDelta Robot::moveRobot() {
	struct LastCommand newCommand;
	newCommand = movementPolicy->move();
	LocationDelta retVal = this->updatePose(newCommand);
	this->lastCommand = newCommand;
	return retVal;
}

HamsterAPI::LidarScan Robot::getLidarScan() {

	return this->hamster->getLidarScan();
}

HamsterAPI::OccupancyGrid Robot::getOccupancyGridMap() {

	return this->hamster->getSLAMMap();
}

HamsterAPI::Hamster* Robot::getHamster() {

	return this->hamster;
}

LocationDelta Robot::updatePose(struct LastCommand newCommand) {

	struct LocationDelta delta;

	long t = newCommand.time - this->lastCommand.time;

	delta.distance = newCommand.speed * t / 1000; // dividing by 1000 to convert from meter over second to meter over milli seconds
	//delta.angle = lastCommand.angle;	//TODO add ackermann steering angle
	if(newCommand.angle == 45)
	{
		delta.angle = t*ANGULAR_SPEED/1000; //dividing by 1000 to converto from angle over second to angle over millisecond
	}
	else if(newCommand.angle == -45)
	{
		delta.angle = t*ANGULAR_SPEED/1000*(-1);
	}
	else{
		delta.angle = 0.0;
	}

	this->loc.updateLocation(delta);


	return delta;
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

