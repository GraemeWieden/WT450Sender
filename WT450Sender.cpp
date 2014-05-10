/*
WT450 Sender Library for Arduino
================================

For the latest version, visit the project home at:
https://github.com/graemewieden

For technical details see readme.md

Released under the BSD license. 
See license.md for more information.
*/

#include "WT450Sender.h"

WT450Sender::WT450Sender()
{
  // Default setup txPin set to 2, House and Channel codes set to 1
  setup(2, 1, 1);
}

WT450Sender::WT450Sender(byte txPin, byte houseCode, byte channel)
{
  setup(txPin, houseCode, channel);
}

void WT450Sender::setup(byte txPin, byte houseCode, byte channel)
{
  setRepeats(3);
  setTxPin(txPin);
  setHouseAndChannel(houseCode, channel);
}

void WT450Sender::setTxPin(byte txPin)
{
  this->txPin = txPin;
}

void WT450Sender::setHouseAndChannel(byte houseCode, byte channel)
{
  this->houseCode = houseCode;
  this->channel = channel;
}

void WT450Sender::setRepeats(byte repeats)
{
  this->repeats = repeats;
}

void WT450Sender::send(byte humidity, double temperature)
{
  unsigned int convertedTemperature = (temperature * 128.0) + 6400;
  byte convertedChannel = channel - 1;

  for (byte i = 0; i < repeats; i++)
  {
    sendWT450Start();
    sendWT450Bits(B1100, 4); // always 1100
    sendWT450Bits(houseCode, 4); // house 1 stored as 0001
    sendWT450Bits(convertedChannel, 2); // channel 1 stored as 00
    sendWT450Bits(B110, 3); // always 110
    sendWT450Bits(humidity, 7); // humidity
    sendWT450Bits(convertedTemperature, 15); // encoded temperature
    sendWT450Bits(parityBit, 1); // parity
    sendWT450End();
  }
}

void WT450Sender::sendWT450Start()
{
  parityBit = 0;
  digitalWrite(txPin, LOW);
  delayMicroseconds(TRIGGERPULSE);
  delayMicroseconds(TRIGGERPULSE); // send twice as delayMicroseconds has a max of 16383
  pulseState = HIGH; // next pulse state to use
}

void WT450Sender::sendWT450End()
{
  // if the last pulse was low (next is high) we need to send a half short high pulse to complete the packet
  // note that this should always be the case because of the even parity bit
  if (pulseState == HIGH)
  {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(SHORTPULSE >> 1);
  }
  digitalWrite(txPin, LOW);
  delayMicroseconds(TRIGGERPULSE);
  delayMicroseconds(TRIGGERPULSE);
}

void WT450Sender::sendWT450Bits(unsigned int data, byte bits)
{
  unsigned int bitMask = 1;
  bitMask = bitMask << (bits - 1);
  for (int i = 0; i < bits; i++)
  {
    sendWT450Bit((data&bitMask) == 0 ? 0 : 1);
    bitMask = bitMask >> 1;
  }
}

void WT450Sender::sendWT450Bit(byte b)
{
  // single long pulse = '0'
  if (b == 0)
  {
    digitalWrite(txPin, pulseState);
    delayMicroseconds(LONGPULSE);
    pulseState = !pulseState;
  }
  // short pulse then short pulse = '1'
  else
  {
    digitalWrite(txPin, pulseState);
    delayMicroseconds(SHORTPULSE);
    digitalWrite(txPin, !pulseState);
    delayMicroseconds(SHORTPULSE);
    // pulseState will be unchanged after two pulses
    parityBit = !parityBit;
  }
}
