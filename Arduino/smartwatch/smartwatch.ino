#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

class alarme
{
  public:
  String tme, msg;
  
  alarme(){
  }

  alarme(String tme1, String msg1)
  {
    tme = tme1;
    msg = msg1;
  }

  void setTme(String tme1)
  {
    tme = tme1;
  }

  void setMsg(String msg1)
  {
    msg=msg1;
  }
  
  String getTme()
  {
    return tme;
  }

  String getMsg()
  {
    return msg;
  }
};


class alarm
{
  public:
  int h, m, ampm;
  String msg;
  
  alarm(){
  }

  alarm(int h1, int m1, int ampm1, String msg1)
  {
    h = h1;
    m = m1;
    ampm = ampm1;
    msg = msg1;
  }

  int getH()
  {
    return h;
  }
  
  int getM()
  {
    return m;
  }

  int getAmpm()
  {
    return ampm;
  }

  String getMsg()
  {
    return msg;
  }
};

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

char* tim;
int h1, h2, m1, m2, s1, s2, ampm;
int h, m, s; 
char* disptime="";


// Variables
alarme alarmeList[4];
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

int triggerSOSButton = 2; // ISR can be pin 2 or 3

// Talkie output at Digital 3
// Display pins at SDA = A4, SCL = A5

void setup() 
{
  Serial.begin(9600);
  pinMode(speak, INPUT);
  pinMode(vibr, INPUT);
  pinMode(screenBrowseButton, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.display();
  display.clearDisplay();
  attachInterrupt(digitalPinToInterrupt(triggerSOSButton), triggerSOSInterrupt, CHANGE);
  showTimeScreen();
  
  for (int i=0; i<4;i++)
    alarmeList[i] = alarme("Test", "Test");
}

void loop() 
{
  // Listen to Changes on App
  if(Serial.available())
  {
    char c = Serial.read();
    if (c == '~')
    {
      initializeAllVariables();
    }
    else if(c == '!')
    {
      addAlarm();
    }
    else if(c == '@')
    {
      deleteAlarm();
    }
    else if(c == '#')
    {
      updateVolume();
    }
    else if(c == '$')
    {
      updateVibration();
    }
  }

  // TODO: Trigger SOS Button
  // TODO: Buzz on Alarm Event
  
  // Browse Screen
  if(digitalRead(screenBrowseButton) == HIGH)
  {
    readScreenBrowseButton++;
    if(readScreenBrowseButton%6 == 0)
      showTimeScreen();
    else if(readScreenBrowseButton%6 == 1)
      showSVScreen(volume.toInt(),vibration.toInt());
    else if(readScreenBrowseButton%6 == 2)
      showAlarmListScreen();
    else if(readScreenBrowseButton%6 == 3)
      showAlarmScreen(alarmeList[readScreenBrowseButton%4]);
    else if(readScreenBrowseButton%6 == 4)
      showFMWScreen();
    else if(readScreenBrowseButton%6 == 5)
      showFMWScreen();
      //showSOSScreen();
  }
}

void initializeAllVariables()
{
  String entireMsg = "";
  int count = 0;
  Serial.print("IV\n");
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      // Works Well
      printRoutine();
      return;
    }
    if(c == '{')
    {
      String s = tokenizeWord();
      if(count == 0)
      {
        myTime1 = s;
      }
      else if(count == 1)
      {
        myTime2 = s;
      }
      else if(count == 2)
      {
        myTime3 = s;
      }
      else if(count == 3)
      {
        numberOfAlarms = s.toInt();
      }
      else if(count == (4 + 2*numberOfAlarms))
      {
        volume = s;
      }
      else if(count == (4 + 2*numberOfAlarms + 1))
      {
        vibration = s;
      }
      else
      {
        if (count%2 == 0)
        {
          alarmeList[(count/2)-1].setTme(s);  
        }
        else if(count%2 == 1)
        {
          alarmeList[(count-3)/2].setMsg(s);
        }
      }
      count++;
      entireMsg = entireMsg + s;
    }
  }
}

void addAlarm()
{
  Serial.println("A.A");
  int count = 0;
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      printRoutine();
      return;
    }
    if(c == '{')
    {
      String s = tokenizeWord();
      if (count == 0)
      {
        numberOfAlarms = s.toInt();
      }
      else if (count == 1)
      {
        alarmeList[numberOfAlarms-1].setTme(s);
      }
      else if (count == 2)
      {
        alarmeList[numberOfAlarms-1].setMsg(s);        
      }
      count++;
    }
  }  
}

void deleteAlarm()
{
  Serial.println("D.A.");
  int count = 0;
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      alarmeList[3].setTme("Testing");
      alarmeList[3].setMsg("Testing");
      printRoutine();
      return;
    }
    if(c == '{')
    {
      String s = tokenizeWord();
      if (count == 0)
      {
        numberOfAlarms = s.toInt();
      }
      else 
      {
        if (count%2 == 1)
        {
          alarmeList[(count-1)/2].setTme(s);  
        }
        else if(count%2 == 0)
        {
          alarmeList[(count-2)/2].setMsg(s);
        }
      }
      count++;
    }
  }  
}

void updateVolume()
{
  Serial.println("Up.Vol.");
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      printRoutine();
      return;
    }
    if(c == '{')
    {
      String s = tokenizeWord();
      volume = s;
    }
  }
}

void updateVibration()
{
  Serial.println("Up.Vib.");
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      printRoutine();
      return;
    }
    if(c == '{')
    {
      String s = tokenizeWord();
      vibration = s;
    }
  }
}

void showTimeScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("18:25 PM");
  display.display();
}

void showSOSScreen()
{
  int i;
  for(i=5;i>=0;i--)
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

void showAlarmScreen(alarme alarme1)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(12,4);
  display.println(alarme1.getTme());
  display.setTextSize(1);
  display.setCursor(10,22);
  display.println(alarme1.getMsg());
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
    char c = '1'+i;
    String j;
    j.concat(c);
    display.println("Alarm "+j+" - "+alarmeList[i].getTme());
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
  for(i=1;i<=53
  ;i++)
  {
    int colorShow = vibration>=i?WHITE:BLACK;
    display.fillRect(x_start, y_start, rect_width, rect_height, colorShow);
    x_start = x_start + rect_width + 2;
  }
  display.display();
}

void triggerSOSInterrupt()
{
  showSOSScreen();
}

String tokenizeWord()
{
  String tokenizedWord = "";
  //Serial.print("\n");
  while(true)
  {
    char ch = Serial.read();
    if(ch == '}')
    {
      //Serial.println(tokenizedWord);
      return tokenizedWord;
    }
    
    // Upper Case || Lower Case || Numbers || space || colon || comma
    if((ch>=65 && ch<=90) || (ch>=97 && ch<=122) || (ch>=48 && ch<=57) || ch==32 || ch==58 || ch==44)
    {
      tokenizedWord = tokenizedWord + ch;
      //Serial.print(ch); 
    }
  }
}

void printRoutine()
{
  Serial.println("Hr: "+myTime1);
  Serial.println("Min: "+myTime2);
  Serial.println("Sec: "+myTime3);
  Serial.println(numberOfAlarms);
  Serial.println(alarmeList[0].getTme());
  Serial.println(alarmeList[0].getMsg());
  Serial.println(alarmeList[1].getTme());
  Serial.println(alarmeList[1].getMsg());
  Serial.println(alarmeList[2].getTme());
  Serial.println(alarmeList[2].getMsg());
  Serial.println(alarmeList[3].getTme());
  Serial.println(alarmeList[3].getMsg());
  Serial.println(volume);
  Serial.println(vibration);
}

