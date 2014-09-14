/*************************************************************************
    > File Name: slice.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 03:04:04 PM EST
 ************************************************************************/

#include "slice.h"

Slice::Slice(): simTime(0.0), viewed(false), channelIndex(-1), viewersNum(0), activeTime(0.0), startTime(0.0), endTime(0.0){
		}
Slice::Slice(double st, bool v, int sn, int vn): simTime(st), viewed(v), channelIndex(sn), viewersNum(vn), activeTime(0.0), startTime(0.0), endTime(0.0){
		}
