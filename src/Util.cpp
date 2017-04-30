/*
 * Util.cpp
 *
 *  Created on: Apr 9, 2017
 *      Author: user
 */

#include "Util.h"
#include <ctime>
#include <unistd.h>
#include <sys/time.h>

long getTimeMil(){

	struct timeval start;

		long mtime, seconds, useconds;

		gettimeofday(&start, NULL);

		seconds = start.tv_sec;
		useconds = start.tv_usec;

		mtime = ((seconds) * 1000 + useconds / 1000000.0) + 0.5; //chagned by guy for useconds = second / 1000000

		return mtime;

}



