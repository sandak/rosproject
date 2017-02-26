/*
 * Locator.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_LOCATOR_H_
#define SRC_LOCATOR_H_
#include "../Entities/Location.h"
#include "../Entities/Particle.h"
#include <vector>
#include <iterator>
#include "HamsterAPIClientCPP/Hamster.h"
#include "../Robot/Robot.h"



using namespace std;

class Locator {
private:
	vector<Particle *> particles;
	HamsterAPI::OccupancyGrid map;
	Robot * robot;
	int startParticlesNum;

public:


	Locator(Robot* robot);
	Location locate();
	void spreadParticles();
	Particle* getMaxBeliefParticle();
	void updatAllParticles(HamsterAPI::LidarScan lidarScan , LocationDelta delta);
	void drawMap();
	virtual ~Locator();
};

#endif /* SRC_LOCATOR_H_ */