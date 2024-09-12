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
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  // int potValue = analogRead(potPin);
  analogWrite(contrastPin, 100);
  // int ledValue = map(potValue, 0, 1024, 0, 255);
  lcd.print("Progress 01");
  lcd.setCursor(0, 1);
  lcd.print("Hack Yeah");

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, HIGH);

}
