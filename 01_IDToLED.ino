/**
 * Given an internal light id from the game, returns an LED on our LED strip to write to
 */
short idToLED(byte id) {
  switch (id) {
    case 0xB5: return 67; //Front Acryl. L (Upper)
    case 0xB7: return 66;
    case 0xB9: return 65;
    case 0xBB: return 62; //Front Acryl. L (Lower)
    case 0xBD: return 63;
    case 0xBF: return 64;

    case 0xC1: return 60; //Rear Acryl. L (Upper)
    case 0xC3: return 59;
    case 0xC5: return 58;
    case 0xC7: return 55; //Rear Acryl. L (Lower)
    case 0xC9: return 56;
    case 0xCB: return 57;


    case 0x01: return 39; //Side LED L (Bottom)
    case 0x03: return 40;
    case 0x05: return 41;
    case 0x07: return 42;
    case 0x09: return 43;
    case 0x0B: return 44;
    case 0x0D: return 45;
    case 0x0F: return 46; //Side LED L (Top)
    case 0x11: return 47;
    case 0x13: return 48;
    case 0x15: return 49;
    case 0x17: return 50;
    case 0x19: return 51;
    case 0x1B: return 52;


    case 0x51: return 36; //Woofer LED L
    case 0x53: return 35;
    case 0x55: return 34;
    case 0x57: return 31; //Woofer LED R
    case 0x59: return 32;
    case 0x5B: return 33; //(Duplicated-ish)


    case 0x1D: return 28; //Side LED R (Bottom)
    case 0x1F: return 27;
    case 0x21: return 26;
    case 0x23: return 25;
    case 0x25: return 24;
    case 0x27: return 23;
    case 0x29: return 22;
    case 0x2B: return 21; //Side LED R (Top)
    case 0x2D: return 20;
    case 0x2F: return 19; //(Duplicated to 0x39)
    case 0x31: return 18;
    case 0x33: return 17;
    case 0x35: return 16;
    case 0x37: return 15;


    case 0xDF: return 12; //Rear Acryl. R (Lower) - TODO: The eval screen's animation plays strangely on this group but looks fine in the test menu - I'm missing part of the lighting format, aren't I?
    case 0xE1: return 11;
    case 0xE3: return 10; //(Duplicated-ish)
    case 0xD9: return 7; //Rear Acryl. R (Upper)
    case 0xDB: return 8;
    case 0xDD: return 9;

    case 0xD3: return 5; //Front Acryl. R (Lower)
    case 0xD5: return 4;
    case 0xD7: return 3;
    case 0xCD: return 0; //Front Acryl. R (Upper)
    case 0xCF: return 1;
    case 0xD1: return 2;


    //Oddity lighting addresses: CxB writes to a few other odd internal ids
    
    //case 0x39: return 72; //Seems like a duplicate of ID 0x2f (Side LED R (Top)). Is written to pretty consistently.
    
    //Two or three addresses are written to during the "ALL LED" and "AUTO LED" tests, but aren't written to during specific LED group tests
    //case 0xE5: return 71; //0xE5 is written once during the all/auto led tests (duplicated from 0xE3) and very inconsistently in gameplay (written on the title screen, and pulses to the beat during songs). Always a light blue color, color doesn't match up with other led groups in gameplay.
    //default: return 73; //Another address or two is written to twice during all/auto tests (but not during group tests, written along with actual IDs 0x5B and 0xE3), and doesn't look to be written to in gameplay. Also catches some garbage data when the game sends Mystery Data:tm: over Serial on game startup
    
    default: return -1; //For non-testing where we don't want garbage LED data our your strip, return -1 to skip writing an LED to the strip
  }
}
