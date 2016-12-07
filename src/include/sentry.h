#ifndef _SENTRY_H
#define _SENTRY_H

#include "../include/scanner.h"
#include "../include/position.h"
#include <string>

class Sentry : public Scanner::ScannerEvent, public Position::PositionEvent{
  std::string _email;
  bool _alarm;
  unsigned int _lastEmail;
  int _fd [2];
  void alarm();
  void sendEmail(std::string message);
public:
  Sentry();
  ~Sentry();
  void objectDetected(int distance);
  void deviceMoved();
  int setup();
};

#endif
