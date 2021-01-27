#include <SPI.h>
#include <SD.h>

void initSDCard(uint8_t csPin){
    Serial.print("Initializing SD card...");

    if (!SD.begin(csPin)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    if (SD.exists("/MOV0.CSV")) {
        Serial.println("MOV0.CSV exists.");
    } else {
        Serial.println("MOV0.CSV doesn't exist.");
    }
}