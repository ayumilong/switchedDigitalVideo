/*************************************************************************
    > File Name: user.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 03:47:16 PM EST
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include "user.h"
#include "variates.h"

const double MIN_START_TIME = 0.1;
const double MAX_START_TIME = 10.0;
const double FLIPPING_NUMBER_MEAN = 6.0;
const double FLIPPING_VIEWTIME_MEAN = 10.0;
const double VIEWTIME_MEAN = 720.0;

User::User(enum userState us, int cci, double vt, int cc, double sh, int fb, int ui): state(us), curChannelIndex(cci), eventList(), viewTime(vt), channelCount(cc), shape(sh), flipBehave(fb), userId(ui), requestFailCount(0), blocking(false), viewing(false){
	initEvents();
}

void User::initEvents(){
	switch(flipBehave){
		case 1:
			sequentialFlip(true);
			break;
		case 2:
			sequentialFlip(false);
			break;
		case 3:
			uniformRandomFlip();
			break;
		case 4:
			noFlip();
			break;
		default:
			std::cout<<"Wrong behavior!"<<std::endl;
			break;
	}
}


void User::sequentialFlip(bool direction){
	rand_val(random());
	//double startTime = std::max(rand_val(0) * MAX_START_TIME, MIN_START_TIME); //The time that the user starts to watch the vedio
	double startTime = rand_val(0) * MAX_START_TIME + 1;
	double endTime = 0.0; //The end time of the specific event
	bool upOrDown = direction; //True is for up, false is for down
	bool firstFlip = true; //If the current event is the first event of the current flipping state
	while(startTime < viewTime){//The user keep viewing or flipping until time goes to the end of the SDV system
		int flippingNum = std::max(1.0, exponentialRN(1/FLIPPING_NUMBER_MEAN)); //The flipping number of the current flipping state, it shoule be greater or equal to 1
		int preChannel; //The previous is the channel that the user intends to leave
		int  folChannel; //The following channel is the channel that the user requests
		enum eventType type; //The type of the current event
		while(flippingNum > 1 && startTime < viewTime){ //All but the last flipping events
			--flippingNum;
			if(eventList.size() > 0){//If the current event isn't the first event
				preChannel = eventList[eventList.size() - 1] -> getFolChannel(); //The previous channel of the current event is the following channel of the last event 
			}else{//The presious channel of the first event is nothing, indicate as 0
				preChannel = -1;
			}
			if(firstFlip){//If the current event is the first event of the flipping state, then we get the following channel based on zipf distribution
				firstFlip = false;
				folChannel = zipfRN(shape, channelCount) - 1;
			}else{//Or the following channel is the next channel of the last event's following channel
				if(upOrDown){
					folChannel = eventList[eventList.size() - 1] -> getFolChannel() - 1;
					if(folChannel == -1){
						upOrDown = false;
						folChannel = 1; //Get to the top of the channel, then begin to flip down
					}
				}else{
					folChannel = eventList[eventList.size() - 1] -> getFolChannel() + 1;
					if(folChannel == channelCount){
						upOrDown = true;
						folChannel = channelCount - 2; //Get to the bottom of the channel, then begin to flip up
					}
				}
			}
			type = FINISH_FLIPPING_VIEW;
			endTime = startTime + exponentialRN(1/FLIPPING_VIEWTIME_MEAN); //The end time of the current event
			if(endTime >= viewTime){
				endTime = viewTime;
			}
			eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
			startTime = endTime; //The next event's start time is the last event's end time
		}
		if(startTime < viewTime){
			if(eventList.size() > 0){ //The last flipping event
				preChannel = eventList[eventList.size() - 1] -> getFolChannel();
				if(upOrDown){
					folChannel = eventList[eventList.size() - 1] -> getFolChannel() - 1;
					if(folChannel == -1){
						upOrDown = false;
						folChannel = 1; //Get to the top of the channel, then begin to flip down
					}
				}else{
					folChannel = eventList[eventList.size() - 1] -> getFolChannel() + 1;
					if(folChannel == channelCount){
						upOrDown = true;
						folChannel = channelCount - 2; //Get to the bottom of the channel, then begin to flip up
					}
				}
			}else{
				preChannel = -1;
				folChannel = zipfRN(shape, channelCount) - 1;
			}
			type = MOVE_TO_VIEWING; //After the last flipping event, the user will enter view state
			endTime = startTime + exponentialRN(1/FLIPPING_VIEWTIME_MEAN);
			if(endTime >= viewTime){
				endTime = viewTime;
			}
			eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
			startTime = endTime;
			type = MOVE_TO_FLIPPING; //After viewing, the user will enter the next flipping state
			if(startTime < viewTime){
				endTime = startTime + exponentialRN(1/VIEWTIME_MEAN);
				if(endTime > viewTime){
					endTime = viewTime;
				}
				eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
				startTime = endTime;
			}
		}
	}
}

void User::uniformRandomFlip(){
	rand_val(random());
	//double startTime = std::max(rand_val(0) * MAX_START_TIME, MIN_START_TIME); //The time that the user starts to watch the vedio
	double startTime = rand_val(0) * MAX_START_TIME + 1;
	double endTime = 0.0; //The end time of the specific event
	while(startTime < viewTime){//The user keep viewing or flipping until time goes to the end of the SDV system
		int flippingNum = std::max(1.0, exponentialRN(1/FLIPPING_NUMBER_MEAN)); //The flipping number of the current flipping state, it shoule be greater or equal to 1
		size_t preChannel; //The previous is the channel that the user intends to leave
		size_t folChannel; //The following channel is the channel that the user requests
		enum eventType type; //The type of the current event
		bool firstFlip = true; //If the current event is the first event of the current flipping state
		while(flippingNum > 1 && startTime < viewTime){ //All but the last flipping events
			--flippingNum;
			if(eventList.size() > 0){//If the current event isn't the first event
				preChannel = eventList[eventList.size() - 1] -> getFolChannel(); //The previous channel of the current event is the following channel of the last event 
			}else{//The presious channel of the first event is nothing, indicate as 0
				preChannel = -1;
			}
			if(firstFlip){//If the current event is the first event of the current flipping state, then we get the following channel based on zipf distribution
				firstFlip = false;
				folChannel = zipfRN(shape, channelCount) - 1;
				while(folChannel == preChannel){
					folChannel = zipfRN(shape, channelCount) - 1;
				}
			}else{//Or the following channel is the next channel of the last event's following channel
				folChannel = random()%channelCount;
				while(folChannel == preChannel){ //Uniform randome select the next flipping channel
					folChannel = random()%channelCount;
				}	
			}
			type = FINISH_FLIPPING_VIEW;
			endTime = startTime + exponentialRN(1/FLIPPING_VIEWTIME_MEAN); //The end time of the current event
			if(endTime >= viewTime){
				endTime = viewTime;
			}
			eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
			startTime = endTime; //The next event's start time is the last event's end time
		}
		if(startTime < viewTime){
			if(eventList.size() > 0){ //The last flipping event
				preChannel = eventList[eventList.size() - 1] -> getFolChannel();
				folChannel = random()%channelCount;
				while(folChannel == preChannel){
					folChannel = random()%channelCount;
				}	
			}else{
				preChannel = -1;
				folChannel = zipfRN(shape, channelCount) - 1;
			}
			type = MOVE_TO_VIEWING; //After the last flipping event, the user will enter view state
			endTime = startTime + exponentialRN(1/FLIPPING_VIEWTIME_MEAN);
			if(endTime >= viewTime){
				endTime = viewTime;
			}
			eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
			startTime = endTime;
			type = MOVE_TO_FLIPPING; //After viewing, the user will enter the next flipping state
			if(startTime < viewTime){
				endTime = startTime + exponentialRN(1/VIEWTIME_MEAN);
				if(endTime > viewTime){
					endTime = viewTime;
				}
				eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
				startTime = endTime;
			}
		}
	}
}

void User::noFlip(){
	rand_val(random());
	//double startTime = std::max(rand_val(0) * MAX_START_TIME, MIN_START_TIME); //The time that the user starts to watch the vedio
	double startTime = rand_val(0) * MAX_START_TIME + 1;
	double endTime = 0.0; //The end time of the specific event
	bool firstView= true; //If the current event is the first event of the current flipping state
	while(startTime < viewTime){//The user keep viewing or flipping until time goes to the end of the SDV system
		size_t preChannel; //The previous is the channel that the user intends to leave
		size_t folChannel; //The following channel is the channel that the user requests
		enum eventType type; //The type of the current event
		type = MOVE_TO_VIEWING; //After viewing, the user will enter the next flipping state
		if(firstView){
			firstView = false;
			preChannel = -1;
		}else{
			preChannel = eventList[eventList.size() - 1] -> getFolChannel();
		}
		folChannel = zipfRN(shape, channelCount) - 1;
		while(folChannel == preChannel){
			folChannel = zipfRN(shape, channelCount) - 1;
		}
		endTime = startTime + exponentialRN(1/VIEWTIME_MEAN);
		if(endTime > viewTime){
			endTime = viewTime;
		}
		eventList.push_back(new Event(startTime, endTime, preChannel, folChannel, userId, type));
		startTime = endTime;
	}
}
