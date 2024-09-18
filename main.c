#include <LiquidCrystal.h>
LiquidCrystal lcd (8, 9, 4, 5, 6, 7);
const int contrastPin = 2; // controls LCD contract
const int greenLED = 10; // green LED
const int redLED = 11; // red LED 
const int gas = A1; // left button
const int shift = A0; // right button

// Initialize car and flag sprites
byte sprite1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01100,
  B11111,
  B01010
};

byte sprite2[8] = {
  B00000,
  B00000,
  B10101,
  B01011,
  B10101,
  B00001,
  B00001,
  B00001
};

byte car = 0;
byte flag = 1;

// State of gas and gear buttons
int gasState = 0;
int shiftState = 0;

// Speed
int speed = 0;
char charSpeed[10];

// Gear
int gear = 1;
char charGear[10];

// Distance
float distance = 0.0;
float addOn = 0.0;
char charDistance[10];

// Time
int startTime;
int elapsedTime;
char charSecs[10];
char charMillis[10];

bool gameStarted = false;
bool gameFinished = false;
bool countdownStarted = false;

bool drawCar(byte car, float distance) {
  lcd.clear();
  if (distance < 100) lcd.setCursor(3, 1);
  else if (distance >= 100 && distance < 200) lcd.setCursor(4, 1);
  else if (distance >= 200 && distance < 300) lcd.setCursor(5, 1);
  else if (distance >= 300 && distance < 400) lcd.setCursor(6, 1);
  else if (distance >= 400 && distance < 500) lcd.setCursor(7, 1);
  else if (distance >= 500 && distance < 600) lcd.setCursor(8, 1);
  else if (distance >= 600 && distance < 700) lcd.setCursor(9, 1);
  else if (distance >= 700 && distance < 800) lcd.setCursor(10, 1);
  else if (distance >= 800 && distance < 900) lcd.setCursor(11, 1);
  else if (distance >= 900 && distance < 1000) lcd.setCursor(12, 1);
  if (distance >= 1000) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("GAME FINISHED");
    return true;
  }
  lcd.write(car);
  return false;
}

// Countdown at start of game
void countdown() {
  lcd.clear();

  lcd.setCursor(7, 0);
  lcd.print("3");
  digitalWrite(redLED, HIGH);
  delay(800);
  digitalWrite(redLED, LOW);
  delay(200);

  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("2");
  digitalWrite(redLED, HIGH);
  delay(800);
  digitalWrite(redLED, LOW);
  delay(200);

  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("1");
  digitalWrite(redLED, HIGH);
  delay(800);
  digitalWrite(redLED, LOW);
  delay(200);

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("GO!");
  digitalWrite(greenLED, HIGH);
  delay(400);
  digitalWrite(greenLED, LOW);
  delay(100);

  lcd.clear();
}

void setup() {
  lcd.begin(16, 2);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(gas, INPUT);
  pinMode(shift, INPUT);
  analogWrite(contrastPin, 100);

  // Create custom sprites
  lcd.createChar(car, sprite1);
  lcd.createChar(flag, sprite2);
  lcd.clear();

  Serial.begin(9600);
}

void loop() {

  gasState = digitalRead(gas);
  shiftState = digitalRead(shift);
  
  // Keep menu displayed until button is pressed
  if (!gameStarted) {
    // Show the menu at start
    lcd.setCursor(2, 0);
    lcd.print("GEAR SHIFTER");
    lcd.setCursor(0, 1);
    lcd.print("PRESS L 2 START!");
    delay(10);
  }

  // Start countdown on first gas press
  if (!countdownStarted && gasState == HIGH) {
    lcd.clear();
    countdownStarted = true;
    countdown();
    speed = 0;
    gear = 1;
    gameStarted = true;
    startTime = millis();
  }

  // Display speed, gear, time, car, and flag
  if (gameStarted && !gameFinished) {

    elapsedTime = millis() - startTime;

    // CLEAN UP !!!!!!!!!!!!!!!!
    // Calculate Distance and Draw Car

    int newTime = (elapsedTime / 1000);
    addOn = (speed * newTime);
    distance += addOn;
    gameFinished = drawCar(car, distance);

    lcd.setCursor(0, 0);
    sprintf(charSpeed, "%d", speed);
    lcd.write(charSpeed);

    lcd.setCursor(14, 0);
    sprintf(charGear, "%d", gear);
    lcd.write(charGear);
    
    int seconds = elapsedTime / 1000;
    int milliseconds = elapsedTime % 1000;
    sprintf(charSecs, "%d", seconds);
    if (seconds < 10) lcd.setCursor(6, 0);
    if (seconds >= 10) lcd.setCursor(5, 0);
    lcd.print(charSecs);

    lcd.setCursor(7, 0);
    lcd.print(".");

    sprintf(charMillis, "%03d", milliseconds);
    lcd.setCursor(8, 0);
    lcd.print(charMillis);

    lcd.setCursor(13, 1);
    lcd.write(flag);


    gasState = digitalRead(gas);
    shiftState = digitalRead(shift);
  }

  if (gameFinished) {
    lcd.clear();

    // Calculate final time
    int finalTime = elapsedTime;
    int finalSeconds = elapsedTime / 1000;
    int finalMilliseconds = elapsedTime % 1000;
    sprintf(charSecs, "%d", finalSeconds);
    if (finalSeconds < 10) lcd.setCursor(6, 1);
    if (finalSeconds >= 10) lcd.setCursor(5, 1);
    lcd.print(charSecs);

    lcd.setCursor(7, 1);
    lcd.print(".");

    sprintf(charMillis, "%03d", finalMilliseconds);
    lcd.setCursor(8, 1);
    lcd.print(charMillis);

    lcd.setCursor(3, 0);
    lcd.print("FINAL TIME");
  }

  gasState = digitalRead(gas);
  shiftState = digitalRead(shift);

  // Gas is pressed
  if (gasState == HIGH)
  {
    //digitalWrite(greenLED, HIGH);
    lcd.setCursor(0, 0);
    speed += 1;
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
  delay(100);
}
