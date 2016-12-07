#include "../include/scanner.h"
#include <wiringPi.h>
#include <exception>

using namespace std;

Scanner::Scanner(Ultrasonic & sensor, Servo & servo, ScannerEvent & handler,int detectRange,int precision)
  :_ping(sensor),_servo(servo),_handler(handler),_detectRange(detectRange),_scanPrecision(precision){}

void Scanner::scanThread(){
  vector<int> objects;
  scanRange(objects,_scanPrecision);
  while(*_run){
    for(int object : objects){
      if(object <= _detectRange && object>-1){
	_handler.objectDetected(object);
	break;
      }
    }
    objects.clear();
    scanRange(objects,_scanPrecision);
  }
}

void * Scanner::launcher(void *arg){
  ((Scanner *)arg)->scanThread();
  pthread_exit(NULL);
}

pthread_t * Scanner::run(bool * run){
  _run = run;
  pthread_t * thread = new pthread_t;
  if(pthread_create(thread, NULL, launcher, (void*)this)) throw new exception();
  return thread;
}

std::vector<int> & Scanner::scanRange(std::vector<int> & v, int offset){
  return scanRange(v,-_servo.DEG_RANGE,_servo.DEG_RANGE,offset);
}

std::vector<int> & Scanner::scanRange(std::vector<int> & v, int start, int end, int offset){
  for(;start<=end;start=start+offset){
    v.push_back(scan(start));
  }
  return v;
}

int Scanner::scan(int deg){
  _servo.setPos(deg);
  delay(250);
  _ping.DistanceMeasure();
  return _ping.microsecondsToInches();
}
