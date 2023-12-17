#include <Arduino.h>
#include <ArduinoBLE.h>

BLEService customService("d14f5020-b0b5-4aab-a9f7-4e51ca9dc5c0"); // Tilpas denne service UUID efter behov

BLEIntCharacteristic LedCharacteristic("352066d4-b359-49af-9bd3-79344287c87c", BLERead | BLEWrite);
BLEStringCharacteristic Printcharacteristic("401e7cf8-4fb0-491f-8046-4ca52462ec0c", BLERead | BLEWrite,100);

void setup() {
    Serial.begin(9600);

    // begin initialization
    if (!BLE.begin()) {
      Serial.println("starting Bluetooth® Low Energy module failed!");
    }

    pinMode(LED_BUILTIN,OUTPUT);

    BLE.setLocalName("JakobsRP2040"); // Sæt navn på enheden

    BLE.setAdvertisedService(customService); // Tilføj Service der skal reklameres

    // Tilføj careteristics til Servicen
    customService.addCharacteristic(LedCharacteristic);
    customService.addCharacteristic(Printcharacteristic);

    // Set default value på carecteristics
    LedCharacteristic.setValue(0);
    Printcharacteristic.setValue("Hello World from BLE");

    // Gør servicen synlig og reklamer enheden
    BLE.addService(customService);
    BLE.advertise();

    Serial.println("BLE Advertising Started");
}

void loop() {
    BLEDevice central = BLE.central();

    if(central)
    {
      Serial.print("Connected to ");
      Serial.print(central.address());
      Serial.println(": ");

      while (central.connected()) {

        // Når Led bliver ændret
        if(LedCharacteristic.written())
        {
          digitalWrite(LED_BUILTIN,LedCharacteristic.value());
          Serial.println("Lamp Changed....");
        }

        // Når Text bliver ændret
        if(Printcharacteristic.written())
        {
          Serial.print("Recieved message: ");
          Serial.println(Printcharacteristic.value());
        }

      }
    }
}