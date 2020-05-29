#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class keyValue
{
  public:
  String key, value;

  keyValue(){
  }

  keyValue(String key1, String value1)
  {
    key = key1;
    value = value1;
  }

  String getKey() { return key;}

  String getValue() {return value;}

  void setKey(String key1) {key = key1;}

  void setValue(String value1) {value = value1;}
};

class alarm
{
  public:
  int h, m, ampm;
  String msg;
  
  alarm()
  {
    h = 0;
    m = 0;
    ampm = -1;
    msg = "";
  }

  alarm(int h1, int m1, int ampm1, String msg1)
  {
    h = h1;
    m = m1;
    ampm = ampm1;
    msg = msg1;
  }

  int getH() {return h;}
  
  int getM() {return m;}

  int getAMPM() {return ampm;}

  String getMsg() {return msg;}

  void setH(int h1) {h = h1;}

  void setM(int m1) {m = m1;}

  void setAMPM(int ampm1) {ampm = ampm1;}

  void setMsg(String msg1) {msg = msg1;}
};

#define OLED_RESET 4
#define RTC_Address 0x68

Adafruit_SSD1306 display(OLED_RESET);

char Time[]     = "  :  :    ";
char Calendar[] = "  /  /20  ";
char temperature[] = " 00.00";
char temperature_msb;
byte i, second, minute, hour, day, date, month, year, temperature_lsb;
int ampm;

int ackNOA=0;
int ackH0=0, ackM0=0, ackS0=0, ackAMPM0=0, ackMSG0=0;
int ackH1=0, ackM1=0, ackS1=0, ackAMPM1=0, ackMSG1=0;
int ackH2=0, ackM2=0, ackS2=0, ackAMPM2=0, ackMSG2=0;
int ackH3=0, ackM3=0, ackS3=0, ackAMPM3=0, ackMSG3=0;
int ackVOL=0, ackVIB=0;

// Variables
alarm alarmList[4];
String myTime1="";
String myTime2="";
String myTime3="";
int numberOfAlarms=-1;
String volume="";
String vibration="";

// Buttons
int PWMPin = 9; // Output of PWM

int speakTimeButton = 7;
int readSpeakTimeButton = HIGH;

int vibrateTimeButton = 6;
int readVibrateTimeButton = HIGH;

int screenBrowseButton = 5;
int readScreenBrowseButton = HIGH;

int currentScreen = 0;

int triggerSOSButton = 2; // ISR does not work with OLED; ISR can be pin 2 or 3
int readSOSButton = HIGH;

// Talkie output at Digital 3
// Display pins at SDA = A4, SCL = A5

void setup() 
{
  Serial.begin(9600);
  pinMode(speakTimeButton, INPUT_PULLUP);
  pinMode(vibrateTimeButton, INPUT_PULLUP);
  pinMode(screenBrowseButton, INPUT_PULLUP);
  pinMode(triggerSOSButton, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.display();
  display.clearDisplay();
  showTimeScreen();

  for (int i=0; i<4; i++)
    alarmList[i] = alarm();
}

void loop() 
{
  // Listen to Changes on App
  if(Serial.available())
  {
    char c = Serial.read();
    if (c == '~')
    {
      fetchVariables();
    }
  }
  
  // TODO: Trigger SOS Button
  readSOSButton = digitalRead(triggerSOSButton);
  if (readSOSButton == LOW)
  {
    showSOSScreen();
    Serial.println("interrupt");
  }

  // TODO: Add Speak Code
  readSpeakTimeButton = digitalRead(speakTimeButton);
  if (readSpeakTimeButton == LOW)
  {
    Serial.println("Speak");
  }

  // TODO: Add Vibration Code
  readVibrateTimeButton = digitalRead(vibrateTimeButton);
  if (readVibrateTimeButton == LOW)
  {
    Serial.println("Vibrate");
  }

  // Alarm Event Check
  for (int i=0; i<4; i++)
  {
    if (alarmList[i].getAMPM() != -1)
    {
      if ((int)hour == alarmList[i].getH())
      {
        if ((int)minute == alarmList[i].getM())
        {
          if (ampm == alarmList[i].getAMPM())
          {
              // TODO: Mechanism to snooze alarm via button
              showAlarmScreen(alarmList[i]);
          }
        }
      }
    }
  }
  
  // Browse Screen
  if(digitalRead(screenBrowseButton) == LOW)
  {
    Serial.println("Screen Browse");
    readScreenBrowseButton++;
    if(readScreenBrowseButton%3 == 0)
    {
      showTimeScreen();
      currentScreen = 0;
    }
    else if(readScreenBrowseButton%3 == 1)
    {
      showSVScreen(volume.toInt(),vibration.toInt());
      currentScreen = 1;
    }
    else if(readScreenBrowseButton%3 == 2)
    {
      showAlarmListScreen();
      currentScreen = 2;
    }
  }
  else
  {
    // Allow Time updation when button not clicked
    if(currentScreen == 0)
      showTimeScreen();
    else if(currentScreen == 1)
      showSVScreen(volume.toInt(),vibration.toInt());
    else if(currentScreen == 2)
      showAlarmListScreen();
  }
}

void fetchVariables()
{
  Serial.println("fetch var");

  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      Serial.println("Fetch Done");
      printRoutine();
      //sendAcknowledgement();
      return;
    }
    if (c == '{')
    {
      keyValue fetch = extractKeyValue();
      String key = fetch.getKey();
      String value = fetch.getValue();
      if (key == "noa")
      {
        numberOfAlarms = value.toInt();
        for (int j = numberOfAlarms; j<4; j++)
          alarmList[j].setAMPM(-1);
        ackNOA = 1;
      }
      else if (key == "h0")
      {
        alarmList[0].setH(value.toInt());
        ackH0 = 1;
      }
      else if (key == "m0")
      {
        alarmList[0].setM(value.toInt());
        ackM0 = 1;
      }
      else if (key == "ap0")
      {
        alarmList[0].setAMPM(value.toInt());
        ackAMPM0 = 1;
      }
      else if (key == "ms0")
      {
        alarmList[0].setMsg(value);
        ackMSG0 = 1;
      }
      else if (key == "h1")
      {
        alarmList[1].setH(value.toInt());
        ackH1 = 1;
      }
      else if (key == "m1")
      {
        alarmList[1].setM(value.toInt());
        ackM1 = 1;
      }
      else if (key == "ap1")
      {
        alarmList[1].setAMPM(value.toInt());
        ackAMPM1 = 1;
      }
      else if (key == "ms1")
      {
        alarmList[1].setMsg(value);
        ackMSG1 = 1;
      }
      else if (key == "h2")
      {
        alarmList[2].setH(value.toInt());
        ackH2 = 1;
      }
      else if (key == "m2")
      {
        alarmList[2].setM(value.toInt());
        ackM2 = 1;
      }
      else if (key == "ap2")
      {
        alarmList[2].setAMPM(value.toInt());
        ackAMPM2 = 1;
      }
      else if (key == "ms2")
      {
        alarmList[2].setMsg(value);
        ackMSG2 = 1;
      }
      else if (key == "h3")
      {
        alarmList[3].setH(value.toInt());
        ackH3 = 1;
      }
      else if (key == "m3")
      {
        alarmList[3].setM(value.toInt());
        ackM3 = 1;
      }
      else if (key == "ap3")
      {
        alarmList[3].setAMPM(value.toInt());
        ackAMPM3 = 1;
      }
      else if (key == "ms3")
      {
        alarmList[3].setMsg(value);
        ackMSG3 = 1;
      }
      else if (key == "vo")
      {
        volume = value;
        ackVOL = 1;
      }
      else if (key == "vi")
      {
        vibration = value;
        ackVIB = 1;
      }
    }
  }
}

void showTimeScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  getRTCTime();
  display.print(Time);
  display.display();
}

void showSOSScreen()
{
  for(int i=5;i>=0;i--)
  {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(8,0);
    display.println("Sending SOS in ...");
    display.setTextSize(3);
    display.setCursor(56,12);
    display.println(i);
    delay(1000);
    display.display();
  }
  delay(5000);
}

void showFMWScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(26,10);
  display.println("Finding your");
  display.setCursor(26,18);
  display.print("watch .....");
  display.display();
}

void showAlarmScreen(alarm alarm1)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(12,4);
  display.print(alarm1.getH());
  display.print(":");
  display.print(alarm1.getM());
  if (alarm1.getAMPM() == 0)
    display.println("AM");
  else
    display.println("PM");
  display.setTextSize(1);
  display.setCursor(10,22);
  display.println(alarm1.getMsg());
  display.display();
  delay(5000);
}

void showAlarmListScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  for(int i=0; i<4; i++)
  {
    if (alarmList[i].getAMPM() != -1)
    {
      char c = '1'+i;
      String j;
      j.concat(c);
      display.print("Alarm "+j+" - ");
      display.print(alarmList[i].getH());
      display.print(":");
      display.print(alarmList[i].getM());
      if (alarmList[i].getAMPM() == 0)
          display.println("AM");
        else
          display.println("PM");  
    }
  }
  display.display();
}

void showSVScreen(int volume, int vibration)
{
  int i;
  int x_start=0, y_start=8;
  int rect_width=20, rect_height=5;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  display.println("Volume");
  for(i=1;i<=5;i++)
  {
    int colorShow = volume>=i?WHITE:BLACK;
    display.fillRect(x_start, y_start, rect_width, rect_height, colorShow);
    x_start = x_start + rect_width + 2;
  }

  x_start = 0;
  y_start = 24;
  display.setCursor(0, 16);
  display.println("Vibration");
  for(i=1;i<=53;i++)
  {
    int colorShow = vibration>=i?WHITE:BLACK;
    display.fillRect(x_start, y_start, rect_width, rect_height, colorShow);
    x_start = x_start + rect_width + 2;
  }
  display.display();
}

void sendAcknowledgement()
{  
  if (ackNOA == 0 || ackH0 == 0 || ackM0 == 0 || ackS0 == 0 || ackAMPM0 == 0)
  {
    Serial.println("ackAlarmTime0");
  }
  else if (ackH1 == 0 || ackM1 == 0 || ackS1 == 0 || ackAMPM1 == 0)
  {
    Serial.println("ackAlarmTime1");
  }
  else if (ackH2 == 0 || ackM2 == 0 || ackS2 == 0 || ackAMPM2 == 0)
  {
    Serial.println("ackAlarmTime2");    
  }
  else if (ackH3 == 0 || ackM3 == 0 || ackS3 == 0 || ackAMPM3 == 0)
  {
    Serial.println("ackAlarmTime3");    
  }
  else if (ackMSG0 == 0 || ackMSG1 == 0 || ackMSG2 == 0 || ackMSG3 == 0)
  {
    Serial.println("ackAlarmMsg");
  }
  else if (ackVOL == 0 || ackVIB == 0)
  {
    Serial.println("ackSV");
  }
}

keyValue extractKeyValue()
{
  Serial.println("KV");

  String key="", value="";
  while(true)
  {
    char ch = Serial.read();
    if(ch == '*')
    {
      while (true)
      {
        char c = Serial.read();
        if(c == '}')
        {
          keyValue abc = keyValue(key, value);
          return(abc);
        }
        value = appendChar(value, c);
      }
    }
    key = appendChar(key, ch);
  }
}

String appendChar(String s, char ch)
{
  // Upper Case || Lower Case || Numbers || space || colon || comma
  if((ch>=65 && ch<=90) || (ch>=97 && ch<=122) || (ch>=48 && ch<=57) || ch==32 || ch==58 || ch==44)
  {
    s = s + ch;
  }
  return s;
}

void printRoutine()
{
  Serial.println();
  Serial.print("Time: ");
  Serial.print(Time);
  Serial.print("Number of Alarms: ");
  Serial.println(numberOfAlarms);
  for (int i=0; i<4; i++)
  {
    Serial.print(alarmList[i].getH());
    Serial.print(":");
    Serial.print(alarmList[i].getM());
    if (alarmList[i].getAMPM() == 0)
        Serial.println("AM");
      else
        Serial.println("PM");  
    Serial.println(alarmList[i].getMsg());
  }
  Serial.print("Volume: ");
  Serial.println(volume);
  Serial.print("Vibration: ");
  Serial.println(vibration);
}

void getRTCTime()
{
  Wire.beginTransmission(RTC_Address);                 // Start I2C protocol with DS3231 address
  Wire.write(0);                                // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
  second = Wire.read();                         // Read seconds from register 0
  minute = Wire.read();                         // Read minuts from register 1
  hour   = Wire.read();                         // Read hour from register 2
  day    = Wire.read();                         // Read day from register 3
  date   = Wire.read();                         // Read date from register 4
  month  = Wire.read();                         // Read month from register 5
  year   = Wire.read();                         // Read year from register 6
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0x11);                             // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 2);                    // Request 2 bytes from DS3231 and release I2C bus at end of reading
  temperature_msb = Wire.read();                // Read temperature MSB
  temperature_lsb = Wire.read();                // Read temperature LSB
   
  DS3231_display();
}

void DS3231_display()
{
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);
  date   = (date >> 4)   * 10 + (date & 0x0F);
  month  = (month >> 4)  * 10 + (month & 0x0F);
  year   = (year >> 4)   * 10 + (year & 0x0F);
  // End conversion

  
  if ((int)hour > 12)
  {
    ampm = 1; // PM
    Time[8] = 'P';
    hour = hour - 12;
  }
  else if((int)hour == 0)
  {
    ampm = 0; // AM
    Time[8] = 'A';
    hour = hour + 12;
  }
  else if((int)hour == 12)
  {
    Time[8] = 'P';
    ampm = 1; // PM
  }
  else
  {
    Time[8] = 'A';
    ampm = 0; // AM
  }

  Time[9] = 'M';
  Time[7]     = second % 10 + 48;
  Time[6]     = second / 10 + 48;
  Time[4]     = minute % 10 + 48;
  Time[3]     = minute / 10 + 48;
  Time[1]     = hour   % 10 + 48;
  Time[0]     = hour   / 10 + 48;
  Calendar[9] = year   % 10 + 48;
  Calendar[8] = year   / 10 + 48;
  Calendar[4] = month  % 10 + 48;
  Calendar[3] = month  / 10 + 48;
  Calendar[1] = date   % 10 + 48;
  Calendar[0] = date   / 10 + 48;
  
  if(temperature_msb < 0)
  {
    temperature_msb = abs(temperature_msb);
    temperature[0] = '-';
  }
  else
    temperature[0] = ' ';
  
  temperature_lsb >>= 6;
  temperature[2] = temperature_msb % 10  + 48;
  temperature[1] = temperature_msb / 10  + 48;
  
  if(temperature_lsb == 0 || temperature_lsb == 2)
  {
    temperature[5] = '0';
    if(temperature_lsb == 0) 
      temperature[4] = '0';
    else                     
      temperature[4] = '5';
  }
  if(temperature_lsb == 1 || temperature_lsb == 3)
  {
    temperature[5] = '5';
    if(temperature_lsb == 1) 
      temperature[4] = '2';
    else                     
      temperature[4] = '7';
  }
}

