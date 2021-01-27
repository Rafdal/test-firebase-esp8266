#include <Arduino.h>


#include "private.h"
#include "wifi.h"
#include "auth.h"
#include "sdcard.h"

/* 8. Define the FirebaseConfig data for config data */
FirebaseConfig config;
FirebaseData fbdo0, fbdo;

void checkTokenStatus();
// void sendDataToFirebase();
void sendFileToStorage(String);

void setup()
{
    Serial.begin(115200);

    initSDCard(D4);

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

    // delay(1000);

    checkTokenStatus();
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
        // sendFileToStorage();
        lastMs2 = currentMs;
    }
}

void checkTokenStatus(){
    struct token_info_t info = Firebase.authTokenInfo();

    if (info.status == token_status_error)
    {
        Serial.printf("Token info: type = %s, status = %s\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
        Serial.printf("Token error: %s\n\n", getTokenError(info).c_str());
    }
    else
    {
        Serial.printf("Token info: type = %s, status = %s\n\n", getTokenType(info).c_str(), getTokenStatus(info).c_str());
    }
}

void sendDataToFirebase(){
    Serial.println("------------------------------------");
    Serial.println("Set int test...");

    /* if (Firebase.set(fbdo0, path + "/seconds", ((int)(millis()/1000))))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo0.dataPath());
        Serial.println("TYPE: " + fbdo0.dataType());
        Serial.println("ETag: " + fbdo0.ETag());
        Serial.print("VALUE: ");
        printResult(fbdo0);
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo0.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    } */
}

void sendFileToStorage(String filename){

    
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

/* void sendFileToFirebase(){
    if (Firebase.setFile(fbdo, StorageType::SD, path + "/Binary/File/data", "/MOV0.CSV"))
    {
        Serial.println("PASSED");
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.fileTransferError());
        Serial.println("------------------------------------");
        Serial.println();
    }
} */