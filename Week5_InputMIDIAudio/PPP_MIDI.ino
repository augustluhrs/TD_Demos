/*
[PPP]
Turning our basicIO circuit into a MIDI Device 

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
// LIBRARY HEADERS
//
#include "MIDIUSB.h" //the midi USB library
#include "Bounce2.h" //the button debounce library

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

const int ledPin = 13; //using on-board LED as debug tool

//
//variables
//
//potentiometer
int potVal = 0; //stores the value of our potentiometer knob
int lastPotVal = 0; //stores the last value we sent, so we only send when there's a change
int potThreshold = 5; //the amount we need to change on the pot before it registers a change (will flicker for now b/c no resistor)
//LED colors
int redVal = 0; 
int blueVal = 0;
int greenVal = 0;
int otherVal = 0;
//debug LED
int debugLED = LOW; //will flip between LOW/HIGH to toggle LED on/off (int b/c same as 0,1)

//debounce buttons
Bounce2::Button otherButton = Bounce2::Button();
Bounce2::Button redButton = Bounce2::Button();
Bounce2::Button greenButton = Bounce2::Button();
Bounce2::Button blueButton = Bounce2::Button();


void setup() {
  //set up Serial communication so we can print debug messages
  Serial.begin(9600);
  
  //set up each pin as either input or output
  //INPUT_PULLUP uses an internal "pull-up" resistor
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  // pinMode(blueButtonPin, INPUT_PULLUP);
  // pinMode(greenButtonPin, INPUT_PULLUP);
  // pinMode(redButtonPin, INPUT_PULLUP);
  // pinMode(otherButtonPin, INPUT_PULLUP);
  pinMode(otherLEDPin, OUTPUT);
  // pinMode(potPin, INPUT); //don't need to set up modabe for pot because analog input

  pinMode(ledPin, OUTPUT);
  
  //set up the debounce buttons
  otherButton.attach(otherButtonPin, INPUT_PULLUP);
  otherButton.interval(5);
  otherButton.setPressedState(LOW);
  redButton.attach(redButtonPin, INPUT_PULLUP);
  redButton.interval(5);
  redButton.setPressedState(LOW);
  greenButton.attach(greenButtonPin, INPUT_PULLUP);
  greenButton.interval(5);
  greenButton.setPressedState(LOW);
  blueButton.attach(blueButtonPin, INPUT_PULLUP);
  blueButton.interval(5);
  blueButton.setPressedState(LOW);
}

void loop() {
  
  /*
  * DEBOUNCE BUTTONS MIDI NOTE SEND
  */
  //can be whatever notes, but I'm using 59-62
  //get the current reading of each
  otherButton.update();
  redButton.update();
  greenButton.update();
  blueButton.update();
  
  //if pressed, send midi note (on then off)
  if(otherButton.pressed()){
    Serial.println("other button pressed");
    debugLED = !debugLED;
    //send MIDI note 59 (0x3B)
    sendNote(0x00, 59, 127);
    delay(50);
    sendNote(0x00, 59, 0);
  }
    if(redButton.pressed()){
    Serial.println("red button pressed");
    debugLED = !debugLED;
    //send MIDI note 60 (0x3C)
    sendNote(0x00, 60, 127);
    delay(50);
    sendNote(0x00, 60, 0);
  }
  if(greenButton.pressed()){
    Serial.println("green button pressed");
    debugLED = !debugLED;
    //send MIDI note 61 (0x3D)
    sendNote(0x00, 61, 127);
    delay(50);
    sendNote(0x00, 61, 0);
  }
  if(blueButton.pressed()){
    Serial.println("blue button pressed");
    debugLED = !debugLED;
    //send MIDI note 62 (0x3E)
    sendNote(0x00, 62, 127);
    delay(50);
    sendNote(0x00, 62, 0);
  }
  
  /*
  * POTENTIOMETER
  */
  //now checking to see if potentiometer has changed, sending CC if so
  //read the value of the potentiometer
  potVal = analogRead(potPin);
  //"map" the value of the pot to a range we want to work with 0-1023 (range of analog input) to 0-126 (MIDI Values)
  //floor means round down
  potVal = floor(map(potVal, 0, 1023, 0, 127));
  //compare to last reading and send message if it's above the threshold, then update last Val
  if (abs(potVal - lastPotVal) > potThreshold){
    //create a MIDI object and send it using the function
    // sendCC(0x91, 0x03, potVal);  //can use hex optionally
    sendCC(0x00, 3, potVal); //index 3 just b/c i like the number
    
    //toggle the LED pin so we know the message was sent without having to look at Serial Monitor
    debugLED = !debugLED;
    
    //set last to current
    lastPotVal = potVal;
    
    //print to Serial so we know it's working
    Serial.println('\n'); //just adds a newline (space)
    Serial.println("Sent CC from Pot:");
    Serial.println(potVal);
    Serial.println('\n');
    
  } else {
    //use the Serial Monitor to display debug messages (in this case, the current reading of our pot)
    // Serial.println(potVal);
  }
  
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
  
  /*
  //Color Cycle Test
  setColor(255, 0, 0);
  delay(potValue);
  setColor(0, 255, 0);
  delay(potValue);
  setColor(0, 0, 255);
  delay(potValue);
  setColor(255, 255, 0);
  delay(potValue);
  setColor(255, 255, 255);
  delay(potValue);
  setColor(0, 0, 0);
  delay(potValue);
  */
  
  //show the debug LED
  digitalWrite(ledPin, debugLED);
  
  //delay is good for giving the code time to respond to the signals, might mess with sensor readings though
  delay(10);
}

void setColor(int redVal, int greenVal, int blueVal){
  analogWrite(redLEDPin, redVal);
  analogWrite(greenLEDPin, greenVal);
  analogWrite(blueLEDPin, blueVal);
}


/*
* MIDI FUNCTIONS
* Built upon the MIDIUSB library
*/
//remember -- "Ox" is just a prefix that means we're using hexadecimal now
//if you want a hexadecimal calculator: https://learn.sparkfun.com/tutorials/hexadecimal/conversion-calculators
  
void sendNote(byte chan,  byte note, byte velocity){
  //MIDI send function that takes three arguments:
  //chan: takes command of NoteOn (0x09) and adds channel (0-15) using bitwise or (|)
  //note: when sending a Note, this is the note (21-127) (middleC is 60)
  //velocity: when sending a Note, this is the on (0x69*) or off (0x00)
  midiEventPacket_t midiMsg = {0x09, 0x90 | chan, note, velocity};
  MidiUSB.sendMIDI(midiMsg);
}

void sendCC(byte chan,  byte indexCC, byte value){
  //MIDI send function that takes three arguments:
  //cmd+chan: command of ControlChange (0x0B) and adds channel (0-15) using bitwise or (|)
  //indexCC: when sending CC, this is the index of the device paramter you're changing
  //value: when sending CC, this is the value (0-127) you want to set the paramter to
  midiEventPacket_t midiMsg = {0x0B, 0xB0 | chan, indexCC, value};
  MidiUSB.sendMIDI(midiMsg);
}
