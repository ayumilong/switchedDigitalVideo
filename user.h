/*************************************************************************
    > File Name: user.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 03:47:13 PM EST
 ************************************************************************/

#ifndef USER_H
#define USER_H

#include <cstdlib>
#include <vector>
#include "event.h"

enum userState{
	FLIPPING,
	VIEWING
};

class User{
	public:
		User(enum userState state, int curChannelIndex, double viewTime, int channelCount, double shape, int flipBehave, int userId);	
		void initEvents();
		void sequentialFlip(bool);
		void uniformRandomFlip();
		void noFlip();	

		const std::vector<Event*>& getEvents() const {
			return eventList;
		}
		void setState(enum userState us){
			state = us;
		}
		enum userState getState() const{
			return state;
		}
		void setUserId(int ui){
			userId = ui;
		}
		int getUserId(){
			return userId;
		}
		void increaseFailCount(){
			++requestFailCount;
		}
		void setFailCount(int rfc){
			requestFailCount = rfc;
		}
		int getFailCount() const{
			return requestFailCount;
		}
		void setIsBlocking(bool sib){
			blocking = sib;
		}
		bool isBlocking() const{
			return blocking;
		}
		void setViewing(bool v){
			viewing = v;
		}
		bool isViewing() const{
			return viewing;
		}
	private:
		enum userState state;
		int curChannelIndex;
		std::vector<Event*> eventList;
		double viewTime;
		int channelCount;
		double shape;
		int flipBehave;
		int userId;
		int requestFailCount;
		bool blocking;
		bool viewing;

		User();
};

#endif
