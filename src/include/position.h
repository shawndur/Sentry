#ifndef _POS_H
#define _POS_H

#include <pthread.h>

class Position{
public:
  class PositionEvent{
  public:
    virtual ~PositionEvent(){};
    virtual void deviceMoved() = 0;
  };

private:
  PositionEvent & _handler;
  bool * _run;
  int _maxMovement;

  Position();
  void positionThread();
  static void * launcher(void * arg);
public:
  Position(PositionEvent & handler, int maxMovement);
  pthread_t * run(bool * run);
};

#endif
