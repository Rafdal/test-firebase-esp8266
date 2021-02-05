#ifndef _SENSOR_HANDLER_H_
#define _SENSOR_HANDLER_H_

#include <DataArray.h>


class SensorHandler
{
private:
    int16_t remaining = -1;
    unsigned int sampling_period_us;
    unsigned long lastMs, us;

public:
    double freq; // $ Hz
    uint16_t samples;


    SensorHandler() {remaining = -1;}
    ~SensorHandler() {}

    bool available();
    uint16_t iterate();
    void wait();
};

bool SensorHandler::available()
{
    // Inicio ciclo
    if (remaining == -1 || remaining == samples)
    {
        remaining = samples;
        us = micros();
        sampling_period_us = round(1000000 * (1.0 / freq));
        return true;
    }
    
    // Continuacion ciclo
    if (remaining > 0)
        {return true;}  

    // fin ciclo
    if (remaining == 0)
    {
        remaining = samples;
        return false;
    }
    return false;
}

uint16_t SensorHandler::iterate()
{
    uint16_t out = samples-remaining;
    if (remaining > 0)
        remaining--;
    return out;
}

void SensorHandler::wait()
{
    while (micros() - us < sampling_period_us) ;
    us += sampling_period_us;
    delay(0);
}

#endif