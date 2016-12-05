#include "../include/sentry.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>
#include <string>

using namespace std;

int Sentry::setup(){
  //int distance;
  string alarm;
  string distance;

  cout<<"Enter a Distance: ";
  getline(cin,distance);
  cout<<"Set Alarm? (y/n): ";
  getline(cin,alarm);
  _alarm = alarm == "y";
  cout<<"\nEmail (Leave Blank for No Email): ";
  getline(cin,_email);

  cout<<"\nArming in 10 seconds"<<endl;
  delay(10000);
  cout<<"Armed"<<endl;

  return stoi(distance);
}

void Sentry::objectDetected(int distance){
  cout<<"An Object Has Been Detected Within " + to_string(distance)+" Inches"<<endl;
  if(_email.size()>0) sendEmail("An Object Has Been Detected Within " + to_string(distance)+" Inches");
  if(_alarm) alarm();
  delay(300000);
}

void Sentry::deviceMoved(){
  cout<<"The Device Has Been Moved"<<endl;
  if(_email.size()>0) sendEmail("The Device Has Been Moved");
  if(_alarm) alarm();
  delay(300000);
}

void Sentry::alarm(){
  for(int i=0;i<4;++i){
    cout << '\a';
    delay(250);
  }
}

void Sentry::sendEmail(std::string message){
  string command = "echo \""+ message +"\" | mail -s \"Sentry Bot Event\" " + _email;
  system(command.c_str());
}
