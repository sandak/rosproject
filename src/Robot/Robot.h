/*
 * Robot.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_
#include <HamsterAPIClientCPP/Hamster.h>
#include "MovementPolicy.h"
#include "../Entities/LocationDelta.h"

class Robot {
private:
	Location loc;
	HamsterAPI::Hamster * hamster;
	MovementPolicy * movementPolicy;

public:
	LocationDelta updatePose();
	Robot(HamsterAPI::Hamster * hamster,MovementPolicy * movementPolicy);
	LocationDelta move();

	HamsterAPI::LidarScan getLidarScan();
	HamsterAPI::OccupancyGrid getOccupancyGridMap();

	virtual ~Robot();
};

#endif /* SRC_ROBOT_H_ */
