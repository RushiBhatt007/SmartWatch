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

int virtual_high = 5;
int virtual_low = 4;

// Talkie output at Digital 3
// Display pins at SDA = A4, SCL = A5

void setup() 
{
  Serial.begin(9600);
  pinMode(virtual_high, HIGH);
  pinMode(virtual_low, LOW);
  pinMode(speak, INPUT);
  pinMode(vibr, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
}

void loop() 
{
  /*
  if(Serial.available())
  {
    char s = Serial.read();
    Serial.println(s);
  }
  */
  display.setCursor(0,0);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print('1');
}
