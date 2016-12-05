#include "../include/servo.h"
#include <wiringPi.h>

Servo::Servo(int pin){
  _hwPwm = pin == HW_PWM_PIN;
  _pin = pin;
  if(_hwPwm){
    pinMode(pin,PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(CLOCK);
    pwmSetRange(RANGE);
  }else{
  //TODO setup software pwm
  }
}

bool Servo::setPos(int deg){
  if(deg < -DEG_RANGE || deg > DEG_RANGE) return false;
  //deg += 90;
  int pulse  = (PULSE_PER_DEG * deg) + OFFSET;
  //int pulse = deg;
 if(_hwPwm){
    pwmWrite(_pin, pulse);
    return true;
  }
  //TODO implement software pwm
  return false;
}
