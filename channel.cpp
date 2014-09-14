/*************************************************************************
    > File Name: channel.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 02:50:50 PM EST
 ************************************************************************/

#include "channel.h"

Channel::Channel(): simTime(0.0), viewed(false), sliceIndex(-1), viewersNum(0), activeTime(0.0), startTime(0.0), endTime(0.0), users() {
		}
Channel::Channel(double st, bool v, int sn, int vn): simTime(st), viewed(v), sliceIndex(sn), viewersNum(vn), activeTime(0.0), startTime(0.0), endTime(0.0), users(){
		}
void Channel::deleteUser(int u){
			std::vector<int>::iterator iter = users.begin();
			while(iter != users.end()){
				if(*iter == u){
					iter = users.erase(iter);
				}else{
					++iter;
				}
			}
		}
bool Channel::hasUser(int u) const{
			std::vector<int>::const_iterator iter = users.begin();
			while(iter != users.end()){
				if(*iter == u){
					return true;
				}
				++iter;
			}
			return false;
		}
