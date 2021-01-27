#ifndef _DATA_ARRAY_H
#define _DATA_ARRAY_H


class DataArray
{
private:
    float *data;
    uint16_t max_size;

public:
    const uint16_t sizeX;
    const uint16_t sizeY;
    
    DataArray(uint16_t x, uint16_t y);
    ~DataArray();

    float & operator ()(uint16_t x, uint16_t y);
    
    // float & operator = ()(uint16_t x, uint16_t y);

    void set(uint16_t x, uint16_t y, float val);
};

DataArray::DataArray(const uint16_t x, const uint16_t y) : sizeX(x), sizeY(y)
{
    data = new float[x * y];
    max_size = x*y;
}

DataArray::~DataArray()
{
    delete[] data;
}

float &DataArray::operator() (uint16_t x, uint16_t y)
{
    if ((sizeY*x + y) < max_size )
    {
        return data[sizeY*x + y];
    }
    Serial.println(F("ERROR SIZE OVERFLOW"));
    return data[0];
}

void DataArray::set(uint16_t x, uint16_t y, float val)
{
    if ((sizeY*x + y) < max_size)
    {
        data[sizeY*x + y] = val;
        return;
    }
    Serial.println(F("ERROR SIZE OVERFLOW"));
}


#endif