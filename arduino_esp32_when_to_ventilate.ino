/*
 * Project name : arduino_esp32_when_to_ventilate
 * Arduino IDE's board selection:
 *            "Heltec_WIFI_kit_32", "80MHz", "115200"
 * 
 * This source code follows Adafruit SSD1306 examples.
 *            
 * by Max-hana / Oct.2017
 * 
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 16 // 4
//Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 display(-1);

HardwareSerial Serial2(2);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {
  //Wire.begin(4,15);     
  Serial.begin(115200);
  
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, 1);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  Serial.println("clearDisplay();");
  display.clearDisplay();

  Serial2.begin(9600); // for mh_z19b 
}

/*
 * Reply example
0-1-255  => 0xff
0-1-134  => 0x86
0-1-3    => 0x03
0-1-18   => 0x12
0-1-66   => 0x42
0-1-0    
0-1-0
0-1-74   => 0x4A
0-1-217  => 0xD9
 */

char buf_co2[20];
int i_co2=0;
int val_co2=-1;

/*
 * int send_CO2();
 *    send GET_STATUS command to MH-Z19B
 *    
 *    return 0 ... could 
 */
int send_CO2(){
  const byte a[] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
  while(Serial2.available() > 0){
    Serial2.read();
  }
  i_co2 = 0;
  if (0==i_co2){
    Serial2.write(a,9);
    return 1;
  }else{
    return 0;
  }
}

int fetch_CO2(){
  static int count = 10;
  if (Serial2.available() > 0) {
    count = 10;
    // read the incoming byte:
    byte in = Serial2.read();
    buf_co2[i_co2++] = in;
    if( (1 == i_co2) && (0xff != buf_co2[0]) ){
      i_co2 = 0;
    }else if( (2 == i_co2) && (0x86 != buf_co2[1]) ){
      if ( 0xff == buf_co2[1] )
        i_co2 = 1;
      else
        i_co2 = 0;
    }else if( 4 == i_co2 ){
      val_co2 = buf_co2[2]*256 + buf_co2[3];
    }else if( 9 <= i_co2 ){
      i_co2 = 0;
    }
    Serial.print("i_co2=");
    Serial.print(i_co2,DEC);
    Serial.print("\n");
  }else{
    --count;
    if(count < 0)
      delay(100);
  }
  return val_co2;
}


void show_CO2(int ppm) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0,0);

  display.print("CO2:");
  display.print(ppm, DEC);
  display.setTextSize(1);
  display.setCursor(100,7);
  display.println("ppm");

  display.display();
  delay(1);
}

void loop() {
  static int loop_count=0;

  if( loop_count < 100 ){
    if( 0 == loop_count ){
      delay(10);
      send_CO2();
      delay(10);
    }else if( 30 > loop_count ){
      fetch_CO2();
    }else if( 30 == loop_count ){
      Serial.print("ppm:");
      Serial.println(val_co2,DEC);
      delay(20);
      display.begin(SSD1306_SWITCHCAPVCC, 0x3C, 1);  // initialize with the I2C addr 0x3D (for the 128x64)
      display.display();
    }else if( 35 > loop_count ){
      delay(100);
    }else{
      show_CO2(val_co2);
      delay(100);
    }
    loop_count++;
  }else{
    loop_count = 0;
    val_co2=-1;
  }

}




