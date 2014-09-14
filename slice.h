/*************************************************************************
    > File Name: slice.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 03:04:07 PM EST
 ************************************************************************/

#ifndef SLICE_H
#define SLICE_H

#include <cstdlib>

class Slice{
	public:
		Slice();
		Slice(double simTime, bool viewed, int channelIndex, int viewersNum);

		void increaseActiveTime(double incr){
			activeTime += incr;
		}
		void setActiveTime(double at){
			activeTime = at;
		}
		double getActiveTime() const{
			return activeTime;
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
		void setViewed(bool v){
			viewed = v;
		}
		bool isViewed() const{
			return viewed;
		}
		void setChannelIndex(int sn){
			channelIndex = sn;
		}
		int getChannelIndex() const{
			return channelIndex;
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

	private:
		double simTime;
		bool viewed; //If the current slice channel has been allocated to a SDV channel or not
		int channelIndex; //If it has been allocated, then channel index indicates which SDV channel is has been allocated
		int viewersNum; //The total users that watching this slice
		double activeTime; //The active time of this slice through the whole simulation process
		double startTime;
	    double endTime;	
};

#endif
