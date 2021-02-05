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

// #include <PerceptorLogger.h>
// PerceptorLogger perceptor;
#include <Adafruit_MPU6050.h>

#include <datalog.h>
#include <Sensors.h>
Datalog logger;
DataArray data(512, 3);

#include <Settings.h>
#include <SerialSetup.h>
SerialSetup ser;
#include <DS3231.h>
DS3231 rtc;

void setup()
{
    Serial.begin(230400);
    rtc.begin();

    settings.load();
    ser.setupLoop([](){
        ser.on("ssid", [](String str){
            settings.json["ssid"] = str;
		});
		ser.on("pass", [](String str){
            settings.json["pass"] = str;
		});
		ser.on("save", [](){
            settings.save();
		});
		ser.on("load", [](){
            settings.load();
		});
		ser.on("print", [](){
            settings.print();
		});
		ser.on("reset", [](){
            ESP.reset();
		});
        ser.on("time", [](){
            rtc.read();
            Serial.println(rtc.getDateTimeISO8601());
        });
        ser.on("set-time", [](String str){
            rtc.setTime(str);
        });
    });

    Serial.println("INICIALIZANDO!");

    sensors.begin(&data);

    initSDCard(D0);

    // beginWifi();

    /* Assign the project host and api key (required) */
    config.host = FIREBASE_HOST;
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    // Firebase.reconnectWiFi(true);

    String base_path = "/UsersData2/";

    // Firebase.begin(&config, &auth);
    /** path for user data is now "/UsersData2/YOUR_USER_UID"
     * The user UID can be taken from auth.token.uid
    */
    path = base_path + auth.token.uid.c_str();

    fbdo.setBSSLBufferSize(1024, 1024);

    //Set the size of HTTP response buffers in the case where we want to work with large data.
    fbdo.setResponseSize(1024);    

    // checkTokenStatus();

    // sendFileToStorage("MOV0.CSV");


    // perceptor.readAndLog(10, "TEST");

    sensors.setInterval(4000UL);

    sensors.onRead([](DataArray* data){
        Serial.println(F("\nCallback\tCallback\tCallback\tCallback"));

        String filename = logger.log(data, "LOG");

        // sendFileToStorage(filename);
    });
}

unsigned long lastMs;
void loop()
{    
    // sensors.run();
    ser.listen();
    unsigned long currentMs = millis();
    if (currentMs - lastMs > 1000)
    {
        lastMs = currentMs;
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