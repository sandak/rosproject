/*
 * Particle.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Particle.h"

Particle::Particle(int x, int y, int yaw, HamsterAPI::OccupancyGrid map) {

	this->loc = new Location(x,y,yaw);
	this->map = map;
	this->belief = 0.5;

}

Location* Particle::getLoc() {
	return this->loc;
}

double Particle::getBelief() {
	return this->belief;
}

void Particle::update(HamsterAPI::LidarScan lidar, LocationDelta delta) {
	this->loc->updateLocation(delta);
	std::cout <<"particle yaw: "<<this->getLoc()->getYaw()<< std::endl;

	//this->belief = BELIEF_FACTOR*this->getBelief()*probByMove(delta)*probScanMatch(lidar);

}

double Particle::probByMove(LocationDelta delta) {
	return 1;
}

double Particle::probScanMatch(HamsterAPI::LidarScan lidar) {
	int hits = 0;
	for (int i = 0; i < lidar.getScanSize(); i++) {
		//std::cout<<lidar.getDistance(i)<<","<<i<<std::endl;
		Location projection = calcPos(i, lidar.getDistance(i));
		if (map.getCell(projection.getX(), projection.getY())
				== HamsterAPI::CELL_OCCUPIED) // TODO advanced calc of div
				{
			hits++;
			std::cout << "++++++ " << hits << std::endl;
		}

	}
	std::cout << "hits " << hits << std::endl;

	return (hits / 360);

}

Location Particle::calcPos(int angle, int distance) {
	double norm;
	double angleRad;

	Location retVal;

	angleRad = angle*PI / 180;

	norm = (distance) * (double) 100;
	if (norm <= 250) {

		retVal.setY((int) (norm * sin(angleRad)) + 250);
		retVal.setX((int) (norm * cos(angleRad)) + 250);

	}
	return retVal;
}

Particle::~Particle() {
	delete loc;
}

