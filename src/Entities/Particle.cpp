/*
 * Particle.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Particle.h"

Particle::Particle(int x, int y, int yaw, HamsterAPI::OccupancyGrid map) {

	this->loc = new Location(x, y, yaw);
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
	std::cout << "particle update func: " << std::endl;
	std::cout << "delta angle: " << delta.angle << ", delta distance: "
			<< delta.distance << std::endl;
	std::cout << "before x: " << this->getLoc()->getX() << ", before y: "
			<< this->getLoc()->getY() << std::endl;
	std::cout << "before yaw: " << this->getLoc()->getYaw() << std::endl;
	std::cout << "updating location...." << std::endl;
	this->loc->updateLocation(delta);
	std::cout << "after x: " << this->getLoc()->getX() << ", after y: "
			<< this->getLoc()->getY() << std::endl;
	std::cout << "after yaw: " << this->getLoc()->getYaw() << std::endl;

	//this->belief = BELIEF_FACTOR * this->getBelief() * probByMove(delta)
			//* probScanMatch(lidar);

}

double Particle::probByMove(LocationDelta delta) {
	return 1;
}

double Particle::probScanMatch(HamsterAPI::LidarScan lidar) {
	double hits = 0.0;
	for (int i = 0; i < lidar.getScanSize(); i++) {

		Location projection = this->calcPos(i, lidar.getDistance(i));
		std::cout << lidar.getDistance(i) << "," << i << std::endl;
		int x = projection.getX() / this->map.getResolution();
		int y = projection.getY() / this->map.getResolution();
		std::cout << "Obstacle x,y :  " << x << "," << y << std::endl;
		if (map.getCell(x, y) == HamsterAPI::CELL_OCCUPIED) // TODO advanced calc of div
		{
			hits++;
			std::cout << "++++++ " << hits << std::endl;
		}
		std::cout << "step" << std::endl;
	}
	std::cout << "hits " << hits << std::endl;

	return (hits / 360.0);

}

Location Particle::calcPos(int angle, int distance) {
	double norm;
	double angleRad;

	Location projection;
	std::cout << "particle position : " << this->getLoc()->getX() << ","
			<< this->getLoc()->getY() << "," << this->getLoc()->getYaw()
			<< std::endl;

	angleRad = ((this->getLoc()->getYaw() + 180 + angle) % 360) * PI / 180;

	projection.setX(this->getLoc()->getX() + distance * cos(angleRad));
	projection.setY(this->getLoc()->getY() + distance * sin(angleRad));

	std::cout << "projection position:  " << angleRad << ","
			<< projection.getX() << "," << projection.getY() << std::endl;

//	norm = (distance) * (double) 100;
//	if (norm <= 250) {
//
//		retVal.setY((int) (norm * sin(angleRad)) + 250);
//		retVal.setX((int) (norm * cos(angleRad)) + 250);
//
//	}
	return projection;
}

Particle::~Particle() {
	delete loc;
}

