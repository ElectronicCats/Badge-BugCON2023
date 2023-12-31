/*
  Salvador Mendoza - Metabase Q
  
  Spoof Apple AirTag advertising package
  Aug 10th, 2022

  Andres Sabas - Mod for ESP32-S3 
  Mar 2, 2023
*/
#include <ArduinoBLE.h>

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEBroadcast);

uint8_t advertisement[31] = { //Data to be advertise
  0x1E, 0xFF, 0x4C, 0x00, 0x12, 0x19, 0x10, 0x12, 0x12, 0x34, 0x56, 
  0x78, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 
  0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x90
  };    

// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
const uint8_t manufactData[4] = {0x01, 0x02, 0x03, 0x04};
const uint8_t serviceData[3] = {0x00, 0x01, 0x02};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  for (int i = 0; i < 10; i++) {
    Serial.println("Hi");
    delay(200);
  }

  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }

  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Build scan response data packet
  BLEAdvertisingData scanData;
  // Set parameters for scan response packet
  //scanData.setLocalName("Test enhanced advertising");
  // Copy set parameters in the actual scan response packet
  //BLE.setScanResponseData(scanData);

  // Build advertising data packet
  BLEAdvertisingData advData;

  uint8_t adv[27];
  for (int x = 4; x < 31; x++) {
    adv[x-4] = (byte)advertisement[x];
  }

  uint16_t idm = 0x004c; //Apple ID
  
  // Set parameters for advertising packet
  advData.setManufacturerData(idm, adv, sizeof(adv));
  advData.setAdvertisedService(myService);
  advData.setAdvertisedServiceData(0xfff0, serviceData, sizeof(serviceData));
  // Copy set parameters in the actual advertising packet
  BLE.setAdvertisingData(advData);

  // Print the address of the device
  Serial.print("Device address: ");
  Serial.println(BLE.address());

  BLE.advertise();
  Serial.println("advertising ...");
}

void loop() {
  BLE.poll();
}
