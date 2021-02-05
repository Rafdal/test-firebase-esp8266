#ifndef _DATA_ARRAY_H
#define _DATA_ARRAY_H


class DataArray
{
private:
    float *data, error = 404.0f;
    uint16_t max_size;

    uint16_t _sizeX, _sizeY;
    bool initialized = false;
public:
    uint16_t sizeX() {return _sizeX;}
    uint16_t sizeY() {return _sizeY;}
    
    DataArray(uint16_t x, uint16_t y);
    DataArray(){}

    void init(uint16_t x, uint16_t y);

    ~DataArray();
    
    void set(uint16_t x, uint16_t y, float val);
    float get(uint16_t x, uint16_t y);
    
    void print();
};

DataArray::DataArray(const uint16_t x, const uint16_t y)
{
    init(x, y);
}

DataArray::~DataArray()
{
    delete[] data;
    initialized = false;
}

void DataArray::init(uint16_t x, uint16_t y)
{
    if (data)
        delete[] data;

    data = new float[x * y];
    max_size = x*y;
    _sizeX = x;
    _sizeY = y;
    initialized = true;
}

float DataArray::get(uint16_t x, uint16_t y)
{
    if (!initialized)
    {
        Serial.println(F("NOT INITIALIZED"));
        return error;
    }

    if ((_sizeY*x + y) < max_size )
    {
        return data[_sizeY*x + y];
    }
    
    Serial.println(F("ERROR SIZE OVERFLOW"));
    return error;
}

void DataArray::set(uint16_t x, uint16_t y, float val)
{
    if (!initialized)
    {
        Serial.println(F("NOT INITIALIZED"));
        return;
    }

    if ((_sizeY*x + y) < max_size)
    {
        data[_sizeY*x + y] = val;
        return;
    }
    Serial.println(F("ERROR SIZE OVERFLOW"));
}

void DataArray::print()
{
    for (uint16_t x = 0; x < _sizeX && initialized; x++)
    {
        Serial.print(x);
        for (uint16_t y = 0; y < _sizeY; y++)
        {
            Serial.print('\t');
            Serial.print(get(x, y));
        }
        Serial.println();
    }
}


#endif