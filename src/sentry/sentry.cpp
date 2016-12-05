#include "../include/sentry.h"
#include <iostream>
#include <wiringPi.h>

using namespace std;

int Sentry::setup(){
  int distance;
  string alarm;

  cout<<"Enter a Distance: ";
  cin>>distance;
  cout<<"Set Alarm? (y/n): ";
  cin>>alarm;
  _alarm = alarm == "y";
  cout<<"Email (Leave Blank for No Email): ";
  cin>>_email;

  cout<<"Arming in 10 seconds"<<endl;
  delay(10000);
  cout<<"Armed"<<endl;

  return distance;
}

void Sentry::objectDetected(int distance){
  if(_alarm) alarm();
  if(_email.size()>0) sendEmail("An Object Has Been Detected Within " + to_string(distance)+" Inches");
}

void Sentry::deviceMoved(){
  if(_alarm) alarm();
  if(_email.size()>0) sendEmail("The Device Has Been Moved");
}

void Sentry::alarm(){
  cout<<"alarm"<<endl;
}

void Sentry::sendEmail(std::string message){
  cout<<"email"<<endl;
}
