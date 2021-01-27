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

    String log(DataArray* data, String name, String colnames = "timestamp,x,y,z");
};


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

    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) 
    {
        Serial.println(F("Archivo abierto correctamente"));
        dataFile.println(colnames);
        for (uint16_t x = 0; x < data->sizeX ; x++)
        {
            dataFile.print(x);
            dataFile.print(",");
            for (uint8_t y = 0; y < (data->sizeY)-1; y++)
            {
                dataFile.print(data->operator()(x , y));
                dataFile.print(",");
            }
            dataFile.print(data->operator()(x , data->sizeY-1));
            dataFile.println();
            delay(0);
        }
    }
    else
    {
        Serial.println(F("Error abriendo el archivo"));
    }
    dataFile.close();

    return filename;
}
