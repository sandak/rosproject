/*
 * Locator.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Locator.h"

Locator::Locator(Robot* robot) {
	this->robot = robot;
	this->startParticlesNum = 300;
	this->map = robot->getOccupancyGridMap();
}

Location Locator::locate() {

	cv::namedWindow("locating");
	spreadParticles();
	int count = 0;
	Particle* maxParticle = getMaxBeliefParticle();
	while (maxParticle->getBelief() < 100) {
		count++;
		LocationDelta delta = robot->move();

		updatAllParticles(robot->getLidarScan(), delta);
		drawMap();
		maxParticle = this->getMaxBeliefParticle();
	}
	return *(maxParticle->getLoc());
}

void Locator::drawMap() {
	cv::Mat_<cv::Vec3b>* m = new cv::Mat_<cv::Vec3b>(this->map.getHeight(),
			this->map.getWidth());
	vector<Particle*>::iterator itr = this->particles.begin();
	int i, j;
	cout << "draw func" << endl;
	for (i = 0; i < this->map.getHeight(); i++) {
		for (j = 0; j < this->map.getWidth(); j++) {
			if (map.getCell(j, i) == HamsterAPI::CELL_OCCUPIED) {
				m->at<cv::Vec3b>(i, j).val[0] = 0;
				m->at<cv::Vec3b>(i, j).val[1] = 0;
				m->at<cv::Vec3b>(i, j).val[2] = 0;
			} else if (map.getCell(j, i) == HamsterAPI::CELL_FREE) {
				m->at<cv::Vec3b>(i, j).val[0] = 255;
				m->at<cv::Vec3b>(i, j).val[1] = 255;
				m->at<cv::Vec3b>(i, j).val[2] = 255;
			} else {
				m->at<cv::Vec3b>(i, j).val[0] = 127;
				m->at<cv::Vec3b>(i, j).val[1] = 127;
				m->at<cv::Vec3b>(i, j).val[2] = 127;
			}
		}
	}

	while (itr != this->particles.end()) {
		int x = (*itr)->getLoc()->getX();
		int y = (*itr)->getLoc()->getY();

		m->at<cv::Vec3b>(y, x).val[0] = 0;
		m->at<cv::Vec3b>(y, x).val[1] = 0;
		m->at<cv::Vec3b>(y, x).val[2] = 255;

		itr++;
	}

	cv::imshow("locating", *m);
	cv::waitKey(1);
}

void Locator::spreadParticles() {

	time_t t;
	int x,y;
	double yaw;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	for (int i = 0; i < startParticlesNum; i++) {
		do{
			x = rand() % this->map.getWidth();
			y = rand() % this->map.getHeight();
			yaw = rand() % 360;
		}while(map.getCell(x,y) != HamsterAPI::CELL_FREE);

		particles.push_back(new Particle(x, y, yaw, this->map));

	}

}

Particle* Locator::getMaxBeliefParticle() {

}

void Locator::updatAllParticles(HamsterAPI::LidarScan lidarScan,
		LocationDelta delta) {

}

Locator::~Locator() {
	// TODO Auto-generated destructor stub
}

