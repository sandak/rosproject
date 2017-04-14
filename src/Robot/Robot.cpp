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
	float before_move_real_x = this->getHamster()->getPose().getX();
	float before_move_real_y = this->getHamster()->getPose().getY();
	float before_move_real_yaw = this->getHamster()->getPose().getHeading();

	newCommand = movementPolicy->move();

	struct LocationDelta retVal;
	if(IS_GETPOSE_ENABLED == false){
		 retVal = this->updatePose(newCommand);
	}
	else
	{
		float after_move_real_x = this->getHamster()->getPose().getX();
		float after_move_real_y = this->getHamster()->getPose().getY();
		float after_move_real_yaw = this->getHamster()->getPose().getHeading();

		retVal.angle = after_move_real_yaw - before_move_real_yaw;
		retVal.distance  = sqrt(pow((before_move_real_x - after_move_real_x),2)+
								pow((before_move_real_y - after_move_real_y),2));
	}

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

	long t = newCommand.time - this->lastCommand.time; //milliseconds

	delta.distance = newCommand.speed * t / 1000.0; // dividing by 1000 to convert from meter over second to meter over milli seconds
	//delta.angle = lastCommand.angle;
	if(newCommand.angle == 45)
	{
		delta.angle = t*ANGULAR_SPEED/1000.0; //dividing by 1000 to convert from angle over second to angle over millisecond
	}
	else if(newCommand.angle == -45)
	{
		delta.angle = t*ANGULAR_SPEED/1000.0*(-1.0);
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

