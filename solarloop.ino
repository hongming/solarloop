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
int loop_leds;
int loop_pin_latch = 8;
int loop_pin_clock = 12;
int loop_pin_data = 11;
float loop_leds_number;
float daygap;
int loop_leds_day=8;

RTC_DS1307 rtc;
sundata solarloop=sundata(31.2,121.2,8); 

void setup() {
  pinMode(loop_pin_latch,OUTPUT);
  pinMode(loop_pin_clock,OUTPUT);
  pinMode(loop_pin_data,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
     rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

  if(!rtc.isrunning()){
    Serial.println("Check your RTC!");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }

}

void loop() {
  DateTime loopnow = rtc.now();

  Serial.print(loopnow.year());
  Serial.print("-");
  Serial.print(loopnow.month());
  Serial.print("-");
  Serial.print(loopnow.day());
  Serial.print("-");
  Serial.print(loopnow.hour()); 
   Serial.print("-");
  Serial.print(loopnow.minute()); 
    Serial.print("-");
  Serial.print(loopnow.second());
    Serial.println("cool");
//  Serial.println(loopnow.year()+'-'+loopnow.month()+'-'+loopnow.day());
  solarloop.time(loopnow.year(),loopnow.month(),loopnow.day(),loopnow.hour(),loopnow.minute(),loopnow.second());
  solarloop.calculations();
  loop_sunrise =solarloop.sunrise_time();
  loop_sunset = solarloop.sunset_time();
// loop_currenttime = loopnow.hour()+loopnow.minute()/60.0+loopnow.second()/3600.0;
  loop_currenttime = 10;
  daygap = (loop_sunset-loop_sunrise-1)/loop_leds_day;
  if(
  
  (loop_currenttime<= (loop_sunrise+loop_timeoffset) &&
    loop_currenttime >= (loop_sunrise-loop_timeoffset)  ) 
    
  ){
    loop_leds = 65535;
  }
  
   else if(
 
      (loop_currenttime<= (loop_sunset+loop_timeoffset) &&
    loop_currenttime >= (loop_sunset-loop_timeoffset) ) 
    
  ){
    loop_leds = 32767;
  }
  
  
  else if(
    loop_currenttime>(loop_sunrise+loop_timeoffset) && loop_currenttime <= (loop_sunset-loop_timeoffset) 
  
  )
  {
   loop_leds_number=ceil((loop_currenttime-loop_sunrise-loop_timeoffset)/daygap)*1.0;
  loop_leds =pow(2.0,loop_leds_number)+0.5;
  }
  else if(
        loop_currenttime<(loop_sunrise-loop_timeoffset) || loop_currenttime >(loop_sunset+loop_timeoffset) 
  
  )
  {
    
      loop_leds = 32767;

  }
  //ShiftOUT
    shiftOut(loop_pin_data,loop_pin_clock,MSBFIRST,(loop_leds >>8));
    shiftOut(loop_pin_data,loop_pin_clock,MSBFIRST,loop_leds);
    Serial.print(loop_sunrise);
    Serial.print("-");
    Serial.println(loop_sunset);    
    Serial.println(loop_currenttime);
    Serial.println(loop_leds_number);
    Serial.println(loop_leds);
        delay(2000);
}
