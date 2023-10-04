/*
PPP Basic Arduino Input/Output Circuit Demo

pinout diagram: https://docs.arduino.cc/static/d77fe4114922e80804b6ebfd2e586316/ABX00027-pinout.png

Materials:
Arduino Nano 33 IoT
2-pin LED
4-pin RGB LED
4-pin push buttons (4)
10k ohm potentiometer
220 or 430 ohm resistors (4)
jumper cables

*/

//
//PIN DECLARATIONS
//const means constant (can't change)
//int means integer (whole number corresponding to digital pin on the arduino, or analog pin (A_))
//
const int redLEDPin = 10;
const int greenLEDPin = 11;
const int blueLEDPin = 12;
const int blueButtonPin = 14;
const int greenButtonPin = 15;
const int redButtonPin = 16;
const int otherButtonPin = 17;
const int otherLEDPin = 19;
const int potPin = A7;

//
//variables
//
int potVal = 0; //stores the value of our potentiometer knob
int redVal = 0; 
int blueVal = 0;
int greenVal = 0;
int otherVal = 0;


void setup() {
  //set up Serial communication so we can print debug messages
  Serial.begin(9600);
  
  //set up each pin as either input or output
  //INPUT_PULLUP uses an internal "pull-up" resistor
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(blueButtonPin, INPUT_PULLUP);
  pinMode(greenButtonPin, INPUT_PULLUP);
  pinMode(redButtonPin, INPUT_PULLUP);
  pinMode(otherButtonPin, INPUT_PULLUP);
  pinMode(otherLEDPin, OUTPUT);
  // pinMode(potPin, INPUT); //don't need to set up mode for pot because analog input
}

void loop() {
  //read the value of the potentiometer
  potVal = analogRead(potPin);
  //use the Serial Monitor to display debug messages (in this case, the current reading of our pot)
  Serial.println(potVal);
  //"map" the value of the pot to a range we want to work with 0-1023 (range of analog input) to 0-255 (color range)
  potVal = map(potVal, 0, 1023, 0, 255);
  
  /*
  //Button LED Control
  //reset all colors at beginning, then check if buttons are pressed to "add" the pot value from above
  redVal = 0;
  greenVal = 0;
  blueVal = 0;
  otherVal = 0;
  if (digitalRead(redButtonPin) == LOW){ //we're reading LOW because we're using INPUT_PULLUP, so connection readings are inverted (normally open)
    redVal = potVal;
  }
  if (digitalRead(blueButtonPin) == LOW){
    blueVal = potVal;
  }
  if (digitalRead(greenButtonPin) == LOW){
    greenVal = potVal;
  }
  if (digitalRead(otherButtonPin) == LOW){
    otherVal = potVal;
  }
  setColor(redVal, greenVal, blueVal);
  digitalWrite(otherLEDPin, otherVal);
  */

  
  //Color Cycle Test
  setColor(255, 0, 0);
  delay(potVal);
  setColor(0, 255, 0);
  delay(potVal);
  setColor(0, 0, 255);
  delay(potVal);
  setColor(255, 255, 0);
  delay(potVal);
  setColor(255, 255, 255);
  delay(potVal);
  setColor(0, 0, 0);
  delay(potVal);
  
  
  //delay is good for giving the code time to respond to the signals
  delay(20);
}

void setColor(int redVal, int greenVal, int blueVal){
  analogWrite(redLEDPin, redVal);
  analogWrite(greenLEDPin, greenVal);
  analogWrite(blueLEDPin, blueVal);
}
