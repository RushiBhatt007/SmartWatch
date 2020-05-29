int buttonPin = 2;
int button2 = 5;

int button5 = 5;
int button6 = 6;
int button7 = 7;

int pressed_ms = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(button2, INPUT);

  pinMode(button5, INPUT_PULLUP);
  pinMode(button6, INPUT_PULLUP);
  pinMode(button7, INPUT_PULLUP);

}

void loop()
{
  if (digitalRead(button5) == LOW)
    Serial.println("button 5");
  if (digitalRead(button6) == LOW)
    Serial.println("button 6");
  if (digitalRead(button7) == LOW)
    Serial.println("button 7");

}

void loop2() {
  int b = checkButton();
  if (b == 1)
  {
    Serial.println("Single Click");
  }
  else if (b == 2)
  {
    Serial.println("Double Click");
  }
}



void loop1() {
  // put your main code here, to run repeatedly:
  while (digitalRead(buttonPin) == LOW)
  {
    delay(100);
    pressed_ms = pressed_ms + 100;
  }

  if(pressed_ms >= 600)
  {
    Serial.println("Long Press");
    Serial.print("ms = ");
    Serial.println(pressed_ms);
    pressed_ms = 0;
  }
  else if (pressed_ms >= 200)
  {
    Serial.print("ms = ");
    Serial.println(pressed_ms);
    Serial.println("Normal Press");
    pressed_ms = 0;
  }
}



//=================================================
//  MULTI-CLICK:  One Button, Multiple Events

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250;            // max ms between clicks for a double click event

// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
boolean DCwaiting = false;  // whether we're waiting for a double click (down)
boolean DConUp = false;     // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;    // whether it's OK to do a single click
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
boolean ignoreUp = false;   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;        // when held, whether to wait for the up event

int checkButton() 
{    
   int event = 0;
   buttonVal = digitalRead(buttonPin);
   // Button pressed down
   if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
   {
       downTime = millis();
       ignoreUp = false;
       waitForUp = false;
       singleOK = true;
       if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true)  
        DConUp = true;
       else  
        DConUp = false;
       DCwaiting = false;
   }
   // Button released
   else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
   {        
       if (not ignoreUp)
       {
           upTime = millis();
           if (DConUp == false) 
            DCwaiting = true;
           else
           {
               event = 2;
               DConUp = false;
               DCwaiting = false;
               singleOK = false;
           }
       }
   }
   // Test for normal click event: DCgap expired
   if ( buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
   {
       event = 1;
       DCwaiting = false;
   }
   buttonLast = buttonVal;
   return event;
}

