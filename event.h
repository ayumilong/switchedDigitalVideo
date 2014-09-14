/*************************************************************************
    > File Name: event.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 03:19:26 PM EST
 ************************************************************************/
#ifndef EVENT_H
#define EVENT_H

#include <cstdlib>
#include <iostream>

enum eventType{
	MOVE_TO_VIEWING = 23,
	MOVE_TO_FLIPPING = 41,
	FINISH_FLIPPING_VIEW= 56
};

class Event{
	public:
		Event();
		Event(double startTime, double endTIme, int preChannel, int folChannel, int userId, enum eventType type);
		void setStartTime(double st){
			startTime = st;
		}
		double getStartTime() const {
			return startTime;
		}
		void setEndTime(double et){
			endTime = et;
		}
		double getEndTime() const {
			return endTime;
		}
		void setPreChannel(int pc){
			preChannel = pc;
		}
		int getPreChannel() const {
			return preChannel;
		}
		void setFolChannel(int fc){
			folChannel = fc;
		}
		size_t getFolChannel() const {
			return folChannel;
		}
		void setType(enum eventType et){
			type = et;
		}
		enum eventType getType() const {
			return type;
		}
		void setUserId(int ui){
			userId = ui;
		}
		int getUserId() const {
			return userId;
		}
	private:
		double startTime;
		double endTime;
		int preChannel; //Previous channel
		int folChannel; //Following channel
		int userId;
		enum eventType type;
};

#endif
