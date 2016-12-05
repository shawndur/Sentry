/***************************************************************************/
//	Function: Measure the distance to obstacles in front and print the distance
//			  value to the serial terminal.The measured distance is from
//			  the range 0 to 400cm(157 inches).
//	Hardware: Ultrasonic Range sensor
//	Arduino IDE: Arduino-1.0
//	Author:	 LG
//	Date: 	 Jan 17,2013
//	Version: v1.0 modified by FrankieChu
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/*****************************************************************************/

/*****************************************************************************
* This code has been ported from Arduino to raspberry pi. The information on the
*   original code can be found above and the source can be found at:
*   https://github.com/RadioShackCorp/2760342-Range-Sensor/blob/master/UltrasonicRangeSensor.ino
*
* Date: Oct 23, 2016
* Version: 2.0 modified by Shawn Durandetto
******************************************************************************/

#include "../include/ping.h"
#include <wiringPi.h>

using namespace std;

Ultrasonic::Ultrasonic(int pin):_pin(pin){}

/*Begin the detection and get the pulse back signal*/
void Ultrasonic::DistanceMeasure(void){
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	//pullUpDnControl (_pin, PUD_UP);

	unsigned int pulseStart = micros();
	unsigned int pulseEnd = micros();
	unsigned int timeoutStart = micros();
	unsigned int timeout = TIMEOUT;

	while(digitalRead(_pin) == LOW){
		pulseStart = micros();
		if((pulseStart - timeoutStart) > timeout){
			_duration = -1;
			return;
		}
	}

	while(digitalRead(_pin) == HIGH){
		pulseEnd = micros();
		if((pulseEnd - timeoutStart) > timeout){
			_duration = -1;
			return;
		}
	}

	_duration = pulseEnd - pulseStart;
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void){
	if(_duration == -1) return _duration;
	return _duration/29/2;
}

/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::microsecondsToInches(void){
	if(_duration == -1) return _duration;
	return _duration/74/2;
}

/******************************************************************************
Original Code:
#include "Arduino.h"
class Ultrasonic
{
public:
Ultrasonic(int pin);
void DistanceMeasure(void);
long microsecondsToCentimeters(void);
long microsecondsToInches(void);
private:
int _pin;//pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
long duration;// the Pulse time received;
};
Ultrasonic::Ultrasonic(int pin)
{
_pin = pin;
}
//Begin the detection and get the pulse back signal
void Ultrasonic::DistanceMeasure(void)
{
pinMode(_pin, OUTPUT);
digitalWrite(_pin, LOW);
delayMicroseconds(2);
digitalWrite(_pin, HIGH);
delayMicroseconds(5);
digitalWrite(_pin,LOW);
pinMode(_pin,INPUT);
duration = pulseIn(_pin,HIGH);
}
//The measured distance from the range 0 to 400 Centimeters
long Ultrasonic::microsecondsToCentimeters(void)
{
return duration/29/2;
}
//The measured distance from the range 0 to 157 Inches
long Ultrasonic::microsecondsToInches(void)
{
return duration/74/2;
}
Ultrasonic ultrasonic(7);
void setup()
{
Serial.begin(9600);
}
void loop()
{
long RangeInInches;
long RangeInCentimeters;
ultrasonic.DistanceMeasure();// get the current signal time;
RangeInInches = ultrasonic.microsecondsToInches();//convert the time to inches;
RangeInCentimeters = ultrasonic.microsecondsToCentimeters();//convert the time to centimeters
Serial.println("The distance to obstacles in front is: ");
Serial.print(RangeInInches);//0~157 inches
Serial.println(" inch");
Serial.print(RangeInCentimeters);//0~400cm
Serial.println(" cm");
delay(100);
}
*******************************************************************************/
