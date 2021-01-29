#ifndef _SHARED_PREFERENCES_H_
#define _SHARED_PREFERENCES_H_

#include <ArduinoJson.h>
#include <FS.h>

class Settings
{
private:
	const char* filename = {"/settings.json"};

public:
	Settings() {}
	~Settings() {}

	StaticJsonDocument<512> json;

	bool save();
	bool load();

	void print();
}settings;

bool Settings::save()
{
	if (!SPIFFS.begin())
	{
		Serial.println(F("SPIFFS error"));
		return false;
	}

	if (SPIFFS.exists(filename))
		SPIFFS.remove(filename);
	
	File file = SPIFFS.open(filename, "w");

	if (!file)
	{
		Serial.print(F("Error opening file "));
		Serial.println(filename);
		return false;
	}

	if (serializeJson(json, file) == 0)
	{
		Serial.println(F("Error serializing json"));
		return false;
	}
	file.close();

	return true;
}

bool Settings::load()
{
	if (!SPIFFS.begin())
	{
		Serial.println(F("SPIFFS error"));
		return false;
	}

	if (!SPIFFS.exists(filename))
	{
		Serial.print(filename);
		Serial.println(F(" doesn't exist"));
		return false;
	}
	
	File file = SPIFFS.open(filename, "w");

	if (!file)
	{
		Serial.print(F("Error opening file "));
		Serial.println(filename);
		return false;
	}

	json.clear();
	DeserializationError error = deserializeJson(json, file);
	if (error)
	{
		Serial.println(F("Failed to read file, using default configuration"));
		return false;
	}

	file.close();

	return true;
}

void Settings::print()
{
	if (load())
	{
		serializeJsonPretty(json, Serial);
	}
	else
	{
		Serial.println(F("Error loading settings.json"));
	}
}

#endif