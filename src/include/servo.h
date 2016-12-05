#ifndef _SERVO_H
#define _SERVO_H

class Servo{
  const int HW_PWM_PIN = 18;
  const int CLOCK = 192;
  const int RANGE = 2000;
  const double PULSE_PER_DEG = ((double)-37)/((double)36);
  const double OFFSET = ((double)315)/((double)2);
  bool _hwPwm;
  int _pin;
  Servo();
public:
  const int DEG_RANGE = 180/2;
  const int MS_PER_DEG = 2;
  Servo(int pin);
  bool setPos(int deg);
};

#endif
