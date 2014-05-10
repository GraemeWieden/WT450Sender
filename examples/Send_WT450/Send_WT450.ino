#include <WT450Sender.h>

// set up the hardware pins
byte txPin = 4; // digital pin connected to the RF transmitter
byte ledPin = 13; // digital pin for LED
byte houseCode = 15; // from 1 to 15
byte channelCode = 4; // from 1 to 4

// construct the WT450Sender object
WT450Sender sender;

void setup()
{
  // set up the WT450Sender object
  sender.setup(txPin, houseCode, channelCode);
  
  pinMode(ledPin, OUTPUT);  
}

void loop()
{
  // create some dummy data to send
  double temperature = 23.4;
  byte humidity = 56;
  
  // send the data
  sender.send(humidity, temperature);
  
  // blink the led to let us know when data is sent
  blinkLed();
  
  delay(10000); // delay for 10 seconds
}

void blinkLed()
{
  digitalWrite(ledPin, HIGH);
  delay(30);   
  digitalWrite(ledPin, LOW);
}

