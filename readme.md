WT450 Sender Library for Arduino
================================

This small library allows an Arduino to emulate the WT450 temperature and humidity sender. This is the same model as the standard Ninja Blocks temperature and humidity sensor.

By default the transmitter data pin should be connected to the Arduino digital pin 9.

Use the sendWT450Packet to send a custom packet specifying:

- House Code (1 to 15)
- Channel (1 to 4)
- Humidity (0 to 127)
- Temperature (floating point 0 to 205)


WT450 Emulator

Send WT450 protocol packets through an RF transmitter.

Thanks to Jaakko Ala-Paavola and colleagues for sharing
the protocol information and packet content for the WT450.

The following information is an abbreviated version of material from
http://ala-paavola.fi/jaakko/doku.php?id=wt450h

+---+   +---+   +-------+       +  high
|   |   |   |   |       |       |
|   |   |   |   |       |       |
+   +---+   +---+       +-------+  low

^       ^       ^       ^       ^  clock cycle
|   1   |   1   |   0   |   0   |  translates as

Each transmission is 36 bits long (i.e. 72 ms)

Example transmission (House 1, Channel 1, RH 59 %, Temperature 23.5 °C)
110000010011001110110100100110011000

b00 - b03  (4 bits): Constant, 1100, probably preamble
b04 - b07  (4 bits): House code (here: 0001 = HC 1)
b08 - b09  (2 bits): Channel code - 1 (here 00 = CC 1)
b10 - b12  (3 bits): Constant, 110
b13 - b19  (7 bits): Relative humidity (here 0111011 = 59 %)
b20 - b34 (15 bits): Temperature (see below)
b35 - b35  (1 bit) : Parity (xor of all bits should give 0)

Summary of bit fields:
1100 0001 00 110 0111011 010010011001100 0
 c1   hc  cc  c2    rh          t        p

c1, c2 = constant field 1 and 2
hc, cc = house code and channel code
rh, t  = relative humidity, temperature
p      = parity bit

The temperature is transmitted as (temp + 50.0) * 128,

-------------------------------------------------------------------
Some additional things I've discovered from sampling transmissions:
The parity description didn't make sense to me so after some testing
it turns out that it's a simple even parity bit. This has the added
benefit of leaving the transmission in a low state at the end of the
final bit. To 'complete' the packet, a 500 μs pulse is sent.
I also believe the 1100 preamble could be used to allow devices to 
determine the pulse durations for long and short pulses.



Acknowledgements
----------------
Thanks to Jaakko Ala-Paavola for the work he and his colleagues did in decoding and publishing the WT450 protocol and also for allowing me to include those details in this utility.