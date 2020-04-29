#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

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
Adafruit_SSD1306 display(OLED_RESET);

char* tim;
int h1=0, h2=0, m1=0, m2=0, s1=0, s2=0, ampm=0;
int h, m, s; 

int ackHr=0, ackMin=0, ackSec=0;
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

int speak = 6;  // Button for speak
int vibr = 7;   // Button for vibrator
int PWMPin = 9; // Output of PWM

int screenBrowseButton = 5;
int readScreenBrowseButton = 0;

int currentScreen = 0;

int triggerSOSButton = 2; // ISR does not work with OLED; ISR can be pin 2 or 3
int readSOSButton = 0;

int ultimateCount = 0;

// Talkie output at Digital 3
// Display pins at SDA = A4, SCL = A5

void setup() 
{
  Serial.begin(9600);
  pinMode(speak, INPUT);
  pinMode(vibr, INPUT);
  pinMode(screenBrowseButton, INPUT);
  pinMode(triggerSOSButton, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.display();
  display.clearDisplay();
  showTimeScreen();

  for (int i=0; i<4; i++)
    alarmList[i] = alarm();
}

void loop() 
{
  Serial.println("loop");
  
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
  if (readSOSButton == HIGH)
  {
    showSOSScreen();
    Serial.println("interrupt");
  }
  
  // Alarm Event Check
  for (int i=0; i<4; i++)
  {
    if (alarmList[i].getAMPM() != -1)
    {
      if ((h1*10 + h2) == alarmList[i].getH())
      {
        if ((m1*10 + m2) == alarmList[i].getM())
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
  if(digitalRead(screenBrowseButton) == HIGH)
  {
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

  // TODO: Recalculate delay
  delay(75);
  if(ultimateCount % 4 == 0)
  {
    oldLogicUpdation();
  }
  ultimateCount++;
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
      if (key == "hr")
      {
        String tempHour = value;
        h1 = int(tempHour[0]-48);
        h2 = int(tempHour[1]-48);
        ackHr = 1;
      }
      else if (key == "min")
      {
        String tempMin = value;
        m1 = int(tempMin[0]-48);
        m2 = int(tempMin[1]-48);
        ackMin = 1;
      }
      else if (key == "sec")
      {
        String tempSec = value;
        s1 = int(tempSec[0]-48);
        s2 = int(tempSec[1]-48);
        ackSec = 1;
      }
      else if (key == "noa")
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
  String disptime = "          ";
  disptime[0] = h1+48;
  disptime[1] = h2+48;
  disptime[2] = ':';
  disptime[3] = m1+48;
  disptime[4] = m2+48;
  disptime[5] = ':';
  disptime[6] = s1+48;
  disptime[7] = s2+48;
  if(ampm==0)
    disptime[8] = 'A';
  else
    disptime[8] = 'P';
  disptime[9] = 'M';
  display.print(disptime);
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
  if (ackHr == 0 || ackMin == 0 || ackSec == 0)
  {
    Serial.println("ackTime");
  }
  else if (ackNOA == 0 || ackH0 == 0 || ackM0 == 0 || ackS0 == 0 || ackAMPM0 == 0)
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

void oldLogicInitialization()
{
  ampm=0;
  if (h1==0 && h2==0)
  {
    h1=1;
    h2=2;
    ampm = 0;
  }
  else if(h1*10+h2>12)
  {
    ampm=1;
    if (h2>=2)
    {
      h2-=2;
      h1-=1;
    }
    else
    {
      h2+=8;
      h1-=2;
    }
  }
}

void oldLogicUpdation()
{
  if (s2==9){
    s2 = 0;
    if (s1!=5){
      s1+=1;
    }else{
      s1 = 0;
      if (m2!=9){
        m2+=1;
      }else{
        m2=0;
        if(m1!=5){
          m1+=1;
        }else{
          m1=0;
          if (h1*10+h2!=12){
            ampm=((-1)*ampm) +1;
            if(h2!=9){
              h2+=1;
            }else{
              h2=0;
              h1+=1;
            }
          }else{
            h2=1;
            h1=0;
          }
        }
      }
    }
  }
  else{
    s2+=1;
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
  Serial.print(h1);
  Serial.print(h2);
  Serial.print(":");
  Serial.print(m1);
  Serial.print(m2);
  Serial.print(":");
  Serial.print(s1);
  Serial.println(s2);
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

