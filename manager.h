/*************************************************************************
    > File Name: manager.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 07:17:16 PM EST
 ************************************************************************/
#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <sys/time.h>
#include "event.h"
#include "user.h"
#include "channel.h"
#include "slice.h"

class Manager{
	public:
		Manager(int userNumber, int channelNumber, int sdvNumber, int sliceNumber, double zipfShape, double simTime, int flipBehave);
		~Manager();
		void initSDV();
		void excuteSDV();
		void showSDVResult();
	private:
		std::ofstream data;	
		int activeUserNumber; //Total number of viewing users

		int userNumber; //Total number of users
		int channelNumber; //Total number of content channels
		int sdvNumber; //Channel number that SDV manageing
		int sliceNumber; //Slice channel number
		double zipfShape; //zipf's law's shape factor
		double simTime; //Simulation total time
		int flipBehave; //Flipping behavior, totally have 4 kind of flipping behaviors

		size_t peakNumberStr; //The peak number of slices that being viewed
		double averageNumberStr; //Average number streams
		double averageBlockingPro; //Average blocking probability
		std::vector<User*> allUsers; //All the users
		std::vector<Event*> allEvents; //All the events
		std::vector<Channel*> allChannels; //All the channels
		std::vector<Slice*> allSlices; //All the slices
		size_t usedSlices; //The number of used slices

		Manager();
};

class CompareEvent{ //Function object for sorting all the events by start time
	public:
		bool operator()(const Event* e1, const Event* e2) const { //Sort by asending order
			return e1 -> getStartTime() < e2 -> getStartTime();
		}
};

class CompareEventByUserId{
	public:
		bool operator()(const Event* e1, const Event* e2) const { //Sort by asending order
			return (e1 -> getStartTime()) < (e2 -> getStartTime());
		}
};
#endif
