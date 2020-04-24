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

  String getKey()
  {
    return key;
  }

  String getValue()
  {
    return value;
  }

  void setKey(String key1)
  {
    key = key1;
  }

  void setValue(String value1)
  {
    value = value1;
  }
};

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
int h1=0, h2=0, m1=0, m2=0, s1=0, s2=0, ampm=0;
int h, m, s; 


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

int currentScreen = 0;

int triggerSOSButton = 2; // ISR can be pin 2 or 3

int ultimateCount = 0;

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
      fetchVariables();
      //initializeAllVariables();
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

  delay(75);
  if(ultimateCount % 4 == 0)
  {
    oldLogicUpdation();
  }
  ultimateCount++;
}

void fetchVariables()
{
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      Serial.println("Fetch Done");
      printRoutine();
      return;
    }
    if (c == '{')
    {
      keyValue fetch = extractKeyValue();
      Serial.println("key: "+ fetch.getKey());
      Serial.println("value: "+ fetch.getValue());
      if (fetch.getKey() == "hr")
      {
        String tempHour = fetch.getValue();
        h1 = int(tempHour[0]-48);
        h2 = int(tempHour[1]-48);
      }
      else if (fetch.getKey() == "min")
      {
        String tempMin = fetch.getValue();
        m1 = int(tempMin[0]-48);
        m2 = int(tempMin[1]-48);
      }
      else if (fetch.getKey() == "sec")
      {
        String tempSec = fetch.getValue();
        s1 = int(tempSec[0]-48);
        s2 = int(tempSec[1]-48);
      }
      else if (fetch.getKey() == "noa")
      {
        numberOfAlarms = fetch.getValue().toInt();
      }
      else if (fetch.getKey() == "t0")
      {
        alarmeList[0].setTme(fetch.getValue());
      }
      else if (fetch.getKey() == "m0")
      {
        alarmeList[0].setMsg(fetch.getValue());
      }
      else if (fetch.getKey() == "t1")
      {
        alarmeList[1].setTme(fetch.getValue());
      }
      else if (fetch.getKey() == "m1")
      {
        alarmeList[1].setMsg(fetch.getValue());
      }
      else if (fetch.getKey() == "t2")
      {
        alarmeList[2].setTme(fetch.getValue());
      }
      else if (fetch.getKey() == "m2")
      {
        alarmeList[2].setMsg(fetch.getValue());
      }
      else if (fetch.getKey() == "t3")
      {
        alarmeList[3].setTme(fetch.getValue());
      }
      else if (fetch.getKey() == "m3")
      {
        alarmeList[3].setMsg(fetch.getValue());
      }
      else if (fetch.getKey() == "vo")
      {
        volume = fetch.getValue();
      }
      else if (fetch.getKey() == "vi")
      {
        vibration = fetch.getValue();
      }
    }
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
      // Old logic for initializtion
      oldLogicInitialization();
      printRoutine();
      return;
    }
    if(c == '{')
    {
      String s = tokenizeWord();
      if(count == 0)
      {
        myTime1 = s;
        h1 = int(myTime1[0]-48);
        h2 = int(myTime1[1]-48);
      }
      else if(count == 1)
      {
        myTime2 = s;
        m1 = int(myTime2[0]-48);
        m2 = int(myTime2[1]-48);
      }
      else if(count == 2)
      {
        myTime3 = s;
        s1 = int(myTime3[0]-48);
        s2 = int(myTime3[1]-48);
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
  Serial.print(h1);
  Serial.println(h2);
  Serial.print(m1);
  Serial.println(m2);
  Serial.print(s1);
  Serial.println(s2);
  Serial.println(numberOfAlarms);
  for (int i=0; i<4; i++)
  {
    Serial.println(alarmeList[i].getTme());
    Serial.println(alarmeList[i].getMsg());
  }
  Serial.println(volume);
  Serial.println(vibration);
}

