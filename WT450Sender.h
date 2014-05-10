/*
WT450 Sender Library for Arduino
================================

For the latest version, visit the project home at:
https://github.com/graemewieden

For technical details see readme.md

Released under the BSD license.
See license.md for more information.
*/

#ifndef WT450Sender_h
#define WT450Sender_h

#include "Arduino.h"

#define TRIGGERPULSE 15000 // trigger time in microseconds
#define SHORTPULSE 1000 // time in microseconds for a short pulse
#define LONGPULSE 2000 // time in microseconds for a long pulse

class WT450Sender
{
public:

  WT450Sender(); // constructor for creating with default settings
  WT450Sender(byte txPin, byte houseCode, byte channel); // constructor for custom settings
  void setup(byte txPin, byte houseCode, byte channel); // change the settings post construction
  void setTxPin(byte txPin);
  void setHouseAndChannel(byte houseCode, byte channel);
  void setRepeats(byte repeats);
  void send(byte humidity, double temperature);

protected:

  void sendWT450Start();
  void sendWT450End();
  void sendWT450Bits(unsigned int data, byte bits);
  void sendWT450Bit(byte b);

  byte txPin; // digital pin for transmitter
  byte houseCode; // valid house codes are 1 to 15
  byte channel; // valid channels are 1 to 4
  boolean pulseState; // start with a high pulse and toggle whenever we send a '0'
  boolean parityBit; // used maintain the parity state as we send bits in the packet
  byte repeats; // the actual WT450 sends three samples
};

#endif