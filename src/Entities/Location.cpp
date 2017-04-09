/*
 * Location.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Location.h"

Location::Location(float x, float y , int yaw) {
	this->x = x;
	this->y = y;
	this->yaw = yaw;
}
Location::Location()
{
	this->x = 0;
	this->y = 0;
	this->yaw = 0;
}

void Location::updateLocation(struct LocationDelta delta){

		int oldYaw = getYaw();
		float oldX = getX();
		float oldY = getY();
		int newYaw = delta.angle;
		float newX = delta.distance*cos((this->getYaw())*PI/180);
		float newY = delta.distance*sin((this->getYaw())*PI/180);
		setYaw((oldYaw + newYaw)%360);
		setX(oldX + newX);
		setY(oldY + newY);

}

Location Location::operator +(Location delta)
{
	this->setX(this->getX()+delta.getX());
	this->setY(this->getY()+delta.getY());
	this->setYaw(delta.getYaw()+this->getYaw());

	return *this;
}

float Location::getX(){
	return this->x;
}

float Location::getY(){
	return this->y;
}

int Location::getYaw(){
	return this->yaw;
}

void Location::setX(float x){
	this->x = x;
}

void Location::setY(float y){
	this->y=y;
}

void Location::setYaw(int yaw){
	this->yaw = yaw;
}

Location::~Location() {
	// TODO Auto-generated destructor stub
}

