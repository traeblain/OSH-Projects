#include <Servo.h>  //Need the Servo module

Servo servo_obj; //Define the Servo Object

const int buttonPin = 16; //Using Pin A1, so it's numerically pin 16
const int ledPin = 13;  //On-board LED
const int setAngle = 125;  //The angle required to fully open your blinds

//Using the Monoprice Curtain Module http://www.monoprice.com/product?p_id=11992
//The OPEN & CLOSED Signal outputs are wired into these pins.
//Pins are logically reversed.
const int ZWaveClosedPin = 11;
const int ZWaveOpenPin = 12;

// VARIABLES
int buttonState;
int angle = 0;
int ZWaveClosed = 0;
int ZWaveOpen = 0;

void setup() {
  //SETUP Requirements
  servo_obj.attach(0); //Attaching the Servo signal line to Pin 0
  servo_obj.write(angle); //On startup close the blinds (This is important due to possible power outages.)
  //Setup the pinModes for each contact/logic gate.
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT); 
  //Z-Wave Pins
  pinMode(ZWaveClosedPin, INPUT);
  pinMode(ZWaveOpenPin, INPUT);
}

void loop() {
  //Read the state of the push button
  buttonState = digitalRead(buttonPin);

  //Check if button is pushed (using pull-down resistor so looking for HIGH voltage).
  if (buttonState == HIGH) {
    delay(250);  //Button needs to be held 1/4 second before proceeding.
    //This is just basic debouncing, keeps from blinds moving unexpectedly.
    if (digitalRead(buttonPin) == HIGH && angle == 0) {
      //This section is when the button was properly pushed and held, and the blinds are closed.
      digitalWrite(ledPin, HIGH); //Turn on the LED
      angle = setAngle; //Make angle variable the open amount (used later).
      servo_obj.write(angle); //Tell the servo to move that amount.
      delay(500); //Half second delay allows the servo to move to fully open before anything else can be done.
    } else if (digitalRead(buttonPin) == HIGH && angle == setAngle) {
      //This section is when the button was properly pushed and held, and the blinds are open.
      digitalWrite(ledPin, LOW); //Turn off the LED
      angle = 0; //Make angle variable the closed position
      servo_obj.write(angle); //Tell the servo to move that amount.
      delay(500); //Half second delay allows the servo to move to fully closed before anything else can be done.
    }
  }


//Z-Wave section
//Monoprice module is a basic NPN Transistor, simply look for a low (runs for ~500-1000milliseconds)
  ZWaveClosed = digitalRead(ZWaveClosedPin); //Read the "OPEN Signal Output" from module
  ZWaveOpen = digitalRead(ZWaveOpenPin);  //Read the "CLOSED Signal Output" from module
  if (ZWaveClosed == LOW) {
    //When asked to be closed, do the same as defined above
    digitalWrite(ledPin, LOW);
    angle = 0;
    servo_obj.write(angle);
    delay(500);
  }
  if (ZWaveOpen == LOW) {
    //When asked to be opened, do the same as defined above
    digitalWrite(ledPin, HIGH);
    angle = setAngle;
    servo_obj.write(angle);
    delay(500);
  }
//End of Loop
}
