/*
 * Robot.h
 *
 *  Created on: Feb 26, 2017
 *      Author: user
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_
#include <HamsterAPIClientCPP/Hamster.h>
#include <ctime>
#include "../Entities/Entity.h"
#include "MovementPolicy.h"
#include "../Entities/LocationDelta.h"
#include "LastCommand.h"


class MovementPolicy;
class Robot: public Entity{
private:
	Location loc;
	HamsterAPI::Hamster * hamster;
	MovementPolicy * movementPolicy;
	//last command parameters
	struct LastCommand lastCommand;

public:
	LocationDelta updatePose(struct LastCommand newCommand);
	Robot(HamsterAPI::Hamster * hamster,MovementPolicy * movementPolicy);
	LocationDelta move();

	HamsterAPI::LidarScan getLidarScan();
	HamsterAPI::OccupancyGrid getOccupancyGridMap();
	HamsterAPI::Hamster * getHamster();
	virtual ~Robot();
};

#endif /* SRC_ROBOT_H_ */
