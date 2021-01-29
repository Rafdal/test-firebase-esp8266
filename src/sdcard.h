#include <SPI.h>
#include <SD.h>

void initSDCard(uint8_t csPin){
    Serial.print("Initializing SD card...");

    if (!SD.begin(csPin)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");
}