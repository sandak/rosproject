/*
 * Locator.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_LOCATOR_H_
#define SRC_LOCATOR_H_
#include <vector>
#include <iterator>
#include <algorithm>

#include <HamsterAPIClientCPP/Hamster.h>

#include "../Entities/Entity.h"
#include "../Entities/Location.h"
#include "../Entities/Particle.h"
#include "../Robot/Robot.h"
#include "../properties.h"

using namespace std;


class Locator {
private:
	vector<Particle *> particles;
	HamsterAPI::OccupancyGrid map;
	Robot * robot;

enum Color {
	    red,
	    blue,
		green
	};

	void createSons(Particle *father ,int count ,int radius , vector<Particle *> *sons);
	void mergeSonsWithFathers(vector<Particle*>* sons);
	void drawRobot(cv::Mat_<cv::Vec3b>* m);
	void drawParticle(cv::Mat_<cv::Vec3b>* m,Particle * p,int arrowLength, Color color);
	void drawEntity(Entity entity);
	//bool comperator (Particle * a,Particle * b);
	void normalizeParticlesSum();
public:


	Locator(Robot* robot);
	Location* locate();
	void spreadParticles(int amount);
	Particle* getMaxBeliefParticle();
	void updatAllParticles(HamsterAPI::LidarScan lidarScan , LocationDelta delta);
	void drawMap();
	void drawMaxMap();
	virtual ~Locator();
	void sortParticles();


};

#endif /* SRC_LOCATOR_H_ */
