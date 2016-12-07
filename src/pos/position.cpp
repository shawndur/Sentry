////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTIMULib
//
//  Copyright (c) 2014-2015, richards-tech, LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in
//  the Software without restriction, including without limitation the rights to use,
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
//  Software, and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "RTIMULib.h"
#include <exception>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <signal.h>
#include <pthread.h>
#include "../include/position.h"
#include <iostream>
#include <sstream>
#include <cmath>
//#include <Python.h>

#define MAX_MOVEMENT 2

using namespace std;

Position::Position(PositionEvent & handler,int maxMovement):_handler(handler),_maxMovement(maxMovement){}


void Position::positionThread(){
  int sampleCount = 0;
  int sampleRate = 0;
  uint64_t rateTimer;
  uint64_t displayTimer;
  uint64_t now;
  RTFLOAT x,y,z;
  bool firstRun = true;

  //  Using RTIMULib here allows it to use the .ini file generated by RTIMULibDemo.
  //  Or, you can create the .ini in some other directory by using:
  //      RTIMUSettings *settings = new RTIMUSettings("<directory path>", "RTIMULib");
  //  where <directory path> is the path to where the .ini file is to be loaded/saved

  RTIMUSettings *settings = new RTIMUSettings("RTIMULib");

  RTIMU *imu = RTIMU::createIMU(settings);

  if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
    printf("No IMU found\n");
    exit(1);
  }

  //  This is an opportunity to manually override any settings before the call IMUInit

  //  set up IMU

  imu->IMUInit();

  //  this is a convenient place to change fusion parameters

  imu->setSlerpPower(0.02);
  imu->setGyroEnable(true);
  imu->setAccelEnable(true);
  imu->setCompassEnable(true);

  //  set up for rate timer

  rateTimer = displayTimer = RTMath::currentUSecsSinceEpoch();

  //  now just process data

  while (_run) {
    //  poll at the rate recommended by the IMU

    usleep(imu->IMUGetPollInterval() * 1000);

    while (imu->IMURead()) {
      RTIMU_DATA imuData = imu->getIMUData();
      sampleCount++;

      now = RTMath::currentUSecsSinceEpoch();

      //  display 10 times per second

      if ((now - displayTimer) > 100000) {
        /*printf("Sample rate %d: %s\r", sampleRate, RTMath::displayDegrees("", imuData.fusionPose));
        fflush(stdout);*/
        RTVector3& vec = imuData.fusionPose;
        RTFLOAT tx = vec.x() * RTMATH_RAD_TO_DEGREE;
        RTFLOAT ty = vec.y() * RTMATH_RAD_TO_DEGREE;
        RTFLOAT tz = vec.z() * RTMATH_RAD_TO_DEGREE;
        if(!firstRun && max(abs(x-tx),max(abs(y-ty),abs(z-tz))) > _maxMovement){
          _handler.deviceMoved();
        }
        x = tx;
        y = ty;
        z = tz;
        firstRun = false;
        displayTimer = now;
      }

      //  update rate every second

      if ((now - rateTimer) > 1000000) {
        sampleRate = sampleCount;
        sampleCount = 0;
        rateTimer = now;
      }
    }
  }
}

void * Position::launcher(void * arg){
  try{
    ((Position *)arg)->positionThread();
  }catch(exception e){
    cerr<<"error could not run position thread"<<endl;
  }
  pthread_exit(NULL);
}

pthread_t * Position::run(bool * run){
  _run = run;
  pthread_t * thread = new pthread_t;
  if(pthread_create(thread, NULL, launcher, (void*)this)) throw new exception();
  return thread;
}
