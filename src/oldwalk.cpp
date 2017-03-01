/*
 * oldwalk.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: user
 */




/*
 * HamsterAPIClientSimpleBehaviourExample.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: ofir
 */

#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include <complex>
#include <math.h>
#define PI 3.14159265359;
#define COLLISIONRANGE 0.5;
using namespace std;

float findMinDistance(int minRange, int maxRange);
void autonomousMovement();
int showHamsterLaserMap(HamsterAPI::LidarScan ld);
void showHamsterCam();

HamsterAPI::Hamster * hamster;

void getScansBetween(double min, double max, std::vector<double> & distances) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++) {
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)
			distances.push_back(scan.getDistance(i));
	}
}

bool willCollide(std::vector<double> distances, int angle_from_center) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();
	double collisionThresh = COLLISIONRANGE
	;

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

bool isFrontFree() {
	// Degrees : [90, 270]

	std::vector<double> distances;

	getScansBetween(90, 270, distances);

	return !willCollide(distances, 40);
}

bool isLeftFree() {
	// Degrees : [180,360]

	std::vector<double> distances;

	getScansBetween(180, 360, distances);

	return !willCollide(distances, 40);
}

bool isRightFree() {
	// Degrees : [0, 180]

	std::vector<double> distances;

	getScansBetween(0, 180, distances);

	return !willCollide(distances, 40);
}

bool isBackFree() {
	// Degrees : [270,360], [0, 90]

	std::vector<double> distances;

	getScansBetween(270, 360, distances);
	getScansBetween(0, 90, distances);

	return !willCollide(distances, 40);
}

void moveForward() {

	float minDistance = findMinDistance(170, 190);
	if (minDistance > 5.0) {
		minDistance = 5.0;
	}
	HamsterAPI::Log::i("Client", "Moving Forward");
	hamster->sendSpeed(minDistance / 5.0, 0.0);
}

void turnLeft() {
	float minDistance = findMinDistance(215, 235);
	if (minDistance > 5.0) {
		minDistance = 5.0;
	}

	HamsterAPI::Log::i("Client", "Turning Left");
	while (!isFrontFree())
		hamster->sendSpeed(minDistance / 5.0, 45.0);
}

void turnRight() {
	float minDistance = findMinDistance(125, 145);
	if (minDistance > 5.0) {
		minDistance = 5.0;
	}
	HamsterAPI::Log::i("Client", "Turning Right");
	while (!isFrontFree())
		hamster->sendSpeed(minDistance / 5.0, -45.0);
}

void moveBackwards() {

	float minDistance1 = findMinDistance(0, 10);
	float minDistance2 = findMinDistance(349,359);
	float minDistance;

	if(minDistance1 <= minDistance2)
	{
		minDistance = minDistance1;
	}
	else
	{
		minDistance = minDistance2;
	}

	if (minDistance > 5.0 ) {
		minDistance = 5.0;
	}

	HamsterAPI::Log::i("Client", "Moving Backwards");
	while (!isLeftFree() && !isRightFree() && isBackFree())
		hamster->sendSpeed(-minDistance / 5.0, 0.0);
	if (isLeftFree())
		turnLeft();
	else
		turnRight();
}

void stopMoving() {
	hamster->sendSpeed(0.0, 0.0);

}

/*int Oldmain(int argc, char ** argv) {
	try {
		hamster = new HamsterAPI::Hamster(1);
		while (hamster->isConnected()) {
			try {
				if (isFrontFree())
					moveForward();
				else {
					stopMoving();
					if (isLeftFree())
						turnLeft();
					else if (isRightFree())
						turnRight();
					else if (isBackFree())
						moveBackwards();
					else
						HamsterAPI::Log::i("Client", "I am stuck!");
				}

			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	} catch (exception *e) {
		cout << e->what();
	}
	return 0;
}
*/
int main(int argc, char ** argv) {
	try {

		hamster = new HamsterAPI::Hamster(1);


		while (hamster->isConnected()) {
			try {
				autonomousMovement();
				showHamsterLaserMap(hamster->getLidarScan());
				showHamsterCam();

			} catch (const HamsterAPI::HamsterError & connection_error) {
				HamsterAPI::Log::i("Client", connection_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}

void autonomousMovement() {

	float minDistanceRight,minDistanceLeft;
	HamsterAPI::LidarScan ld = hamster->getLidarScan();

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
float findMinDistance(int minRange, int maxRange) {

	HamsterAPI::LidarScan ld = hamster->getLidarScan();
	float minDistance = 10;

	for (int i = minRange; i < maxRange; i++) {
		if (ld.getDistance(i) < minDistance) {
			minDistance = ld.getDistance(i);
		}
	}
	return minDistance;
}
void showHamsterCam() {

	cv::namedWindow("Hamster_Cam");
	// Get Camera Image and convert it to OpenCV
	HamsterAPI::Image cameraImage = hamster->getCameraImage();
	cv::Mat mat = cameraImage.toCVMat();

	cv::Mat matgrey;
	cv::cvtColor(mat, matgrey, CV_BGR2GRAY);		//convert to grayscale mat
	cv::waitKey(1);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(matgrey, circles, 3, 1, 10, 100, 30, 1, 40);//extracting circles from greyscale mat

	for (size_t i = 0; i < circles.size(); i++) { //running through circles and painting circles on the mat
		cv::Vec3i c = circles[i];
		cv::circle(mat, cv::Point(c[0], c[1]), c[2], cv::Scalar(0, 0, 255), 3,
				8);
		cv::circle(mat, cv::Point(c[0], c[1]), 2, cv::Scalar(0, 255, 0), 3, 8);
	}
	cv::imshow("Hamster_Cam", mat);

}

int showHamsterLaserMap(HamsterAPI::LidarScan ld) {
	cv::Mat_<cv::Vec3b> * m = new cv::Mat_<cv::Vec3b>(500, 500);
	for (int i = 0; i < 500; i++)
		for (int j = 0; j < 500; j++) {
			m->at<cv::Vec3b>(i, j).val[0] = 0;
			m->at<cv::Vec3b>(i, j).val[1] = 0;
			m->at<cv::Vec3b>(i, j).val[2] = 0;
		}

	//paint the robot pixels
	cv::circle(*m, cv::Point(250, 250), 2, cv::Scalar(0, 255, 0), 2, 0);

	cv::namedWindow("Lidar_Map");
	//paint the laser pixels
	double norm;
	double angleDeg, angleRad;
	int x, y;
	for (int i = 0; i < 360; i++) {

		//angleDeg = i*(-1)+270;
		angleDeg = i;
		angleRad = angleDeg * PI
		;
		angleRad = angleRad / 180;

		norm = (ld.getDistance(i)) * (double) 100;
		if (norm <= 250) {

			//std::complex<int> c = std::polar(norm, angle);
			//x = c.real() + 250;
			//y = c.imag() + 250;
			y = (int) (norm * sin(angleRad)) + 250;
			x = (int) (norm * cos(angleRad)) + 250;

			m->at<cv::Vec3b>(x, y).val[2] = 255;
		}

	}

	cv::imshow("Lidar_Map", *m);
	cv::waitKey(1);

	return 0;
}
