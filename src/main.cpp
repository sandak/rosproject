#include <iostream>
#include <HamsterAPIClientCPP/Hamster.h>

#include "Robot/RandomMovementPolicy.h"
#include "Robot/Robot.h"
#include "Tools/Locator.h"

using namespace std;

int main(){

	HamsterAPI::Hamster* hamster;

	try {
			hamster = new HamsterAPI::Hamster(1);
			sleep(1);
			RandomMovementPolicy * policy = new RandomMovementPolicy();
			Robot * robot = new Robot(hamster,policy);
			policy->setRobot(robot);
			Locator * locator = new Locator(robot);

			while (hamster->isConnected()) {
				try {
						locator->locate();

				} catch (const HamsterAPI::HamsterError & message_error) {
					HamsterAPI::Log::i("Client", message_error.what());
				}

			}
		} catch (const HamsterAPI::HamsterError & connection_error) {
			HamsterAPI::Log::i("Client", connection_error.what());
		}
		return 0;

}
