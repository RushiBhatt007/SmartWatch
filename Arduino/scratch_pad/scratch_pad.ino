int buttonFive = 5;
int buttonTwo = 2;

int count;

int readButtonFive;

void interruptFunction()
{
  Serial.println("Interrupt Happened");
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  count = 0;
  pinMode(buttonFive, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonTwo), interruptFunction, CHANGE);
}

void loop() 
{
   readButtonFive = digitalRead(buttonFive);
   Serial.println(count);
   count++;
   delay(10000);
}
