//DS1307 library from https://github.com/adafruit/RTClib
//ShiftOut for 74HC595 http://arduino.cc/en/Reference/ShiftOut 
//ShiftOut Example http://arduino.cc/en/Tutorial/ShiftOut

#include "Wire.h"
#include "RTClib.h"
#include "sundata.h"
float loop_sunrise;
float loop_sunset;
float loop_currenttime;
float loop_timeoffset=0.5;
int loop_leds = 8;
int loop_pin_latch = 8;
int loop_pin_clock = 12;
int loop_pin_data = 11;

RTC_DS1307 rtc;
sundata solarloop=sundata(31.2,121.2,8); 

void setup() {
  pinMode(loop_pin_latch,OUTPUT);
  pinMode(loop_pin_clock,OUTPUT);
  pinMode(loop_pin_data,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  if(!rtc.isrunning()){
    Serial.println("Check your RTC!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }

}

void loop() {
  DateTime loopnow = rtc.now();
  Serial.println(loopnow.year()+'-'+loopnow.month()+'-'+loopnow.day());
  solarloop.time(loopnow.year(),loopnow.month(),loopnow.day(),loopnow.hour(),loopnow.minute(),loopnow.second());
  solarloop.calculations();
  loop_sunrise =solarloop.sunrise_time();
  loop_sunset = solarloop.sunset_time();
  loop_currenttime = loopnow.hour()+loopnow.minute()/60+loopnow.second()/3600;
  if(
  
  (loop_currenttime<= (loop_sunrise+loop_timeoffset) &&
    loop_currenttime >= (loop_sunrise-loop_timeoffset) ) ||
      (loop_currenttime<= (loop_sunset+loop_timeoffset) &&
    loop_currenttime >= (loop_sunset-loop_timeoffset) ) 
    
  ){
    loop_leds = 255;
  }
  else
  {
    //speicific led

  }
  //ShiftOUT
    shiftOut(loop_pin_data,loop_pin_clock,MSBFIRST,(loop_leds >>8));
    shiftOut(loop_pin_data,loop_pin_clock,MSBFIRST,loop_leds);
}
