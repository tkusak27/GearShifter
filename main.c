#include <LiquidCrystal.h>
LiquidCrystal lcd (8, 9, 4, 5, 6, 7);
const int contrastPin = 2; // controls LCD contract
const int greenLED = 10; // green LED
const int redLED = 11; // red LED 
const int gas = A1; // left button
const int shift = A0; // right button


void setup() {
  lcd.begin(16, 2);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(gas, INPUT);
  pinMode(shift, INPUT);
}

// State of gas and gear buttons
int gasState = 0;
int shiftState = 0;

// Speed
int speed = 0;
char charSpeed[10];

// Gear
int gear = 1;
char charGear[10];

bool gameStarted = false;


void loop() {
  if (!gameStarted) {
    
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  sprintf(charSpeed, "%d", speed);
  lcd.write(charSpeed);

  lcd.setCursor(14, 0);
  sprintf(charGear, "%d", gear);
  lcd.write(charGear);

  analogWrite(contrastPin, 100);

  gasState = digitalRead(gas);
  shiftState = digitalRead(shift);

  // Gas is pressed
  if (gasState == HIGH)
  {
    //digitalWrite(greenLED, HIGH);
    lcd.setCursor(0, 0);
    speed += 5;
  }
  else if (speed > 0)
  {
    speed--;
  }

  // Shift is pressed
  if ((shiftState == HIGH) && (gear < 6))
  {
    gear++;
    digitalWrite(redLED, HIGH);
    delay(40);
  }

  digitalWrite(redLED, LOW);
  delay(200);
}
