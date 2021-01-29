#include <Arduino.h>
#include <FS.h>


#include "private.h"
#include "wifi.h"
#include "auth.h"
#include "sdcard.h"

/* 8. Define the FirebaseConfig data for config data */
FirebaseConfig config;
FirebaseData fbdo0, fbdo;

void checkTokenStatus();
// void sendDataToFirebase();
void sendFileToStorage(String, String);


void feedTheDog()
{
    yield();
    ESP.wdtFeed();
}

#include <PerceptorLogger.h>
PerceptorLogger perceptor;

#include <Settings.h>

void setup()
{
    Serial.begin(115200);

    while (!Serial.available())
    {
        Serial.println(F("wait"));
        delay(1000);
    }

    // settings.json["ssid"] = "Wifi-Alcides";
    // settings.json["pass"] = "12345678";
    // settings.json["readInterval"] = 8;

    // settings.print();

    Serial.println("INICIALIZANDO!");

    perceptor.initialize();

    initSDCard(D0);

    beginWifi();

    /* Assign the project host and api key (required) */
    config.host = FIREBASE_HOST;
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    Firebase.reconnectWiFi(true);

    String base_path = "/UsersData2/";

    Firebase.begin(&config, &auth);
    /** path for user data is now "/UsersData2/YOUR_USER_UID"
     * The user UID can be taken from auth.token.uid
    */
    path = base_path + auth.token.uid.c_str();

    fbdo.setBSSLBufferSize(1024, 1024);

    //Set the size of HTTP response buffers in the case where we want to work with large data.
    fbdo.setResponseSize(1024);    

    checkTokenStatus();

    delay(1000);

    // sendFileToStorage("MOV0.CSV");
    feedTheDog();


    perceptor.readAndLog(10, "TEST");


    feedTheDog();
    

}

unsigned long lastMs, lastMs2;

void loop()
{
    unsigned long currentMs = millis();
    if (currentMs - lastMs > 5000)
    {
        // sendDataToFirebase();
        lastMs = currentMs;
    }

    if (currentMs - lastMs2 > 15000)
    {
        lastMs2 = currentMs;
    }
}


void sendDataToFirebase(){

}

void sendFileToStorage(String filename, String subfolder = ""){

    // subfolder += filename;
    if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, filename.c_str(), mem_storage_type_sd, filename.c_str(), "text/csv"))
    {
        Serial.printf("Download URL: %s\n", fbdo.downloadURL().c_str());
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}