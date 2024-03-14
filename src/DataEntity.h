#include <StringUtils.h>
class DataEntity
{

public:
    enum DataType
    {
        VALUE,
        RUNNABLE,
        DATETIME
    };
    DataEntity(/* args */);
    DataEntity(String name, int value, int tempValue, int minValue, int maxValue, DataType type);
    ~DataEntity();
    String name;
    int value;
    int tempValue;
    int minValue;
    int maxValue;
    DataType type;
    void saveChangedValue()
    {
        value = tempValue;
    }
    void discardChanges()
    {
        tempValue = value;
    }
    void incrementTempValue()
    {
        if (tempValue >= minValue && tempValue <= maxValue)
        {
            tempValue++;
        }
    }
    void decrementTempValue()
    {
        if (tempValue >= minValue && tempValue <= maxValue)
        {
            tempValue--;
        }
    }
};

DataEntity::DataEntity(String name, int value, int tempValue, int minValue, int maxValue, DataType type)
{
    this->name = name;
    this->value = value;
    this->tempValue = tempValue;
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->type = type;
}

DataEntity::~DataEntity()
{
}
