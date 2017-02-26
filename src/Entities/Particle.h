/*
 * Particle.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_PARTICLE_H_
#define SRC_PARTICLE_H_
#include "Location.h"
#include "LocationDelta.h"
#include "HamsterAPIClientCPP/Hamster.h"
class Particle {
private:

	Location* loc;
	double belief;

public:


	HamsterAPI::OccupancyGrid map;


	void update(HamsterAPI::LidarScan l, LocationDelta delta);

	Particle(int x, int y, int yaw, HamsterAPI::OccupancyGrid map);

	Location * getLoc();
	double getBelief();
	virtual ~Particle();
};

#endif /* SRC_PARTICLE_H_ */
