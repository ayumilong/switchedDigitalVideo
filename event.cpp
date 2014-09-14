/*************************************************************************
    > File Name: event.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 03:19:30 PM EST
 ************************************************************************/

#include "event.h"

Event::Event(): startTime(0.0), endTime(0.0), preChannel(-1), folChannel(-1), userId(-1), type(FINISH_FLIPPING_VIEW){
}
Event::Event(double st, double et, int pc, int fc, int ui, enum eventType t): startTime(st), endTime(et), preChannel(pc), folChannel(fc), userId(ui), type(t){
}
