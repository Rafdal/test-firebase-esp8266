#ifndef _PERC_LOGGER_H_
#define _PERC_LOGGER_H_


#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "datalog.h"

class PerceptorLogger
{
private:
    Adafruit_MPU6050 mpu;
    unsigned int sampling_period_us;
    double freq = 1000; // $ Hz
    uint16_t samples;
    
public:
    PerceptorLogger(){}
    ~PerceptorLogger(){}

    void initialize(double _freq, uint16_t _samples);

    void readAndLog(uint16_t files, String name);
    // void logFile(uint16_t files, String name);
};


void PerceptorLogger::initialize(double _freq = 1000, uint16_t _samples = 512){

    freq = _freq;
    samples = _samples;

    // Try to initialize!

	if (!mpu.begin()) {
        pinMode(LED_BUILTIN, OUTPUT);
		while (1) {
			delay(1000);
            Serial.println("Failed to find MPU6050 chip");
		}
	}

	Serial.println("MPU6050 Found!");

	Wire.setClock(400000L); // 400kHz

	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

	delay(100);

	sampling_period_us = round(1000000 * (1.0 / freq));


	/* pinMode(4, OUTPUT);
    digitalWrite(4, LOW);   // Activar SD
    if (!SD.begin(4)) {
        pinMode(LED_BUILTIN, OUTPUT);
        while(1){
            delay(1000);
            Serial.println(F("SD card failed, or not present"));
        }
    }
    Serial.println(F("SD card initialized.\n")); */
}


void PerceptorLogger::readAndLog(uint16_t files, String name){
    
    unsigned long us, ms_stamp;
    Datalog datalog; // Nombre del archivo

    DataArray data(samples, 3);


    for (uint16_t j = 0; j < files; j++)
	{
		ms_stamp = millis();
		us = micros();
		for (int i = 0; i < samples; i++)
		{
			sensors_event_t a,g,t;
			mpu.getEvent(&a, &g, &t);


			data.set(i, 0, a.acceleration.x);
			data.set(i, 1, a.acceleration.y);
			data.set(i, 2, a.acceleration.z);
			// data.x[i] = a.acceleration.x;
			// data.y[i] = a.acceleration.y;
			// data.z[i] = a.acceleration.z;


			while (micros() - us < sampling_period_us) ;

			us += sampling_period_us;
			

			feedTheDog();

		}
		Serial.print("Time2Read: ");
		Serial.println(millis() - ms_stamp);

		String filename = datalog.log(&data, name);

		sendFileToStorage(filename, "TEST/");

		/* for (uint16_t i = 0; i < samples; i++)
		{
			Serial.print(data.x[i]);
			Serial.print(",\t");
			Serial.print(data.y[i]);
			Serial.print(",\t");
			Serial.println(data.z[i]);
		} */
		// for(;;);

		Serial.print(j);
		Serial.println(F(" Lectura Finalizada"));
		
		feedTheDog();
	}
}


#endif