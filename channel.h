/*************************************************************************
    > File Name: channel.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 02:50:46 PM EST
 ************************************************************************/

#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdlib>
#include <vector>

class Channel{
	public:
		Channel();
		Channel(double simTime, bool viewed, int sliceIndex, int viewersNum);
		void setViewed(bool v){
			viewed = v;
		}
		bool isViewed() const{
			return viewed;
		}
		void setSliceIndex(int sn){
			sliceIndex = sn;
		}
		int getSliceIndex() const{
			return sliceIndex;
		}
		void increaseViewersNum(){
			++viewersNum;
		}
		void decreaseViewersNum(){
			--viewersNum;
		}
		void setViewersNum(int vn){
			viewersNum = vn;
		}
		int getViewersNum() const{
			return viewersNum;
		}
		void addUser(int u){
			users.push_back(u);
		}
		void deleteUser(int u);
		bool hasUser(int u) const;

		double getActiveTime() const{
			return activeTime;
		}
		void setActiveTime(double at){
			activeTime = at;
		}
		void increaseActiveTime(double incr){
			activeTime += incr;
		}
		double getStartTime() const{
			return startTime;
		}
		void setStartTime(double st){
			startTime = st;
		}
		double getEndTime() const{
			return endTime;
		}
		void setEndTime(double et){
			if(endTime < et){
				endTime = et;
			}	
		}
		int getUserNumber() const{
			return users.size();
		}

	private:
		double simTime;
		bool viewed; //If this channel is viewed or not
		int sliceIndex; //If it is viewing by some people, slice index indicates which slice channel it occupies
		int viewersNum; //The total number of users that view this channel
		double activeTime;
		double startTime;
		double endTime;
		std::vector<int> users;
};

#endif
