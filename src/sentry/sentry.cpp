#include "../include/sentry.h"
#include <iostream>
#include <cstdlib>
#include <wiringPi.h>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;

Sentry::Sentry(){
  _lastEmail = 0;
}
Sentry::~Sentry(){
  close(_fd[1]);
}

int Sentry::setup(){
  //int distance;
  string alarm;
  string distance;

  cout<<"Enter a Distance: ";
  getline(cin,distance);
  cout<<"Set Alarm? (y/n): ";
  getline(cin,alarm);
  _alarm = alarm == "y";

  if(_alarm){
    fstream file ("/tmp/sentry_alarm.py",ios::out | ios::in| ios::trunc);
    file << "#!/usr/bin/python\n";
    file << "import sys, os\n";
    file << "triggerFd = int(sys.argv[1])\n";
    file << "from sense_hat import SenseHat\n";
    file << "from time import sleep\n";
    file << "sense = SenseHat()\n";
    file << "red = (255, 0, 0)\n";
    file << "blue = (0,0,255)\n";
    file << "def alarm():\n";
    file << "\ti = 5\n";
    file << "\twhile(i>0):\n";
    file << "\t\tsense.clear(red)\n";
    file << "\t\tsleep(.25)\n";
    file << "\t\tsense.clear(blue)\n";
    file << "\t\tsleep(.25)\n";
    file << "\t\ti = i-1\n";
    file << "\tsense.clear()\n";
    file << "result = 0\n";
    file << "while(os.read(triggerFd,1) != \"\"):\n";
    file << "\talarm()\n";
    file << "os.close(triggerFd)\n";
    file.close();
    pipe(_fd);
    string command = "python /tmp/sentry_alarm.py "+ std::to_string(_fd[0]) +" & > /dev/null &";
    system(command.c_str());
  }
  system("chmod 777 /tmp/sentry_alarm.py");

  cout<<"\nEmail (Leave Blank for No Email): ";
  getline(cin,_email);
  cout<<_email<<endl;
  cout<<"\nArming in 10 seconds"<<endl;
  delay(10000);
  cout<<"Armed"<<endl;

  return stoi(distance);
}

void Sentry::objectDetected(int distance){
  cout<<"An Object Has Been Detected Within " + to_string(distance)+" Inches"<<endl;
  if(_email.size()>0) sendEmail("An Object Has Been Detected Within " + to_string(distance)+" Inches");
  if(_alarm) alarm();
  //delay(300000);
}

void Sentry::deviceMoved(){
  cout<<"The Device Has Been Moved"<<endl;
  if(_email.size()>0) sendEmail("The Device Has Been Moved");
  if(_alarm) alarm();
  //delay(300000);
}

void Sentry::alarm(){
  char buff = 'a'; 
  write(_fd[1],&buff,sizeof(char));
  system("speaker-test -f500 -tsine -P2 -l1 > /dev/null");
  //system("python /tmp/sentry_alarm.py > /dev/null");
  /*char buff = 'a';
  write(_fd[1],&buff,sizeof(char));*/
}

void Sentry::sendEmail(std::string message){
 // unsigned int curr = millis();
 // if(curr - _lastEmail > 300000 || _lastEmail == 0){
    string command = "echo \""+ message +"\" | mail -s \"Sentry Bot Event\" " + _email;
    system(command.c_str());
 // }
 // _lastEmail = curr;
}
