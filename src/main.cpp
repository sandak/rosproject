#include <iostream>
#include <HamsterAPIClientCPP/Hamster.h>

#include "Robot/RandomMovementPolicy.h"
#include "Robot/Robot.h"
#include "Tools/Locator.h"
#include <ctime>

using namespace std;

int main() {

	HamsterAPI::Hamster* hamster;

	try {
		hamster = new HamsterAPI::Hamster(1);
		sleep(2);
		RandomMovementPolicy * policy = new RandomMovementPolicy();
		Robot * robot = new Robot(hamster, policy);
		policy->setRobot(robot);
		Locator * locator = new Locator(robot);

		Location* finalLocation = locator->locate();

		while(hamster->isConnected())
		{
			hamster->sendSpeed(1,45);
		}

		if(finalLocation == NULL)
		{
			std::cout << "all particles died" << std::endl;
		}
		else
		{
			std::cout << "robot location is: (" << finalLocation->getX() << "," << finalLocation->getY() << ")" << std::endl;
		}

	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;

}
