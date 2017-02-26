/*
 * Location.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Location.h"

Location::Location(int x, int y , int yaw) {
	this->x = x;
	this->y = y;
	this->yaw = yaw;
}

Location Location::operator +(Location delta)
{
	this->setX(this->getX()+delta.getX());
	this->setY(this->getY()+delta.getY());
	this->setYaw(delta.getYaw());
	return *this;
}

int Location::getX(){
	return this->x;
}
int Location::getY(){
	return this->y;
}
int Location::getYaw(){
	return this->yaw;
}

void Location::setX(int x){
	this->x = x;
}

void Location::setY(int y){
	this->y=y;
}
void Location::setYaw(int yaw){
	this->yaw = yaw;
}

Location::~Location() {
	// TODO Auto-generated destructor stub
}
