/*
 * RandomMovementPolicy.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */
#define COLLISIONRANGE 0.5
#include "RandomMovementPolicy.h"

RandomMovementPolicy::RandomMovementPolicy(HamsterAPI::Hamster * hamster):MovementPolicy(hamster) {
	// TODO Auto-generated constructor stub

}

LocationDelta RandomMovementPolicy::move(){

	LocationDelta * delta;
		if (this->hamster->isConnected()) {
/*
				if (isFrontFree())
					delta = moveForward();
				else {
					stopMoving();
					if (isLeftFree())
						delta = turnLeft();
					else if (isRightFree())
						delta = turnRight();
					else if (isBackFree())
						delta = moveBackwards();
					else
						HamsterAPI::Log::i("Client", "I am stuck!");
				}

*/

				float minDistanceRight,minDistanceLeft;

				if (isFrontFree()) {
					moveForward();
				} else {
					stopMoving();

					minDistanceRight = findMinDistance(125, 145);
					minDistanceLeft = findMinDistance(215, 235);
					if (isLeftFree() || isRightFree()) {
						if (minDistanceRight > minDistanceLeft) {
							turnRight();
						} else {
							turnLeft();
						}
					} else if (isBackFree())
						moveBackwards();
					else
						HamsterAPI::Log::i("Client", "I am stuck!");

				}
			}
		else
		{
			//TODO what if not connected?
		}
		return *delta;
}


RandomMovementPolicy::~RandomMovementPolicy() {
	// TODO Auto-generated destructor stub
}


void RandomMovementPolicy::getScansBetween(double min, double max, std::vector<double> & distances) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++) {
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)
			distances.push_back(scan.getDistance(i));
	}
}

bool RandomMovementPolicy::willCollide(std::vector<double> distances, int angle_from_center) {
	HamsterAPI::LidarScan scan = this->hamster->getLidarScan();

		double collisionThresh = COLLISIONRANGE	;
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
	// Degrees : [90, 270]

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

LocationDelta* RandomMovementPolicy::moveForward() {

	float minDistance = findMinDistance(170, 190);
	if (minDistance > 5.0) {
		minDistance = 5.0;

	}
	HamsterAPI::Log::i("Client", "Moving Forward");
	hamster->sendSpeed(minDistance / 5.0, 0.0);

}

LocationDelta *RandomMovementPolicy::turnLeft() {
	HamsterAPI::Log::i("Client", "Turning Left");
	while (!isFrontFree())
		hamster->sendSpeed(0.04, 45.0);
}

LocationDelta* RandomMovementPolicy::turnRight() {
	HamsterAPI::Log::i("Client", "Turning Right");
	while (!isFrontFree())
		hamster->sendSpeed(0.04, -45.0);
}

LocationDelta *RandomMovementPolicy::moveBackwards() {
	HamsterAPI::Log::i("Client", "Moving Backwards");
	while (!isLeftFree() && !isRightFree() && isBackFree())
		hamster->sendSpeed(-0.4, 0.0);
	if (isLeftFree())
		turnLeft();
	else
		turnRight();
}

void RandomMovementPolicy::stopMoving() {
	hamster->sendSpeed(0.0, 0.0);
}

float RandomMovementPolicy::findMinDistance(int minRange, int maxRange) {

	HamsterAPI::LidarScan ld = hamster->getLidarScan();
	float minDistance = 10;

	for (int i = minRange; i < maxRange; i++) {
		if (ld.getDistance(i) < minDistance) {
			minDistance = ld.getDistance(i);
		}
	}
	return minDistance;
}

