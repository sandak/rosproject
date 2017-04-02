/*
 * Particle.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_PARTICLE_H_
#define SRC_PARTICLE_H_
#include <iostream>

#include "Location.h"
#include <HamsterAPIClientCPP/Hamster.h>
#include "LocationDelta.h"
#include "Entity.h"
#include "../properties.h"


class Particle: public Entity {
private:

	Location* loc;
	double belief;
	double probByMove(LocationDelta delta);
	double probScanMatch(HamsterAPI::LidarScan lidar);
public:
	Location calcPos(int angle, int distance);


	HamsterAPI::OccupancyGrid map;


	void update(HamsterAPI::LidarScan l, LocationDelta delta);



	Particle(int x, int y, int yaw, HamsterAPI::OccupancyGrid map);

	Location * getLoc();
	double getBelief();
	virtual ~Particle();
};

#endif /* SRC_PARTICLE_H_ */
