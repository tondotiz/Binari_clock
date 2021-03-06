#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
int datapin = 2;
int clockpin = 3;
int latchpin = 4;
int datapin2 = 8;
int clockpin2 = 9;
int latchpin2= 10;

void setup()
{
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2017, 12, 02, 15, 33, 10));
  }

  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(datapin2, OUTPUT);
  pinMode(clockpin2, OUTPUT);
  pinMode(latchpin2, OUTPUT);
}

void loop()
{

  DateTime now = rtc.now();
  // All used for checking the time of the clock
  // This section can be removed when everything is working
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  // End of section that can be removed

  int mins = now.minute();
  int secs = now.second();
  int hr = now.hour();
  // convert to 12 hour time
  if (hr>12)
  {
    hr = hr-12;
  }

  // variables to describe pattern of on lights
  byte data1 = 0;
  byte data2 = 0;


  // encode the time
  // hr = 1st four bits controlled by the first shift register
  for (int i =0;i<4;i++)
  {
    if (bitRead(hr,i)==1)
    {
      bitWrite(data1,3-i,1);
    }
  }
  // mins on the first shift register (last 4 leds)
  for (int i =2;i<6;i++)
  {
    if (bitRead(mins,i)==1)
    {
      bitWrite(data1,9-i,1);
    }
  }
  // mins on the second shift register (first 2 leds)
  for (int i =0;i<2;i++)
  {
    if (bitRead(mins,i)==1)
    {
      bitWrite(data2,1-i,1);
    }
  }
  // seconds, controlled by the second shift register (all 6 leds)
  for (int i =2;i<8;i++)
  {
    if (bitRead(secs,i-2)==1)
    
      bitWrite(data2,9-i,1);
    }
  }
  // output the information
  writeByte(data1,1);
  writeByte(data2,2);
  // a pause every one second for the serial monitor output
  delay(1000);

}

void writeByte(byte data, byte set)
{
  int d,c,l;
  if (set==1)
  {
    d = 2;
    c = 3;
    l = 4;
  }
  else if (set==2)
  {
    d = 8;
    c = 9;
    l = 10;
  }
  shiftOut(d, c, MSBFIRST, data);
  // toggle the latch pin so that the data appears as an output
  digitalWrite(l, HIGH);
  digitalWrite(l, LOW);
}
