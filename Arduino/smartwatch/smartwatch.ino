#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

class alarm
{
  public:
  int h, m, ampm;

  alarm(){
  }

  alarm(int h1, int m1, int ampm1)
  {
    h = h1;
    m = m1;
    ampm = ampm1;
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
};

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

char* tim;
int h1, h2, m1, m2, s1, s2, ampm;
int h, m, s; 
char* disptime="";

int speak = 6;  // Button for speak
int vibr = 7;   // Button for vibrator
int PWMPin = 9; // Output of PWM

int screenBrowseButton = 5;
int readScreenBrowseButton = 0;

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
  showTimeScreen();
}

void loop() 
{
  //Serial.println(digitalRead(screenBrowseButton));
  if(digitalRead(screenBrowseButton) == HIGH)
  {
    readScreenBrowseButton++;
    if(readScreenBrowseButton%6 == 0)
      showTimeScreen();
    else if(readScreenBrowseButton%6 == 1)
      showSVScreen(2,2);
    else if(readScreenBrowseButton%6 == 2)
      showAlarmListScreen();
    else if(readScreenBrowseButton%6 == 3)
      showAlarmScreen();
    else if(readScreenBrowseButton%6 == 4)
      showFMWScreen();
    else if(readScreenBrowseButton%6 == 5)
      showSOSScreen();
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

void showAlarmScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(12,4);
  display.println("12:45 AM");
  display.setTextSize(1);
  display.setCursor(10,22);
  display.println("Take acidity pills");
  display.display();
}

void showAlarmListScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Alarm 1 - 12:45 AM");
  display.println("Alarm 2 - 01:20 PM");
  display.println("Alarm 3 - 12:45 AM");
  display.println("Alarm 4 - 01:20 PM");
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
    int colorShow = volume>=i?WHITE:BLACK;
    display.fillRect(x_start, y_start, rect_width, rect_height, colorShow);
    x_start = x_start + rect_width + 2;
  }
  display.display();
}

