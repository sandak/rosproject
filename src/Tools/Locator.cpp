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
	spreadParticles();
	int numOfIterations = 0;
	Particle* maxParticle = getMaxBeliefParticle();

	while (maxParticle->getBelief() < 100 && particles.size() != 0) {

		numOfIterations++;

		LocationDelta delta = robot->moveRobot();
		std::cout << this->particles.size() << std::endl;
		updatAllParticles(robot->getLidarScan(), delta);
		std::cout << this->particles.size() << std::endl;
		drawMap();

		maxParticle = this->getMaxBeliefParticle();
	}
	if (particles.size() == 0) {
		return NULL;
	} else {
		return (maxParticle->getLoc());
	}
}

void Locator::updatAllParticles(HamsterAPI::LidarScan lidarScan,
		LocationDelta delta) {

	vector<Particle*> * sons = new vector<Particle*>();

	int particleCount = 0;
	vector<Particle*>::iterator itr = this->particles.begin();
	while (itr != this->particles.end() && particles.size() != 0) {
		try {
			std::cout << "updating particle num: " << ++particleCount
					<< std::endl;

			(*itr)->update(lidarScan, delta);

			double bel = (*itr)->getBelief();

			if (bel < 0.3) {

				particles.erase(itr);
			}
			if (bel >= 0.6 && bel <= 0.7) {
				createSons(*itr, 2, 2, sons);//TODO need fix - > we trap in try to create sons loop
			}
			if (bel > 0.7) {

				createSons(*itr, 6, 2, sons);
			}
			itr++;
		} catch (const std::exception& e) {
			cout << e.what() << endl;
		}
	}

	mergeSonsWithFathers(sons);
	delete sons;
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
	//cout << "draw func" << endl;
	cv::Mat_<cv::Vec3b>* m = new cv::Mat_<cv::Vec3b>(this->map.getHeight(),
			this->map.getWidth());
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

	while (itr != this->particles.end()) {
		int x = (*itr)->getLoc()->getX();
		int y = (*itr)->getLoc()->getY();
		int yaw = (*itr)->getLoc()->getYaw();
		int x_end, y_end;
		int mapH = map.getHeight();
		int mapW = map.getWidth();
		if (y < mapH && y > 0 && x < mapW && x > 0) {

			drawParticle(m, *itr, 5);

		}
		itr++;
	}

	drawRobot(m);

	cv::imshow("locating", *m);
	cv::waitKey(1);

	//cout << "end draw func" << endl;
}

void Locator::drawParticle(cv::Mat_<cv::Vec3b>* m, Particle * p,
		int arrowLength) {
	float x = p->getLoc()->getX();
	float y = p->getLoc()->getY();
	float i = y / map.getResolution();
	float j = x / map.getResolution();
	int yaw = p->getLoc()->getYaw();

	float j_end, i_end;

	cv::Scalar_<double> * color = new cv::Scalar_<double>(0, 0, 255);
	cv::Point_<int>* start = new cv::Point_<int>((int) (i), (int) (j));
	j_end = j + (arrowLength * std::cos(yaw));
	i_end = i + (arrowLength * std::sin(yaw));
	cv::Point_<int>* end = new cv::Point_<int>((int) (i_end), (int) (j_end));
	cv::line(*m, *start, *end, *color, 1, 1, 0);
	cv::circle(*m, *start, 2, *color, 1, 1, 0);

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
void Locator::spreadParticles() {

	time_t t;
	int x, y;
	double yaw;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	for (int i = 0; i < INIT_PARTICLES_AMOUNT; i++) {
		do {
			x = rand() % this->map.getWidth();
			y = rand() % this->map.getHeight();
			yaw = rand() % 360;
		} while (map.getCell(x, y) != HamsterAPI::CELL_FREE);

		particles.push_back(
				new Particle(x * map.getResolution(), y * map.getResolution(),
						yaw, this->map));

	}

}

void Locator::createSons(Particle *father, int count, int radius,
		vector<Particle*> * sons) {
	time_t t;
	int x, y;
	double yaw;
	/* Intializes random number generator */
	srand((unsigned) time(&t));
	int triesCount;
	bool isPushChild = true;
	for (int i = 0; i < count; i++) {
		triesCount = 1; //going to count how many tries to create child were attempted
		do {
			//TODO there is a particle jumping in from nowhere
			x = rand() % (radius + 1);
			y = rand() % (radius + 1);
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
			yaw = rand() % 360;

			x = x / map.getResolution();
			y = y / map.getResolution();
			std::cout << "try number: " << triesCount << std::endl;
			std::cout << "map.getCell(" << x << "," << y << ") = "
					<< map.getCell(x, y) << std::endl;
			if (triesCount++ == 100) {
				isPushChild = false;
				break;
			}
		} while (map.getCell(x, y) != HamsterAPI::CELL_FREE);

		if (isPushChild)
			sons->push_back(
					new Particle(x * map.getResolution(),
							y * map.getResolution(), yaw, this->map));

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

