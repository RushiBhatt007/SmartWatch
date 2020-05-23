#include <Wire.h>
#define RTC_Address 0x68
byte RTCtime[7] = {0,42,1,1,24,5,20}; // Replace the 0's with seconds, minutes, hours, day, date, month and year. 
//Note Sunday = day 1 and saturday is day 7. 
byte decimal2BCD(byte val)
  {
    return (16*val/10+val%10);
  }
byte BCD2decimal(byte val)
  {
    
    return (10*val/16+val%16);
  }
void setup()
{
  //This  is for setting up the RTC time for the first time, following which please comment the line
  //Otherwise the time will be rewritten every time the program is burnt
  Wire.begin();
  Serial.begin(9600);
  setRTCtime();
}
void setRTCtime(void)
{
  Wire.beginTransmission(RTC_Address);
  Wire.write(0);
  Wire.write(decimal2BCD(RTCtime[0]));
  Wire.write(decimal2BCD(RTCtime[1]));
  Wire.write(decimal2BCD(RTCtime[2]));
  Wire.write(decimal2BCD(RTCtime[3]));
  Wire.write(decimal2BCD(RTCtime[4]));
  Wire.write(decimal2BCD(RTCtime[5]));
  Wire.write(decimal2BCD(RTCtime[6]));//Year is in between 00-99
  Wire.endTransmission();
}
void getRTCtime()
{
  Wire.beginTransmission(RTC_Address);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(RTC_Address,7);
  RTCtime[0] = BCD2decimal(Wire.read() & 0x7f);
  RTCtime[1] = BCD2decimal(Wire.read());
  RTCtime[2] = BCD2decimal(Wire.read() & 0x3f);
  RTCtime[3] = BCD2decimal(Wire.read());
  RTCtime[4] = BCD2decimal(Wire.read());
  RTCtime[5] = BCD2decimal(Wire.read());
  RTCtime[6] = BCD2decimal(Wire.read());
}
void loop()
{
  getRTCtime();
}

