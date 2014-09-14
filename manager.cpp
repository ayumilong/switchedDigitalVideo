/*************************************************************************
    > File Name: manager.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 07:17:19 PM EST
 ************************************************************************/

#include <algorithm>
#include <iostream>
#include "manager.h"

Manager::Manager(int un, int cn, int sdvn, int sn, double zs, double st, int fb): data("./log.dat"), activeUserNumber(0), userNumber(un), channelNumber(cn), sdvNumber(sdvn), sliceNumber(sn), zipfShape(zs), simTime(st), flipBehave(fb), peakNumberStr(0), averageNumberStr(0.0), averageBlockingPro(0.0), allUsers(), allEvents(), allChannels(), allSlices(), usedSlices(0){	
	data<<"  Time  "<<"   Slices  "<<"Users"<<"\n";
}

Manager::~Manager(){ //Delete all the heap-based memory that the SDV system used
	for(size_t i = 0; i < allUsers.size(); ++i){
		delete allUsers[i];
	}
	for(size_t i = 0; i < allEvents.size(); ++i){
		delete allEvents[i];
	}
	for(size_t i = 0; i < allChannels.size(); ++i){
		delete allChannels[i];
	}
	for(size_t i = 0; i < allSlices.size(); ++i){
		delete allSlices[i];
	}
}


void Manager::initSDV(){ //Initialize the SDV system
	int pos = 0;
	while(pos < userNumber){
		allUsers.push_back(new User(FLIPPING, 0, simTime, sdvNumber, zipfShape, flipBehave, pos)); //Add users and the same time each user generates its own events
		++pos;
		size_t preSize = allEvents.size();
		size_t addSize = allUsers[allUsers.size() - 1] -> getEvents().size();
		allEvents.resize(preSize + addSize); //Store all the events of each user generated
		std::copy(allUsers[allUsers.size() - 1] -> getEvents().begin(), allUsers[allUsers.size() - 1] -> getEvents().end(), allEvents.begin() + preSize);
	}	
	sort(allEvents.begin(), allEvents.end(), CompareEvent()); //Sort all the events in asending order by start time
	pos = 0;
	while(pos < sdvNumber){ //Add all channels
		++pos;
		allChannels.push_back(new Channel(simTime, false, 0, 0));
	}
	pos = 0;
	while(pos < sliceNumber){ //Add all slices
		++pos;
		allSlices.push_back(new Slice(simTime, false, 0, 0));
	}
}

void Manager::excuteSDV(){
	std::vector<Event*>::iterator pos = allEvents.begin();
	while(pos != allEvents.end()){
		int preChannel = (*pos) -> getPreChannel(); //The SDV channel that a specific user will leave
		if(preChannel != -1 && (((*pos) -> getType()) != MOVE_TO_FLIPPING)){ //If the current event is not the first event of a user, then the channel should be peocess
			allChannels[preChannel] -> decreaseViewersNum(); //One user leaves this SDV channel
			--activeUserNumber;
			//data.setf(std::fstream::showpoint); 
			//data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"    A user leaved slice "<<allChannels[preChannel] -> getSliceIndex() + 1<<"\n";
			allSlices[allChannels[preChannel] -> getSliceIndex()] -> decreaseViewersNum(); //The same with the slice channel
			allChannels[preChannel] -> deleteUser((*pos) -> getUserId());
			if((allChannels[preChannel] -> getViewersNum()) == 0){//There is no user to watch this channel anymore so release the slice that the channel occupied
				--usedSlices; //Release the used slice
				allChannels[preChannel] -> setViewersNum(0);
				allChannels[preChannel] -> setViewed(false);
				allSlices[allChannels[preChannel] -> getSliceIndex()] -> setViewed(false);
				allSlices[allChannels[preChannel] -> getSliceIndex()] -> increaseActiveTime((*pos) -> getStartTime() - (allSlices[allChannels[preChannel] -> getSliceIndex()] -> getStartTime())); //Increase the channel's active time
				data.setf(std::fstream::showpoint); 
				data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"\n";
				//data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"    Free slice "<<allChannels[preChannel] -> getSliceIndex() + 1<<"\n";
				allChannels[preChannel] -> increaseActiveTime(allChannels[preChannel] -> getEndTime() - allChannels[preChannel] -> getStartTime());
				allSlices[allChannels[preChannel] -> getSliceIndex()] -> setStartTime(0.0);
				allSlices[allChannels[preChannel] -> getSliceIndex()] -> setEndTime(0.0);
				allChannels[preChannel] -> setEndTime(0.0);
				allChannels[preChannel] -> setStartTime(0.0); 
			}
		}
		int folChannel = (*pos) -> getFolChannel(); //The SDV channel that the user requests
		if(allChannels[folChannel] -> isViewed()){ //If there are some people who are watching this channel
			if(not allChannels[folChannel] -> hasUser((*pos) -> getUserId())){ //A new user
				++activeUserNumber;
				allChannels[folChannel] -> addUser((*pos) -> getUserId());
				allUsers[(*pos) -> getUserId()] -> setIsBlocking(false); //Increase the specific user's blocking number
				allChannels[folChannel] -> increaseViewersNum(); //Just increase the total viewer number
				allSlices[allChannels[folChannel] -> getSliceIndex()] -> increaseViewersNum();
				//data.setf(std::fstream::showpoint); 
				//data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"    Add a new user to slice "<<allChannels[folChannel] -> getSliceIndex() + 1<<"\n";
			}
			allSlices[allChannels[folChannel] -> getSliceIndex()] -> setEndTime((*pos) -> getEndTime());
			allChannels[folChannel] -> setEndTime((*pos) -> getEndTime());
		}else{ //There is no poeple who are watching this channel, then try to allocate a slice
			if(usedSlices < allSlices.size()){//There are some free slices for use
				++activeUserNumber;
				allChannels[folChannel] -> addUser((*pos) -> getUserId());
				++usedSlices;
				if(usedSlices > peakNumberStr){
					peakNumberStr = usedSlices;
				}
				size_t p = 0;
				while(p < allSlices.size()){
					if(not allSlices[p] -> isViewed()){
						allChannels[folChannel] -> setSliceIndex(p); //Allocate a free slice for the SDV channel
						allSlices[p] -> setChannelIndex(folChannel);
						allSlices[p] -> setViewed(true);
						allSlices[p] -> increaseViewersNum();
						allSlices[p] -> setStartTime((*pos) -> getStartTime()); //Record the start time of the slice in order to record the total active time
						allSlices[p] -> setEndTime((*pos) -> getEndTime());
						data.setf(std::fstream::showpoint); 
						data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"\n";
						//data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"    Allocate slice "<<p + 1<<"\n";
						break;
					}
					++p;
				}
				allChannels[folChannel] -> setViewed(true);
				allChannels[folChannel] -> increaseViewersNum();
				allChannels[folChannel] -> setStartTime((*pos) -> getStartTime()); //Record the start time of the slice in order to record the total active time
				allChannels[folChannel] -> setEndTime((*pos) -> getEndTime());
			}else{//All slices are used, this user should wait for a specific time range 
				//data.setf(std::fstream::showpoint); 
				//data<<std::setw(8)<<(*pos) -> getStartTime()<<"    "<<std::setw(3)<<usedSlices<<"    "<<std::setw(3)<<activeUserNumber<<"    A new user is blocked because all slices are used\n";
				++averageBlockingPro; //Increase total blocking number
				allUsers[(*pos) -> getUserId()] -> setIsBlocking(true); //Increase the specific user's blocking number
				allUsers[(*pos) -> getUserId()] -> increaseFailCount(); //Increase the specific user's blocking number
				/*
				std::vector<Event*>::iterator ptr = pos;
				int userId = (*ptr) -> getUserId();
				++ptr;
				while(ptr != allEvents.end()){
					if((*ptr) -> getUserId() == userId){
						(*ptr) -> setPreChannel(-1);
						break;
					}
					++ptr;
				}*/
				
				
				std::vector<Event*>::iterator ptr = pos;
				int userId = (*ptr) -> getUserId();
				double waitTime = (*ptr) -> getStartTime() + 600; //The current user should wait for 10 minutes
				++ptr;
				while(ptr != allEvents.end()){
					if((*ptr) -> getUserId() == userId && ((*ptr) -> getStartTime()) < waitTime){
						delete *ptr;
						ptr = allEvents.erase(ptr);
					}else if((*ptr) -> getUserId() == userId && ((*ptr) -> getStartTime()) >= waitTime){
						if((*ptr) -> getType() == MOVE_TO_FLIPPING){
							++ptr;
							continue; //The start of the next flipping and viewing should be flipping
						}
						(*ptr) -> setPreChannel(-1); //The start of the next flippping and viewing
						break;
					}else{
						++ptr;
					}
				}
			}
		}
		++pos;
	}
		
	size_t p = 0;
	size_t size = allSlices.size();
	while(p < size){
		if(allSlices[p] -> isViewed()){
			allSlices[p] -> increaseActiveTime(allSlices[p] -> getEndTime() - allSlices[p] -> getStartTime());
		}
		++p;
	}
	
}

void Manager::showSDVResult(){
	size_t pos = 0;
		
	sort(allEvents.begin(), allEvents.end(), CompareEventByUserId()); //Sort all the events in asending order by start time
	while(pos < allEvents.size()){
		std::cout<<"Start time: "<<allEvents[pos] -> getStartTime()<<"---End time: "<<allEvents[pos] -> getEndTime()<<"---User ID: "<<allEvents[pos] -> getUserId()<<"---PreChannel: "<<allEvents[pos] -> getPreChannel()<<"---FolChannel: "<<allEvents[pos] -> getFolChannel()<<"---Event type: "<<allEvents[pos] -> getType()<<std::endl;
		++pos;
	}
	
	pos = 0;
	while(pos < allSlices.size()){ //Compute the total average number stream of the whole SDV system
		//std::cout<<pos + 1<<": "<<allSlices[pos] -> getActiveTime()<<std::endl;
		averageNumberStr += allSlices[pos] -> getActiveTime();
		++pos;
	}
	std::cout<<"Peak number of viewed slices: "<<peakNumberStr<<std::endl;
	std::cout<<"Average number of viewed slices: "<<averageNumberStr/simTime<<std::endl;
	std::cout<<"Average blocking probability: "<<100.0 * averageBlockingPro/allEvents.size()<<"%"<<std::endl;
	std::cout<<"Utilization: "<<100.0 * averageNumberStr/(simTime*sliceNumber)<<"%"<<std::endl;
}
