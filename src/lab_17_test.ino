#include "oled-wing-adafruit.h"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
const size_t UART_TX_BUF_SIZE = 20;
const size_t SCAN_RESULT_COUNT = 20;
BleScanResult scanResults[SCAN_RESULT_COUNT];
BleCharacteristic peerTxCharacteristic;
BleCharacteristic peerRxCharacteristic;
BlePeerDevice peer;
uint8_t txBuf[UART_TX_BUF_SIZE];
size_t txLen = 0;
const unsigned long SCAN_PERIOD_MS = 2000;
unsigned long lastScan = 0;
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    for (size_t ii = 0; ii < len; ii++) {
        Serial.print(data[ii]);
    }
}
// #include "LIS3DH.h"
// Vector<BleScanResult> scanResults;
// uint8_t buf[BLE_MAX_ADV_DATA_LEN];
// size_t len;
// SYSTEM_MODE(MANUAL);
// SYSTEM_THREAD(ENABLED);
// BleCharacteristic accelData;
void scan() {
    BleScanFilter filter;
    filter.serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    peerTxCharacteristic = BLE.scanWithFilter(filter);
    BlePeerDevice peer = BLE.connect(scanResults[0].address());
    if(peer.connected()) {
      Serial.print("connected");
      peer.getCharacteristicByUUID(peerTxCharacteristic, BleUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E"));
    }
    // uint8_t buf[BLE_MAX_ADV_DATA_LEN];
    // size_t len;
    // len = scanResults[0].advertisingData().get(BleAdvertisingDataType::SERVICE_UUID_16BIT_COMPLETE, buf, BLE_MAX_ADV_DATA_LEN);
    // Serial.println(len);
}
// void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
// void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
//     for (size_t i = 0; i < len; i++) {
//         Serial.println(data[i]);
//     }
// }

  // ssize_t getAdvertisingData(BleAdvertisingData* advertisingData);
  //   void getAdvertisingData(BleAdvertisingData* advertisingData){
  // Serial.println(advertisingData);
  //   }

// LIS3DHSPI accel(SPI, D3, WKP);
// int movingX;
// int movingY;
// // const unsigned long PRINT_SAMPLE_PERIOD = 100;
// // unsigned long lastPrintSample = 0;



// OledWingAdafruit display;
// int x = 10;
// int y = 10;

// const unsigned char bitmap_mff31l[] PROGMEM = {
//   0xFF,0xF7,0xFF,0xFC,
//   0xFF,0xE7,0xFF,0xFC,
//   0xC0,0x04,0x30,0x0C,
//   0xDF,0xFC,0xE7,0xEC,
//   0xC0,0x00,0x04,0x0C,
//   0xDF,0xE7,0xFC,0x3C,
//   0xDF,0xF7,0xFC,0x3C,
//   0xD0,0x34,0x84,0x0C,
//   0xDF,0xB4,0xA7,0x8C,
//   0xC0,0x34,0xB4,0x0C,
//   0xFF,0xF4,0xBC,0x0C,
//   0xFF,0xF4,0xBC,0x2C,
//   0xC0,0x36,0x34,0x2C,
//   0xDF,0xF7,0xF7,0xEC,
//   0xD8,0x30,0x36,0x6C,
//   0xF0,0x30,0x34,0x2C,
//   0xF6,0x1F,0xA4,0x2C,
//   0xD6,0x00,0x00,0x2C,
//   0xD6,0xFF,0xFF,0x2C,
//   0xD6,0x7F,0xFF,0x8C,
//   0xD6,0x00,0x31,0x8C,
//   0xD7,0xFF,0xB1,0xFC,
//   0xD0,0x06,0x30,0x0C,
//   0xDF,0xF4,0xBF,0xEC,
//   0xDF,0xF4,0x9F,0xEC,
//   0xC0,0x34,0x84,0x2C,
//   0xDF,0xF4,0xE4,0x2C,
//   0xD0,0x04,0x34,0x0C,
//   0xFF,0xFF,0xF7,0xFC,
//   0xFF,0xFF,0xE7,0xFC
// };



void setup() {
      Serial.begin(9600);
	while (!Serial.isConnected());
	Serial.println("Central initialized...");
	BLE.on();
    peerTxCharacteristic.onDataReceived(onDataReceived, &peerTxCharacteristic);
  // Serial.begin(9600);
  // BLE.on();
  // scan();
  // accelData.onDataReceived(onDataReceived, NULL);
  // display.setup();
	// display.clearDisplay();
	// display.display();

  // LIS3DHConfig config;
	// config.setPositionInterrupt(16);
	// bool setupSuccess = accel.setup(config);
	// Serial.printlnf("setupSuccess=%d", setupSuccess);
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.

void loop() {
      if (BLE.connected()) {
        while (Serial.available() && txLen < UART_TX_BUF_SIZE) {
            txBuf[txLen++] = Serial.read();
            Serial.write(txBuf[txLen - 1]);
        }
        if (txLen > 0) {
        	// Transmit the data to the BLE peripheral
            peerRxCharacteristic.setValue(txBuf, txLen);
            txLen = 0;
        }
    }
    else {
    	if (millis() - lastScan >= SCAN_PERIOD_MS) {
    		// Time to scan
    		lastScan = millis();
    		size_t count = BLE.scan(scanResults, SCAN_RESULT_COUNT);
			if (count > 0) {
				for (uint8_t ii = 0; ii < count; ii++) {
					// Our serial peripheral only supports one service, so we only look for one here.
					// In some cases, you may want to get all of the service UUIDs and scan the list
					// looking to see if the serviceUuid is anywhere in the list.
					BleUuid foundServiceUuid;
					size_t svcCount = scanResults[ii].advertisingData().serviceUUID(&foundServiceUuid, 1);
					if (svcCount > 0 && foundServiceUuid == serviceUuid) {
						peer = BLE.connect(scanResults[ii].address());
						if (peer.connected()) {
							peer.getCharacteristicByUUID(peerTxCharacteristic, txUuid);
							peer.getCharacteristicByUUID(peerRxCharacteristic, rxUuid);
							// Could do this instead, but since the names are not as standardized, UUIDs are better
							// peer.getCharacteristicByDescription(peerTxCharacteristic, "tx");
						}
						break;
					}
				}
			}
    	}
    }
    // len = scanResults[0].advertisingData().get(BleAdvertisingDataType::SERVICE_UUID_16BIT_COMPLETE, buf, BLE_MAX_ADV_DATA_LEN);
    // Serial.println(len);
  // 	LIS3DHSample sample;
  // if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD){
  //   lastPrintSample = millis();
  //   if (accel.getSample(sample)){
  //     Serial.printlnf("%d,%d,%d", sample.x, sample.y, sample.z);
  //     movingX = sample.x;
  //     movingY = sample.y +300;
  //   }
  //   else{
  //     Serial.println("no sample");
  //   }
  // }
  // movingY = movingY/2000;
  // movingX = movingX/2000;

  // if(movingY > 0){
  //   for (size_t i = 0; i < movingY ; i++){
  //     if(display.getPixel(x,y+1) == 0 && y != 31){
  //       y = y+1;
  //     }
  //   }
  // }
  // if(movingX > 0){
  //   for (size_t i = 0; i < movingX ; i++){
  //     if(display.getPixel(x+1,y) == 0 && x != 123){
  //       x = x+1;
  //     }
  //   }
  // }
  // if(movingY < 0){
  //   for (int i = 0; i > movingY ; i--){
  //     if(display.getPixel(x,y-1) == 0 && y > 0){
  //       y = y-1;
  //     }
  //   }
  // }
  //  if(movingX < 0){
  //   for (int i = 0; i > movingX ; i--){
  //     if(display.getPixel(x-1,y) == 0 && x > 0){
  //       x = x-1;
  //     }
  //   }
  // }


  // display.clearDisplay();
	// display.setTextSize(1);
	// display.setTextColor(WHITE);
  // display.drawPixel(x,y,WHITE);
  // display.drawBitmap(90,0,bitmap_mff31l,30,30,WHITE);
	// display.display();
  // delay(200);

  // The core of your code will likely live here.

}

// SYSTEM_MODE(MANUAL);
// SYSTEM_THREAD(ENABLED);
// // These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// // UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.
// const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
// const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
// const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
// const size_t UART_TX_BUF_SIZE = 20;
// const size_t SCAN_RESULT_COUNT = 20;
// BleScanResult scanResults[SCAN_RESULT_COUNT];
// BleCharacteristic peerTxCharacteristic;
// BleCharacteristic peerRxCharacteristic;
// BlePeerDevice peer;
// uint8_t txBuf[UART_TX_BUF_SIZE];
// size_t txLen = 0;
// const unsigned long SCAN_PERIOD_MS = 2000;
// unsigned long lastScan = 0;
// void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
//     for (size_t ii = 0; ii < len; ii++) {
//         Serial.write(data[ii]);
//     }
// }
// void setup() {
//     Serial.begin();
// 	while (!Serial.isConnected());
// 	Serial.println("Central initialized...");
// 	BLE.on();
//     peerTxCharacteristic.onDataReceived(onDataReceived, &peerTxCharacteristic);
// }
// void loop() {

// }