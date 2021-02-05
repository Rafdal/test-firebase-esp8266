#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <Arduino.h>
#include "SensorHandler.h"
#include <DataArray.h>


typedef void (*data_callback_t)(DataArray*);

class Sensors
{
private:
	
	#ifdef _ADAFRUIT_MPU6050_H
	Adafruit_MPU6050 mpu;
	#endif
	SensorHandler mpuHandler;

	data_callback_t callback;
	unsigned long lastMs;
	unsigned long interval = 15000;

public:
	DataArray* data;
	
	Sensors() {}
	~Sensors() {}

	void setMPU(double _freq = 1000, uint16_t _samples = 512);

	void begin(DataArray* d);

	void read(data_callback_t c);

	void onRead(data_callback_t c) { callback = c; }

	void setInterval(unsigned long i) { interval = i; }

	void run();
}sensors;

void Sensors::begin(DataArray* d)
{
	data = d;
	setMPU();
}

void Sensors::run()
{
	unsigned long currentMs = millis();
	if (currentMs - lastMs > interval)
	{
		read(callback);

		lastMs = currentMs;
	}
}

void Sensors::read(data_callback_t c)
{
	if (data==NULL)
	{
		Serial.println(F("Sensors.h:64"));
		return;
	}
	
	while (mpuHandler.available())
	{
		uint16_t i = mpuHandler.iterate();
		
		sensors_event_t a,g,t;
		mpu.getEvent(&a, &g, &t);

		data->set(i, 0, a.acceleration.x);
		data->set(i, 1, a.acceleration.y);
		data->set(i, 2, a.acceleration.z);

		mpuHandler.wait();
	}
	
	if (c != NULL)
		(*c)(data);
}

void Sensors::setMPU(double _freq, uint16_t _samples)
{
	mpuHandler.freq = _freq;
	mpuHandler.samples = _samples;

	if (!mpu.begin()) {
		pinMode(LED_BUILTIN, OUTPUT);
		delay(1000);
		Serial.println(F("Failed to find MPU6050 chip"));
	}

	Serial.println(F("MPU6050 Found!"));

	Wire.setClock(400000L); // 400kHz

	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

	delay(500);	
	sensors_event_t a,b,c;
	for (uint8_t i = 0; i < 200; i++)
	{
		mpu.getEvent(&a, &b, &c);
	}	
}

//aasdasd


#endif