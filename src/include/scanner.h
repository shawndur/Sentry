#ifndef _SCAN_H
#define _SCAN_H

#include "servo.h"
#include "ping.h"
#include <vector>
#include <pthread.h>

class Scanner {
public:
  class ScannerEvent{
  public:
    virtual ~ScannerEvent(){};
    virtual void objectDetected(int distance) = 0;
  };
private:
  Ultrasonic & _ping;
  Servo & _servo;
  ScannerEvent & _handler;
  bool * _run;
  int _detectRange;
  int _scanPrecision;

  Scanner();
  int scan(int deg);
  std::vector<int> & scanRange(std::vector<int> & v, int start, int end, int offset = 1);
  std::vector<int> & scanRange(std::vector<int> & v, int offset = 1);
  void scanThread();
  static void * launcher(void * arg);
public:
  Scanner (Ultrasonic & sensor, Servo & servo, ScannerEvent & handler, int detectRange);
  pthread_t * run(bool * run);
};

#endif
