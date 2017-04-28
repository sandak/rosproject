/*
 * RandomMovementPolicy.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */
#define COLLISIONRANGE 0.5
#include "RandomMovementPolicy.h"

RandomMovementPolicy::RandomMovementPolicy() {
	// TODO Auto-generated constructor stub

}

struct LastCommand RandomMovementPolicy::move() {

	struct LastCommand lastCommand;
	if (this->robot != NULL) {
		if (this->robot->getHamster()->isConnected()) {

			float minDistanceRight, minDistanceLeft;

			if (isFrontFree()) {
				lastCommand = moveForward();
			} else {
				stopMoving();

				minDistanceRight = findMinDistance(125, 145);
				minDistanceLeft = findMinDistance(215, 235);
				if (isLeftFree() || isRightFree()) {
					if (minDistanceRight > minDistanceLeft) {
						lastCommand = turnRight();
					} else {
						lastCommand = turnLeft();
					}
				} else if (isBackFree())
					lastCommand = moveBackwards();
				else
					HamsterAPI::Log::i("Client", "I am stuck!");

			}
		} else {
			//TODO what if not connected?
		}
	}
	return lastCommand;
}

RandomMovementPolicy::~RandomMovementPolicy() {
	// TODO Auto-generated destructor stub
}

void RandomMovementPolicy::getScansBetween(double min, double max,
		std::vector<double> & distances) {
	HamsterAPI::LidarScan scan = this->robot->getHamster()->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++) {
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)
			distances.push_back(scan.getDistance(i));
	}
}

bool RandomMovementPolicy::willCollide(std::vector<double> distances,
		int angle_from_center) {
	HamsterAPI::LidarScan scan = this->robot->getHamster()->getLidarScan();

	double collisionThresh = COLLISIONRANGE;
//TODO handle wide range
	float frontDistance = distances[180];
	size_t i = distances.size() / 2 - angle_from_center / 2;
	for (; i < distances.size() / 2 + angle_from_center / 2; i++) {
		//if (distances[i] < scan.getMaxRange() / 4.0)
		//collisions++;
		if (distances[i] < collisionThresh && frontDistance < 0.4) {
			return true;
		}
	}
	return false;

}

bool RandomMovementPolicy::isFrontFree() {
	// Degrees : [90,time(&(retVal.time)); 270]

	std::vector<double> distances;

	getScansBetween(90, 270, distances);

	return !willCollide(distances, 40);
}

bool RandomMovementPolicy::isLeftFree() {
	// Degrees : [180,360]

	std::vector<double> distances;

	getScansBetween(180, 360, distances);

	return !willCollide(distances, 40);
}

bool RandomMovementPolicy::isRightFree() {
	// Degrees : [0, 180]

	std::vector<double> distances;

	getScansBetween(0, 180, distances);

	return !willCollide(distances, 40);
}

bool RandomMovementPolicy::isBackFree() {
	// Degrees : [270,360], [0, 90]

	std::vector<double> distances;

	getScansBetween(270, 360, distances);
	getScansBetween(0, 90, distances);

	return !willCollide(distances, 40);
}

struct LastCommand RandomMovementPolicy::moveForward() {

	struct LastCommand command;
	float minDistance = findMinDistance(170, 190);
	if (minDistance > 5.0) {
		minDistance = 5.0;

	}
	//HamsterAPI::Log::i("Client", "Moving Forward");
	this->robot->getHamster()->sendSpeed(minDistance / 5.0, 0.0);

	command.speed = minDistance / 5.0;
	command.angle = 0;
	command.time = getTimeMil();

	return command;
}

struct LastCommand RandomMovementPolicy::turnLeft() {
	struct LastCommand command;

	//HamsterAPI::Log::i("Client", "Turning Left");
	//while (!isFrontFree())
		this->robot->getHamster()->sendSpeed(0.04, 45.0);

		command.speed = 0.04;
		command.angle = 45;
		command.time = getTimeMil();

	return command;

}

struct LastCommand RandomMovementPolicy::turnRight() {

	struct LastCommand command;
	//HamsterAPI::Log::i("Client", "Turning Right");
	//while (!isFrontFree())
		this->robot->getHamster()->sendSpeed(0.04, -45.0);

		command.speed = 0.04;
		command.angle = -45.0;
		command.time = getTimeMil();

	return command;
}

struct LastCommand RandomMovementPolicy::moveBackwards() {
	struct LastCommand command;

	//HamsterAPI::Log::i("Client", "Moving Backwards");
	//TODO need fix while with command
	while (!isLeftFree() && !isRightFree() && isBackFree()) {
		this->robot->getHamster()->sendSpeed(-0.4, 0.0);
		command.speed = -0.4;
		command.angle = 0.0;
		command.time = getTimeMil();
	}
	if (isLeftFree())
		command = turnLeft();
	else
		command = turnRight();

	return command;
}

struct LastCommand RandomMovementPolicy::stopMoving() {
	struct LastCommand command;

	this->robot->getHamster()->sendSpeed(0.0, 0.0);

	command.speed = 0.0;
	command.angle = 0.0;
	command.time = getTimeMil();

	return command;
}

float RandomMovementPolicy::findMinDistance(int minRange, int maxRange) {

	HamsterAPI::LidarScan ld = this->robot->getHamster()->getLidarScan();
	float minDistance = 10;

	for (int i = minRange; i < maxRange; i++) {
		if (ld.getDistance(i) < minDistance) {
			minDistance = ld.getDistance(i);
		}
	}
	return minDistance;
}

