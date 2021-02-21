# AnimalRFIDTagReader

Reader for ISO11784/11785 134.2 kHz RFID tags as used in animal tracking and identification tags.

## Hardware

The code is for an ESP32 development board due to its small size and second hardware serial port but should also work with an Arduino Mega 2560 or similar.
To display the read information, a 0.66in OLED display with 64x48 pixel is used.
The display is connected via I2C pins D21 (SDA/D2) and D22 (SCL/D1), 3V3 and GND. The EM4305 board is connected to 5V, GND and RX2 on the ESP32.
If the ESP32 has a LiPo battery connector, fully portable operation should be possible.

## Open issues

The reader/antenna combinatio currently works fine with a test tag on close range but does not yet work with implanted tags. If anybody has an idea how to fix this - better antenna maybe? - let me know. 
