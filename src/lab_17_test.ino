#include "oled-wing-adafruit.h"

#include "LIS3DH.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

LIS3DHSPI accel(SPI, D3, WKP);
int movingX;
int movingY;
const unsigned long PRINT_SAMPLE_PERIOD = 100;
unsigned long lastPrintSample = 0;



OledWingAdafruit display;
int x = 10;
int y = 10;

const unsigned char bitmap_9063d6[] PROGMEM = {
  0xFF,0xFF,0xFF,0xFC,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0x00,0x00,0x00,0x04,
  0xFF,0xFF,0xFF,0xFC
};



void setup() {
  Serial.begin(9600);
  
  display.setup();
	display.clearDisplay();
	display.display();

  LIS3DHConfig config;
	config.setPositionInterrupt(16);
	bool setupSuccess = accel.setup(config);
	Serial.printlnf("setupSuccess=%d", setupSuccess);
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.

void loop() {
  	LIS3DHSample sample;
  if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD){
    lastPrintSample = millis();
    if (accel.getSample(sample)){
      Serial.printlnf("%d,%d,%d", sample.x, sample.y, sample.z);
      movingX = sample.x;
      movingY = sample.y +300;
    }
    else{
      Serial.println("no sample");
    }
  }
  movingY = movingY/2000;
  movingX = movingX/2000;

  if(movingY > 0){
    for (size_t i = 0; i < movingY ; i++){
      if(display.getPixel(x,y+1) == 0 && y != 31){
        y = y+1;
      }
    }
  }
  if(movingX > 0){
    for (size_t i = 0; i < movingX ; i++){
      if(display.getPixel(x+1,y) == 0 && x != 123){
        x = x+1;
      }
    }
  }
  if(movingY < 0){
    for (int i = 0; i > movingY ; i--){
      if(display.getPixel(x,y-1) == 0 && y > 0){
        y = y-1;
      }
    }
  }
   if(movingX < 0){
    for (int i = 0; i > movingX ; i--){
      if(display.getPixel(x-1,y) == 0 && x > 0){
        x = x-1;
      }
    }
  }

  // Serial.println(display.getPixel(x,y));
  //   if(display.getPixel(x+1,y) == 0){
  //   x = x+1;
  // }
  display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
  display.drawPixel(x,y,WHITE);
  display.drawBitmap(90,0,bitmap_9063d6,30,30,WHITE);
	display.display();
  delay(200);

  // The core of your code will likely live here.

}

