#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


#include "DataArray.h"


class Datalog
{
private:
    size_t file_idx=0;

public:
    Datalog(){}
    ~Datalog(){}

    void throwError();

    String log(DataArray* data, String name, String colnames = "timestamp,x,y,z");
    void logToFile(DataArray* data, String file, bool printIndex);
    void logToFile(String data, String file);

    void logDebug(String text);
};

void Datalog::throwError()
{
    ets_printf("FATAL ERROR: Null number\n");
    abort();
}


void Datalog :: logDebug(String text)
{
    logToFile(text, "DEBUG.TXT");
}

void Datalog :: logToFile(DataArray* data, String filename, bool printIndex)
{
    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile && data != NULL) 
    {
        Serial.println(F("Archivo abierto correctamente"));
        for (uint16_t x = 0; x < data->sizeX() ; x++)
        {
            if (printIndex)
            {
                dataFile.print(x);
                dataFile.print(",");
            }
            for (uint8_t y = 0; y < (data->sizeY())-1; y++)
            {
                if (isnan(data->get(x , y)))
                {
                    throwError();
                }
                
                dataFile.print(data->get(x , y));
                dataFile.print(",");
            }
            dataFile.print(data->get(x , data->sizeY()-1));
            dataFile.println();
            delay(0);
        }
    }
    else
    {
        Serial.println(F("Error abriendo el archivo o DataArray nulo"));
    }
    dataFile.close();
}

void Datalog :: logToFile(String data, String filename)
{
    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) 
    {
        Serial.println(F("Archivo abierto correctamente"));
        dataFile.println(data);
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();
    
}

String Datalog :: log(DataArray* data, String name, String colnames)
{
    String filename;
    for (; file_idx < 1024; file_idx++)    
    {
        delay(0);
        filename = name + String(file_idx) + ".CSV";
        if (SD.exists(filename)) {;}
        else {break;}
    }
    logToFile(colnames, filename);
    logToFile(data, filename, true);    

    return filename;
}
