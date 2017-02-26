/*
 * Particle.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Particle.h"

Particle::Particle(int x, int y, int yaw, HamsterAPI::OccupancyGrid map) {

	this->loc  = new LocationDelta(x,y,yaw);
	this->map = map;
	this->belief = 0;

}

Location* Particle::getLoc(){
	return this->loc;
}

double Particle::getBelief(){
	return this->belief;
}

Particle::~Particle() {
	delete loc;
}

