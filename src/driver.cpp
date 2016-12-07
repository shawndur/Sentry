#include "./include/scanner.h"
#include "./include/ping.h"
#include "./include/position.h"
#include "./include/sentry.h"
#include "./include/servo.h"
#include <csignal>
#include <wiringPi.h>
#include <sstream>
#include <iostream>

#define MAX_MOVEMENT 2
#define PING_PIN 22
#define SERVO_PIN 18
#define PRECISION 5

using namespace std;

volatile bool run;

void sigHandler(int sig){
  run = false;
}

int main(int argc, char const *argv[]) {
  signal(SIGINT,sigHandler);
  signal(SIGTERM,sigHandler);
  wiringPiSetupGpio ();
  run = true;

  Sentry sentry;
  int distance = sentry.setup();
  Ultrasonic ping (PING_PIN);
  Servo servo(SERVO_PIN);
  Scanner scanner (ping, servo, sentry, distance, PRECISION);
  Position position (sentry,MAX_MOVEMENT);

  bool threadRun = true;
  scanner.run(&threadRun);
  position.run(&threadRun);

  while(run){}

  threadRun = false;
  delay(250);
  return 0;
}
