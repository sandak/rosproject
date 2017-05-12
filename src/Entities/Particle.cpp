/*
 * Particle.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 *
 */

#include "Particle.h"

Particle::Particle(int x, int y, int yaw, HamsterAPI::OccupancyGrid map) {

	this->loc = new Location(x, y, yaw);
	this->map = map;
	this->belief = 0.2;

}

Location* Particle::getLoc() {
	return this->loc;
}

double Particle::getBelief() {
	return this->belief;
}

void Particle::update(HamsterAPI::LidarScan lidar, LocationDelta delta) {
	this->loc->updateLocation(delta);
	this->belief =  probScanMatch(lidar);

}

double Particle::probByMove(LocationDelta delta) {
	return 1;
}

double Particle::probScanMatch(HamsterAPI::LidarScan lidar) {
	double hits = 0.0;
	for (int k = 0; k < lidar.getScanSize(); k++) {

		Location projection = this->calcPos(k, lidar.getDistance(k));
		int i = projection.getX() / this->map.getResolution();
		int j = projection.getY() / this->map.getResolution();
		if (map.getCell(i, j) == HamsterAPI::CELL_OCCUPIED) // TODO advanced calc of div
		{
			hits++;
		}
		else if (
				(map.getCell(i+1, j) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i, j+1) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i+1, j+1) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i-1, j-1) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i, j-1) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i-1, j) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i+1, j-1) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i-1, j+1) == HamsterAPI::CELL_OCCUPIED)
		)
		{
			hits = hits +0.6;
		}
		else if (
				(map.getCell(i+2, j) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i, j+2) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i+2, j+2) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i-2, j-2) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i, j-2) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i-2, j) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i+2, j-2) == HamsterAPI::CELL_OCCUPIED)||
				(map.getCell(i-2, j+2) == HamsterAPI::CELL_OCCUPIED)
		)
		{
			hits = hits +0.3;
		}
	}

	return (hits / 360.0);

}

Location Particle::calcPos(int angle, int distance) {
	double norm;
	double angleRad;
	Location projection;
	angleRad = ((this->getLoc()->getYaw() + 180 + angle) % 360) * PI / 180;
	projection.setX(this->getLoc()->getX() + (distance * cos(angleRad)));
	projection.setY(this->getLoc()->getY() + (distance * sin(angleRad)));

	return projection;
}

Particle::~Particle() {
	delete loc;
}

