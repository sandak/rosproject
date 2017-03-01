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

Location Locator::locate(){

	spreadParticles();
	int count = 0;
	Particle* maxParticle = getMaxBeliefParticle();
	while(maxParticle->getBelief() < 100)
	{
		count ++;
		LocationDelta delta = robot->move();
		updatAllParticles(robot->getLidarScan(),delta);
		drawMap();
		maxParticle = this->getMaxBeliefParticle();
	}
	return *(maxParticle->getLoc());
}

void Locator::drawMap(){
	cv::Mat_<cv::Vec3b>* m =new cv::Mat_<cv::Vec3b>(this->map.getHeight(),this->map.getWidth());
	vector<Particle*>::iterator itr  = this->particles.begin();
	uint32_t i,j;

	for (i=0;i<this->map.getHeight();i++)
	{
		for (j=0;j<this->map.getWidth();j++)
		{
			if (map.getCell(j,i) == HamsterAPI::CELL_OCCUPIED)
			{
			m->at<cv::Vec3b>(i,j).val[0]=0;
			m->at<cv::Vec3b>(i,j).val[1]=0;
			m->at<cv::Vec3b>(i,j).val[2]=0;
			} else 	{
			m->at<cv::Vec3b>(i,j).val[0]=255;
			m->at<cv::Vec3b>(i,j).val[1]=255;
			m->at<cv::Vec3b>(i,j).val[2]=255;
			}
		}
	}

	while(itr!= this->particles.end())
	{
		int x = (*itr)->getLoc()->getX();
		int y = (*itr)->getLoc()->getY();

		m->at<cv::Vec3b>(y,x).val[0]=0;
		m->at<cv::Vec3b>(y,x).val[1]=0;
		m->at<cv::Vec3b>(y,x).val[2]=255;

		itr++;
	}

}

void Locator::spreadParticles(){

	  time_t t;

	   /* Intializes random number generator */
	   srand((unsigned) time(&t));


	for(int i = 0 ; i < startParticlesNum;i++){
		int x = rand()%this->map.getWidth();
		int y = rand()%this->map.getHeight();
		double yaw = rand()%360;
		particles.push_back(new Particle(x,y,yaw, this->map));

	}


}

Particle* Locator::getMaxBeliefParticle(){

}

void Locator::updatAllParticles(HamsterAPI::LidarScan lidarScan , LocationDelta delta){

}

Locator::~Locator() {
	// TODO Auto-generated destructor stub
}

