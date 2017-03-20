/*
 * Robot.cpp

 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */
#include <iostream>
#include "Robot.h"
#define PI 3.14159265359
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

	LocationDelta delta(0,0,0);
	//todo fix time calc
	float t = newCommand.time - this->lastCommand.time;
	float distance = newCommand.speed*t;
	delta.setX(distance*cos((90-lastCommand.angle)*PI/180));
	delta.setY(distance*sin((90-lastCommand.angle)*PI/180));
	delta.setYaw(lastCommand.angle);

	this->loc = this->loc + delta;

	return delta;
}

Robot::~Robot() {
	// TODO Auto-generated destructor stub
}

