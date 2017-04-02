/*
 * Locator.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#include "Locator.h"

Locator::Locator(Robot* robot) {
	this->robot = robot;
	this->startParticlesNum = INIT_PARTICLES_AMOUNT;
	this->map = robot->getOccupancyGridMap();
}

Location Locator::locate() {

	cv::namedWindow("locating");
	spreadParticles();
	int count = 0;
	Particle* maxParticle = getMaxBeliefParticle();

	while (maxParticle->getBelief() < 100) {
		//cout<<maxParticle->getBelief()<<endl;
		count++;
		LocationDelta delta = robot->move();
		//cout << "move end" << endl;
		updatAllParticles(robot->getLidarScan(), delta);
		//cout << "update end" << endl;
		drawMap();
		//cout << "draw end" << endl;
		maxParticle = this->getMaxBeliefParticle();
	}
	return *(maxParticle->getLoc());
}

void Locator::updatAllParticles(HamsterAPI::LidarScan lidarScan,
		LocationDelta delta) {
	//cout << " enter update all" << endl;

	vector<Particle*>::iterator itr = this->particles.begin();
	while (itr != this->particles.end()) {
		try {
			(*itr)->update(lidarScan, delta);

			double bel = (*itr)->getBelief();

			//cout << bel << endl;
			if (bel < 0.3) {
				//cout << "before single delete" << endl;
				particles.erase(itr);
			}
			if (bel > 0.7) {
				//cout << "before single create" << endl;
				createSons(*itr, 30, 3);
			}
			itr++;
		} catch (const std::exception& e) {
			cout << e.what() << endl;
		}

	}
}

void Locator::drawMap() {

	cv::Mat_<cv::Vec3b>* m = new cv::Mat_<cv::Vec3b>(this->map.getHeight(),
			this->map.getWidth());
	vector<Particle*>::iterator itr = this->particles.begin();
	u_int i, j;
	//cout << "draw func" << endl;
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

	while (itr != this->particles.end()) {
		int x = (*itr)->getLoc()->getX();
		int y = (*itr)->getLoc()->getY();
		int yaw = (*itr)->getLoc()->getYaw();
		int x_end,y_end;
		int mapH = map.getHeight();
		int mapW = map.getWidth();
		if (y < mapH && y > 0 && x < mapW && x > 0) {

			cv::Scalar_<double> *color = new cv::Scalar_<double>(0, 0, 255);
			cv::Point_<int>* start = new cv::Point_<int>(x, y);
			x_end = x + (std::cos(yaw));
			y_end = y + (std::sin(yaw));
			cv::Point_<int>* end = new cv::Point_<int>(x_end , y_end);

			cv::circle(*m, *start, 3, *color, 1, 8, 0);
		}
		itr++;
	}

	drawRobot(m);

	cv::imshow("locating", *m);
	cv::waitKey(1);

	//cout << "end draw func" << endl;
}

void Locator::drawEntity(Entity entity) {
	int x = entity.getX();
	int y = entity.getY();
	float yaw = entity.getYaw();
}

void Locator::drawRobot(cv::Mat_<cv::Vec3b> * m) {
	float robot_x, robot_y, robot_heading;
	robot_x = robot->getHamster()->getPose().getX() + ((*m).cols) / 2;
	robot_y = robot->getHamster()->getPose().getY() + ((*m).rows) / 2;
	robot_heading = robot->getHamster()->getPose().getHeading();
	cv::Scalar_<int> *color = new cv::Scalar_<int>(255, 0, 0);
	cv::Point_<float>* position = new cv::Point_<float>(robot_x, robot_y);

	cv::circle(*m, *position, 3, *color, 1, 8, 0);
}
void Locator::spreadParticles() {

	time_t t;
	int x, y;
	double yaw;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	for (int i = 0; i < startParticlesNum; i++) {
		do {
			x = rand() % this->map.getWidth();
			y = rand() % this->map.getHeight();
			yaw = rand() % 360;
		} while (map.getCell(x, y) != HamsterAPI::CELL_FREE);

		particles.push_back(new Particle(x, y, yaw, this->map));

	}

}

void Locator::createSons(Particle *father, int count, int radius) {
	time_t t;
	int x, y;
	double yaw;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	for (int i = 0; i < count; i++) {
		do {
			x = rand() % radius;
			y = rand() % radius;
			int factor1 = rand() % 2;
			int factor2 = rand() % 2;
			if (factor1 == 0)
				x += father->getLoc()->getX();
			else
				x -= father->getLoc()->getX();
			if (factor2 == 0)
				y += father->getLoc()->getY();
			else
				y -= father->getLoc()->getY();
			yaw = rand() % 360;
		} while (map.getCell(x, y) != HamsterAPI::CELL_FREE);

		particles.push_back(new Particle(x, y, yaw, this->map));

	}
}

Particle* Locator::getMaxBeliefParticle() {

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
}

Locator::~Locator() {
	// TODO Auto-generated destructor stub
}

