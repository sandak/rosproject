/*
 * Locator.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Locator.h"

Locator::Locator(Robot* robot) {
	this->robot = robot;

	this->map = robot->getOccupancyGridMap();
}

Location* Locator::locate() {

	cv::namedWindow("locating");
	int numOfIterations = 0;
	Particle* maxParticle;
do{
	if(numOfIterations>0)
		cout << "all particles are DEAD!!! starting over..."<<endl;
	spreadParticles(INIT_PARTICLES_AMOUNT);
	maxParticle = getMaxBeliefParticle();


	while (maxParticle->getBelief() < 0.9 && particles.size() != 0) {

		numOfIterations++;

		LocationDelta delta = robot->moveRobot();
		//std::cout << this->particles.size() << std::endl;
		updatAllParticles(robot->getLidarScan(), delta);
		//std::cout << this->particles.size() << std::endl;
		drawMap();
		if (particles.size()>0)
		{
		maxParticle = this->getMaxBeliefParticle();
		cout<< " MaxParticle "<<maxParticle->getBelief()<<endl;
		}
	}
}while ((particles.size()==0) || maxParticle->getBelief() < 0.9);
		cout<< "returning MaxParticle"<<endl;
		return (maxParticle->getLoc());
}

void Locator::updatAllParticles(HamsterAPI::LidarScan lidarScan,LocationDelta delta) {

	vector<Particle*> * sons = new vector<Particle*>();

	vector<Particle*>::iterator itr = this->particles.begin();
	while (itr != this->particles.end() && particles.size() != 0) {
		try {

			(*itr)->update(lidarScan, delta);


			double bel = (*itr)->getBelief();

			if (bel < 0.1) {
				delete(*itr);
						itr = particles.erase(itr);
			}
			else{
			if (bel >= 0.5 && bel <= 0.7) {
				createSons(*itr, 10, 3, sons);
			}
			if (bel >= 0.7 && bel <= 0.8) {
							createSons(*itr, 10, 2, sons);
						}

			if (bel > 0.8) {

				createSons(*itr, 10, 1, sons);
			}
			itr++;
			}
		} catch (const std::exception& e) {
			cout << e.what() << endl;
		}
	}
	normalizeParticlesSum();
	mergeSonsWithFathers(sons);

	//spreadParticles(INIT_PARTICLES_AMOUNT*0.05);
	std::cout << "num of particles " << particles.size() <<std::endl;
	delete sons;
}

void Locator::normalizeParticlesSum(){
	if (particles.size()>INIT_PARTICLES_AMOUNT*3)
	{
		sortParticles();
		vector<Particle*>::iterator itr = particles.begin();
		int i =0;
		int limit =  particles.size()- (INIT_PARTICLES_AMOUNT*1.2);

		while (itr != particles.end()&&i<limit) {
			delete(*itr);
			itr = particles.erase(itr);
			i++;
		}
	}


}


void Locator::mergeSonsWithFathers(vector<Particle*> * sons) {
	vector<Particle*>::iterator itr = sons->begin();
	while (itr != sons->end()) {
		particles.push_back(*itr);
		itr++;
	}

	while (!sons->empty()) {
		sons->pop_back();
	}
}

void Locator::drawMap() {

	cv::Mat_<cv::Vec3b>* m = new cv::Mat_<cv::Vec3b>(this->map.getHeight(),	this->map.getWidth());
	vector<Particle*>::iterator itr = this->particles.begin();
	u_int i, j;

	for (i = 0; i < this->map.getHeight(); i++) {
		for (j = 0; j < this->map.getWidth(); j++) {
			if (map.getCell(i, j) == HamsterAPI::CELL_OCCUPIED) {
				m->at<cv::Vec3b>(i, j).val[0] = 0;
				m->at<cv::Vec3b>(i, j).val[1] = 0;
				m->at<cv::Vec3b>(i, j).val[2] = 0;
			} else if (map.getCell(i, j) == HamsterAPI::CELL_FREE) {
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
	sortParticles();
	int order = 0;
	while (itr != this->particles.end()) {
		if (order >particles.size()-6)
			drawParticle(m, *itr, 5,red);
		else
			drawParticle(m, *itr, 5,blue);
		order++;
		itr++;
	}

	//drawRobot(m);

	cv::imshow("locating", *m);
	cv::waitKey(1);
}

void Locator::drawParticle(cv::Mat_<cv::Vec3b>* m, Particle * p,int arrowLength, Color drawColor) {
	float x = p->getLoc()->getX();
	float y = p->getLoc()->getY();
	float i = y / map.getResolution();
	float j = x / map.getResolution();
	int yawRad = p->getLoc()->getYaw()*PI/180;
	int mapH = map.getHeight();
	int mapW = map.getWidth();
	float j_end, i_end;
	if (i < mapH && i > 0 && j < mapW && j > 0) {
		cv::Scalar_<double> * color;
		switch (drawColor){
		case red:
		color = new cv::Scalar_<double>(0, 0, 255);
		break;
		case blue:
		color = new cv::Scalar_<double>(255, 0, 0);
		break;
		default:
		color = new cv::Scalar_<double>(255, 0, 0);
		break;

		}
		cv::Point_<int>* start = new cv::Point_<int>((int) (i), (int) (j));
		j_end = j + (arrowLength * std::cos(yawRad));
		i_end = i + (arrowLength * std::sin(yawRad));
		cv::Point_<int>* end = new cv::Point_<int>((int) (i_end), (int) (j_end));
		cv::line(*m, *start, *end, *color, 1, 1, 0);
		cv::circle(*m, *start, 2, *color, 1, 1, 0);
	}

}

void Locator::drawEntity(Entity entity) {
	int x = entity.getX();
	int y = entity.getY();
	float yaw = entity.getYaw();
}

void Locator::drawRobot(cv::Mat_<cv::Vec3b> * m) {
	float robot_x, robot_y, robot_heading;
	float robot_i, robot_j;

	robot_x = robot->getHamster()->getPose().getX() / map.getResolution();
	robot_y = robot->getHamster()->getPose().getY() / map.getResolution();
	robot_heading = robot->getHamster()->getPose().getHeading() + 30.0;

	robot_i = robot_y + (map.getHeight() / 2.0);
	robot_j = robot_x + (map.getWidth() / 2.0);
	cv::Scalar_<int> *color = new cv::Scalar_<int>(255, 0, 0);
	cv::Point_<float>* position = new cv::Point_<float>(robot_i, robot_j);

	cv::circle(*m, *position, 3, *color, 1, 8, 0);
}
void Locator::spreadParticles(int amount) {

	time_t t;
	double x, y;
	double yaw;
	/* Intializes random number generator */
	srand((unsigned) time(&t));
int i,j;
	for (int h = 0; h < amount; h++) {
		do {
			i = rand() % this->map.getHeight();
			j = rand() % this->map.getWidth();
			yaw = rand() % 360;
		} while (map.getCell(j, i) != HamsterAPI::CELL_FREE);
		x = j *  map.getResolution();
		y = i * map.getResolution();
		particles.push_back(new Particle(x , y ,yaw, this->map));

	}

}

void Locator::createSons(Particle *father, int count, int radius,vector<Particle*> * sons) {
	try{
		time_t t;
		double x, y;
		double yaw;
		/* Intializes random number generator */
		srand((unsigned) time(&t));
		int triesCount, i , j;
		bool isPushChild = true;
		for (int h = 0; h < count; h++) {
			triesCount = 1; //going to count how many tries to create child were attempted
			do {
				//TODO there is a particle jumping in from nowhere
				x = (rand() % (radius*100 + 1))/100;
				y = (rand() % (radius*100 + 1))/100;
				int factor1 = rand() % 2;
				int factor2 = rand() % 2;
				if (factor1 == 0)
					x += father->getLoc()->getX();
				else
					x = father->getLoc()->getX() - x;
				if (factor2 == 0)
					y += father->getLoc()->getY();
				else
					y = father->getLoc()->getY() - y;
				yaw = father->getLoc()->getYaw() + (rand() % 90) -45;

				j = x / map.getResolution();
				i = y / map.getResolution();

			} while (map.getCell(j, i) != HamsterAPI::CELL_FREE);

				sons->push_back(new Particle(x ,y , yaw, this->map));
		}
	}
	catch (exception &e) {
		cout << e.what() << endl;;
	}

}

Particle* Locator::getMaxBeliefParticle() {
/*
	vector<Particle*>::iterator itr = this->particles.begin();
	double maxBelief = -1;
	Particle * maxParticle;
	while (itr != this->particles.end()) {

		if ((*itr)->getBelief() > maxBelief) {
			maxParticle = *(itr);
			maxBelief = (*itr)->getBelief();
		}

		itr++;
	}
	return maxParticle;
	*/
	if(particles.size()>0)
		return particles[particles.size()-1];
	else
		return NULL;
}

bool pcomperator (Particle * a,Particle * b) {
	return (a->getBelief()<b->getBelief()); }

void Locator::sortParticles() {
	std::sort(particles.begin(),particles.end(),pcomperator);}



Locator::~Locator() {
// TODO Auto-generated destructor stub
}

