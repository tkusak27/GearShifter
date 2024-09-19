#include <LiquidCrystal.h>
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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
const double vMax[] = {38.88, 69.02, 103.62, 135.52, 159.78, 188.87}; // in MPH


// Acceleration
double accelr8[6] = {
  13.500000,
  5.886719,
  4.499350,
  3.174129,
  2.047257,
  2.076374
};

// Deceleration
const double decelr8 = 6.82; //MPH per second

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
float newTime;
char charSecs[10];
char charMillis[10];

bool gameStarted = false;
bool gameFinished = false;
bool countdownStarted = false;

bool drawCar(byte car, float distance) {
  lcd.clear();
  if (distance < 10000) lcd.setCursor(3, 1);
  else if (distance >= 50000 && distance < 10000) lcd.setCursor(4, 1);
  else if (distance >= 10000 && distance < 15000) lcd.setCursor(5, 1);
  else if (distance >= 15000 && distance < 20000) lcd.setCursor(6, 1);
  else if (distance >= 20000 && distance < 25000) lcd.setCursor(7, 1);
  else if (distance >= 25000 && distance < 30000) lcd.setCursor(8, 1);
  else if (distance >= 30000 && distance < 35000) lcd.setCursor(9, 1);
  else if (distance >= 35000 && distance < 40000) lcd.setCursor(10, 1);
  else if (distance >= 40000 && distance < 45000) lcd.setCursor(11, 1);
  else if (distance >= 45000 && distance < 50000) lcd.setCursor(12, 1);
  if (distance >= 50000) {
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

double calcSpeed(int speed, int g, float t){

      speed += accelr8[g - 1] * t;
      return MIN(speed, vMax[g - 1]);
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

    newTime = (elapsedTime / 1000);
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
    speed = calcSpeed(speed, gear, newTime);
  }
  else if (speed > 0)
  {
    speed -= 6.82;
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
