# rev.LIGHTS

rev.LIGHTS is Arduino firmware that reads lighting data sent over serial in the crossbeats REV. lighting format, and outputs to it an addressible LED strip.

Just about any Arduino-compatible board will work - If it has a pulse (and can read serial data/run FastLED), it's probably compatible.

The codebase and protocol are documented so you can tinker around to get the perfect setup.

---

## Setup
1) Download/install the FastLED library
1) Upload RevLights.ino to an Arduino - just about anything that can run FastLED should work.
2) Set your Arduino to use serial port COM11
   * *To check/set: Go to Device Manager -> Ports (COM & LPT) -> Double-click the Arduino in the list -> Port Settings -> Advanced -> COM Port Number*
   * *Note: If COM11 is marked as "in use", make sure no other devices are set to use that port! If nothing else is using that port, set your Arduino to use COM11 anyways. You may need to reboot if you have issues opening COM11 (or if you plug in another device that Windows decides should *also* use COM11).*
3) Plug in a WS2812/Neopixel LED strip (default: pin 5) - the code is currently configured to use a strip at least 67 pixels long (the game outputs 58 usable pixels of data).
3) ???
4) Lights! :D

---

## Notes/Acknowledgements

Big thanks to jammitch for providing a reference video to help with light positioning/mapping

I definitely don't have the protocol figured out 100% - things work pretty dang well rn, but it seems there's still some minor inaccuracies lurking around. Feel free to tinker around, and enjoy!

---

*crossbeats REV. is owned by Capcom - this is an unofficial project that is in no way affiliated with or endorsed by Capcom. Please support their official releases!*