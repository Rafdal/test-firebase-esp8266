#include <Wire.h>
#include <Arduino.h>

#ifndef _DS3231_H_
#define _DS3231_H_

typedef struct Time_Data
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t wday;
    uint8_t mday;
    uint8_t month;
    uint8_t year;
}time_data_t;

class DS3231
{
private:
    uint8_t dectobcd(const uint8_t val)
    {
        return ((val / 10 * 16) + (val % 10));
    }
    uint8_t bcdtodec(const uint8_t val)
    {
        return ((val / 16 * 10) + (val % 16));
    }

    /* static const char ctab[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 
        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 
        'W', 'X', 'Y', 'Z'
    }; */

public:
    uint8_t second,minute,hour,wday,mday,month,year;
    // set 18-1-3 02:12:01
    
    DS3231();
    void begin();
    void write(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wday, uint8_t mday, uint8_t mon, uint8_t year);
    void update();
    void read();
    void setTime(String s);
    time_data_t time();
    String getTime();
    String getDate();
    String getDateISO8601();
    String getDateTimeISO8601();
    String get8byteDate();
    // String getDateTime();
    // char* getDayName();
};

DS3231::DS3231()
{
}

void DS3231::begin()
{
    Wire.begin();
}

void DS3231::write(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wday, uint8_t mday, uint8_t mon, uint8_t year)
{
    uint8_t TimeDate[7] = {sec, min, hour, wday, mday, mon, year};

    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    for (uint8_t i = 0; i <= 6; i++) {
        TimeDate[i] = dectobcd(TimeDate[i]);
        Wire.write(TimeDate[i]);
    }
    Wire.endTransmission();
}

void DS3231::update()
{
    uint8_t TimeDate[7] = {second, minute, hour, wday, mday, month, year};

    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    for (uint8_t i = 0; i <= 6; i++) {
        TimeDate[i] = dectobcd(TimeDate[i]);
        Wire.write(TimeDate[i]);
    }
    Wire.endTransmission();
}

void DS3231::read()
{
    // uint8_t TimeDate[7];        //second,minute,hour,dow,day,month,year
    Wire.beginTransmission(0x68);
    Wire.write(0x00);
    Wire.endTransmission();

	bool gotData = false;
	uint32_t start = millis(); // start timeout
	while(millis()-start < 100){
	  if (Wire.requestFrom(0x68, 7) == 7) {
      	gotData = true;
      	break;
      }
      delay(2);
    }
	if (!gotData)
    {
        second = 80;
    	return; // error timeout
    }
    second  = bcdtodec(Wire.read());
    minute  = bcdtodec(Wire.read());
    hour    = bcdtodec(Wire.read());
    wday    = bcdtodec(Wire.read());
    mday    = bcdtodec(Wire.read());
    month   = bcdtodec(Wire.read());
    year    = bcdtodec(Wire.read());   
}

time_data_t DS3231::time()
{
    time_data_t t;
    t.hour = hour;
    t.minute = minute;
    t.second = second;
    t.wday = wday;
    t.mday = mday;
    t.month = month;
    t.year = year;
    return t;
}

/* char* DS3231::getDayName()
{
    memory.set(n_dia);
    return memory.take(wday-1);
} */

String DS3231::getDate()
{
	char out[21];
	sprintf(out,"%02u/%02u/20%02u", mday, month, year);
	return String(out);
}

String DS3231::getDateISO8601()
{
	char out[21];
	sprintf(out,"20%02u-%02u-%02u", year, month, mday);
	return String(out);
}

void DS3231::setTime(String datetime)
{
    int _year=0,_month=0,_mday=0,_hour=0,_minute=0,_second=0;
    if(sscanf(datetime.c_str(), "%u-%u-%u %u:%u:%u", &_year, &_month, &_mday, &_hour, &_minute, &_second) == 6)
    {
        year = (uint8_t)_year;
        month = (uint8_t)_month;
        mday = (uint8_t)_mday;
        hour = (uint8_t)_hour;
        minute = (uint8_t)_minute;
        second = (uint8_t)_second;
        update();
        return;
    }
    Serial.println(F("Error, time must be in \"YY-MM-DD HH:MM:SS\" format"));
}

String DS3231::getDateTimeISO8601()
{
	char out[32];
    //          YYYY-MM-DDThh:mm:ssZ
	sprintf(out,"20%02u-%02u-%02uT%02u:%02u:%02uZ", year, month, mday, hour, minute, second);
	return String(out);
}

String DS3231::getTime()
{
	char out[10];
	sprintf(out,"%02u:%02u:%02u", hour, minute, second);
	return String(out);
}
// set 19-12-31 23:51:59

String DS3231::get8byteDate()
{
    char out[10];
	sprintf(out,"%02u-%02u-%02u", year, month, mday);
	return String(out);
}

/* String DS3231::get5byteDate()
{
    char out[10];
	sprintf(out,"%c%c%c", ctab[month], ctab[mday]);
	return String(out);
} */

/* String DS3231::getDateTime()
{
	char out[21];
    memory.set(n_dia);
    String shortDay = String(memory.take(wday)).substring(0,2);
	sprintf(out,"%2s %02u/%02u/%02u %02u:%02u:%02u", shortDay.c_str(), mday, month, year, hour, minute, second);
	return String(out);
} */

#endif