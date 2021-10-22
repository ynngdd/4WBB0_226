/*
  BLE_Peripheral.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE
  as a peripheral device and specifies a service and a characteristic. Depending
  of the value of the specified characteristic, an on-board LED gets on.

  The circuit:
  - Arduino Nano 33 BLE.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>


const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

bool gesture = 0;

BLEService gestureService(deviceServiceUuid);
BLEBoolCharacteristic gestureCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);


void setup() {
  Serial.begin(9600);
  //while (!Serial);
  
  pinMode(4, OUTPUT); // LED
  pinMode(6, OUTPUT); // BUZZER
  
  if (!BLE.begin()) {
    Serial.println("- Starting BLE module failed!");
    while (1);
  }

  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");
  BLE.setAdvertisedService(gestureService);
  gestureService.addCharacteristic(gestureCharacteristic);
  BLE.addService(gestureService);
  gestureCharacteristic.writeValue(0);
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
      Serial.println("centralConn");
           // if (gestureCharacteristic.written()) {
      gesture = gestureCharacteristic.value();
      if(gesture) {
      writeGesture(gesture);        
        }
      else {
        digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
        noTone(5);
        }

          //  }
    }

    Serial.println("* Disconnected to central device!");
  }
}

void writeGesture(bool gesture) {
  Serial.println("it's this: ");
  Serial.println(gesture);
  digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  tone(6, 1000); // Send 1KHz sound signal...
  delay(1000);                       // wait for a second
  digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  noTone(6);
  delay(1000);                       // wait for a second
  
}
