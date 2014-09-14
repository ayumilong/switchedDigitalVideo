/*************************************************************************
    > File Name: main.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Sun 17 Nov 2013 12:09:09 PM EST
 ************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "manager.h"

void DieWithUserMessage(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(0);
}

const int PARAMETER_NUMBER = 8;
const int MAX_USER_NUMBER = 1024*1024;
const int MAX_CHANNEL_NUMBER = 1024*1024;
const double MIN_ZIPF_SHAPE = 0.5;
const double MAX_ZIPF_SHAPE = 2.0;
const double MIN_SIM_TIME = 120.0;
const double MAX_SIM_TIME = 36000;
const int MIN_FLIP_BEHAVIOR = 1;
const int MAX_FLIP_BEHAVIOR = 4;

int main(int argc, char** argv){
	if(argc < PARAMETER_NUMBER){
		DieWithUserMessage("Useage", "SDVSystem <User Number> <Channel Number> <SDV Channel Number> <Slice Channel Number> <Zipf Shape> <Simulation time> <Flipping behaviors>"); 
	}
	int userNumber = atoi(argv[1]);
	if(userNumber <= 0 || userNumber > MAX_USER_NUMBER){
		DieWithUserMessage("Error", "Invalid user number, please input again! (0, 1024*1024]");
	}
	int channelNumber = atoi(argv[2]);
	if(channelNumber <= 0 || channelNumber > MAX_CHANNEL_NUMBER){
		DieWithUserMessage("Error", "Invalid channel number, please input again! (0, 1024*1024]");
	}
	int SDVNumber = atoi(argv[3]);
	if(SDVNumber <= 0 || SDVNumber > channelNumber){
		DieWithUserMessage("Error", "Invalid SDV channel number, please input again! (0, channelNumber]");
	}
	int sliceNumber = atoi(argv[4]);
	if(sliceNumber <= 0 || sliceNumber > SDVNumber){
		DieWithUserMessage("Error", "Invalid slice channel number, please input again! (0, SDVNumber]");
	}
	double zipfShape = atof(argv[5]);
	if(zipfShape < MIN_ZIPF_SHAPE || zipfShape > MAX_ZIPF_SHAPE){
		DieWithUserMessage("Error", "Invalid zipf shape, please input again! [0.5, 2.0]");
	}
	double simTime = atof(argv[6]);
	if(simTime < MIN_SIM_TIME || simTime > MAX_SIM_TIME){
		DieWithUserMessage("Error", "Invalid simulation time, please input again! [120, 36000]");
	}
	int flipBehave = atoi(argv[7]);
	if(flipBehave < MIN_FLIP_BEHAVIOR || flipBehave > MAX_FLIP_BEHAVIOR){
		DieWithUserMessage("Error", "Invalid flipping behavior specification, please input again! [1, 4]");
	}
	srand(time(NULL));
	Manager manager(userNumber, channelNumber, SDVNumber, sliceNumber, zipfShape, simTime, flipBehave);
	manager.initSDV(); //Initialize the SDV system
	manager.excuteSDV(); //Excution
	manager.showSDVResult(); //Display the statistics
	return 0;
}
