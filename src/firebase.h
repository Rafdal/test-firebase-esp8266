
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