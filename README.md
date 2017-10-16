# arduino_esp32_when_to_ventilate (still in progress)

![overview](./png/when2ventilate_all.png "overview")

- This is a project for ESP32 with SSD1306 OLED modules(Heltec WIFI_KIT_32).
- MH-Z19B(CO2 sensor) is required to build this whole project. (get it from ebay or amazon approximately US$20.)
- MH-Z19B can be connected to micro controller in two way. PWM or Serial. I chose Serial.

|IO pin | Selected feature |
|-------|-----------|
|IO16 | U2_RXD <br />(* OLED reset pin is overridden with U2_RXD)|
|IO17 |  U2_TXD |


## installation instruction
- Add esp32 tool chain to your Arduino IDE.
  [github.com/espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)

- Add "Adafruit-GFX-Library" to your libraries folder.
[github.com/adafruit/Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)

- Add "Adafruit_SSD1306" to your libraries folder.
[github.com/adafruit/Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

- Due to the nature of Adafruit library, you need to do the following:
1. change the value of "SSD1306_LCDHEIGHT" to 64 so that it fits the display dimensions. To do this, simply uncomment "#define SSD1306_128_64" in stead of "#define SSD1306_128_32" in Adafruit_SSD1306.h:Line74-75
2. change "Wire.begin()" to "Wire.begin(4,15)" in Adafruit_SSD1306.cpp:Line206


## Notes
- The ESP32 board has a cheap/low-quality Chinese chip(CH341). This chip cannot go beyond 115200bps.
- IO pins for Serial1 is not available. Serial2 is the only available option to communicate with MH-19B for this board.
