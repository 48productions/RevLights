/*
    rev.LIGHTS

    Protocol "Reversed" (more like stumbled upon lmao) by 48


    --Protocol information:--
   
     Port settings: Rev sends light data over serial with the following settings:
       Port: COM11
       Parity: None
       Baud: 115200
       Byte size: 8
       Stop bits: 1


    Lighting data sent from the game follows this format:
    
     - To set an LED: 4 bytes are sent in the order of <id><R><G><B>
     - <id> is an LED id to write to, and R, G, and B are colors bytes for that LED

     - The coin blocker is *not* part of the lighting protocol, the Aime reader's lights don't look to be either.
     
     - 0xFF is sent after every lighting update "frame" - The id and color bytes *never* have this value (colors cap at 0xFE/254 when being sent over serial, as far as I can tell)
     
     - After sending a frame, Rev waits for serial data to be sent back before sending out the next frame.
        - This wait will time out after some time passes, sending the next frame anyways - (Around 100ish ms)
        - I don't know if the specific data sent back has any importance - just sending 0x00 once seems to work
    
     - Valid IDs (as sent by test mode):
          - 100 total IDs (0x01-0xED) are written to via "ALL LED TEST"
          - Groups of IDs are written to via individual LED zone tests, but this doesn't write to all the IDs written to in the ALL and AUTO LED TESTs
          
          - The following groups of IDs (hex values) are used. The order of LEDs in a group should (hopefully!) be correct (top -> bottom, etc).

          1) FRONT ACRYL LED - L (UPPER):  B5, B7, B9 (top -> bottom)
          2) FRONT ACRYL LED - L (LOWER):  BB, BD, BF (bottom -> top)
           
          3) FRONT ACRYL LED - R (UPPER):  CD, CF, D1 (top -> bottom)
          4) FRONT ACRYL LED - R (LOWER):  D3, D5, D7 (bottom -> top)
           
          5) REAR ACRYL LED - L (UPPER):  C1, C3, C5 (top -> bottom)
          6) REAR ACRYL LED - L (LOWER):  C7, C9, CB (bottom -> top)
           
          7) REAR ACRYL LED - R (UPPER):  D9, DB, DD (top -> bottom)
          8) REAR ACRYL LED - R (LOWER):  DF, E1, E3 (bottom -> top)
  
          9)  SIDE LED - L (TOP):     0F, 11, 13, 15, 17, 19, 1B (bottom -> top)
          10) SIDE LED - L (BOTTOM):  01, 03, 05, 07, 09, 0B, 0D (bottom -> top)
  
          11) SIDE LED - R (TOP):    2B, 2D, 2F, 31, 33, 35, 37, (kinda) 39 (bottom -> top)
          12) SIDE LED - R (BOTTOM): 1D, 1F, 21, 23, 25, 27, 29 (bottom -> top)
           
          13) WOOFER LED - L: 51, 53, 55 (left -> right)
          14) WOOFER LED - R: 57, 59, 5B (right -> left)


          ??) ID 0x39 - Part of SIDE LED - R (TOP), seems to be a duplicate/extra LED
          ??) ID 0xE5 - Written to during all/auto led tests but not specific LED groups, written to once on the title screen and pulses to beat during songs. Always light-blue, doesn't match other LED groups.
          ??) ID 0x?? - Another ID or two (probably from the "unassigned" list below) are written to during all/auto led tests but not specific LED groups, unused in gameplay

           Unassigned: 69, 6B, 6D, 6F, 71, 73, 75, 77, 79, 7B, 7D, 7F, 81, 83, 85, 87, 89, 8B, 8D, 8F,
           Unassigned: 91, 93, 95, 97, 99, 9B, 9D, 9F, A1, A3, A5, A7, A9, AB, AD, AF, B1, B3,
           Unassigned: E7, E9, EB, ED


   Random oddities:
   
     - A string of bytes are sent before the first frame, usually starting with 0xFE 0xFE. I don't know if this is important or consistent.
     - Light id EF is written on game close, but I don't *think* during the other tests? (doubt this is relevant either)
     
     - On my setup, light output will sometimes freeze within a minutes or so of boot, probably because Windows hates my guts and the Arduinos I've been using for this are sketchy as heck. YMMV.
     
*/
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>


#define STRIP_LENGTH 90 //FastLED: The length of the LED strip in use (I tested with a 90 led long strip. I only write to 67 leds, and only 58 of them are actual game-controlled lights)
#define STRIP_DATA 5 //FastLED: The pin the LED strip is attached to

CRGB leds[STRIP_LENGTH];

byte curId = 0; //Internal ID of the light we're setting right now
byte curRed = 0; //The red, green, and blue values for the light we're setting
byte curGreen = 0;
//byte curBlue = 0;
short bytesToRead = 0; //Number of data bytes we still need to read for this LED
int ledNum = 0;

void setup() {
  //Initialize LEDs and serial
  FastLED.addLeds<WS2812B, STRIP_DATA, GRB>(leds, STRIP_LENGTH);
  FastLED.setBrightness(80);
  Serial.begin(115200, SERIAL_8N1);
}

void loop() {
  if (Serial.available()) {
    while (Serial.available() > 0) {
      byte inByte = Serial.read();
      //Serial.println(inByte);
      if (inByte == 0xFF) { //Just read a frame end byte, write out the LED strip and tell rev we're ready for the next lighting update afterwards
        bytesToRead = 0;
        FastLED.show();
        Serial.write(byte(0x00)); //Rev expects SOME kind of response after every sent frame - sending nothing results in laggy/glitchy updates. No idea what I need to send but this works, /shrug

      } else {
        switch (bytesToRead) {
          case 0: //Reading an ID byte
            curId = inByte;
            bytesToRead = 4;
            break;

          case 1: //Reading a data byte (Blue)
            ledNum = idToLED(curId); //This is the final byte for this LED - if the LED ID we got is one we can write to the LED strip, write it to the strip!
            if (ledNum != -1) { leds[ledNum] = CRGB(curRed, curGreen, inByte); } //Got the final byte, write to the strip
            break;
            
          case 2: //Reading a data byte (Green)
            curGreen = inByte; break;
            
          case 3: //Reading a data byte (Red)
            curRed = inByte; break;
        }
        bytesToRead--; //Decrement the "bytes to read" value
      }
    }
    
  }
}
