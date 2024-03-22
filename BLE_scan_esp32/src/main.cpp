#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Arduino.h>

/* TTL RX TX Pins */
#define RX                                              16   // Pin to UART TTL Rx receiver
#define TX                                              17   // Pin to UART TTL Tx transmitter


/* Standard ScanTime: 1 minute (60 sec) */
int scanTime = 5;

/* Instance of BLE object */
BLEScan* pBLEScan;

/* Serial Communication baud rate */
int long PARAMS_SERIAL = 9600;

/* Buffer with scan data */
String buffer_scan;

/* Buffer to send by serial */
String buffer_send_tx = "";

/* Power limit to consider device */
int min_rssi = -55;


class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    buffer_scan = "";
    buffer_scan = String(advertisedDevice.toString().c_str());
  }
};


void setup() {

  // Serial monitor
  Serial.begin(PARAMS_SERIAL);

  // Send to LoRa
  Serial2.begin(PARAMS_SERIAL, SERIAL_8N1, RX, TX);

  Serial.println("Scanning...");
  BLEDevice::init("BLE application");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.printf("\nDevices found: %d", foundDevices.getCount());
  buffer_send_tx = "";
  for (uint32_t i = 0; i < foundDevices.getCount(); i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    if (device.getRSSI() > min_rssi) {
      buffer_send_tx += String(device.getAddress().toString().c_str()) + ",";
      buffer_send_tx += String(device.getName().c_str()) + ",";
      buffer_send_tx += String(device.getRSSI()) + ",";
    }
  }
  pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory

  if (Serial.available() > 0) {
    // serial applications
  }
  
  Serial.printf("\nDevices found: %s", buffer_send_tx.c_str());
  Serial2.write((buffer_send_tx).c_str());

  delay(10000);
}