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

			std::cout << "robot location is: (" << finalLocation->getX() << "," << finalLocation->getY() << ")" << std::endl;
			while(true)
			{
				sleep(1);
				locator->drawMaxMap();
			}


	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;

}
