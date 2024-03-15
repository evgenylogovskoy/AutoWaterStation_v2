#ifndef Global_H
#define Global_H
#include <Arduino.h>
#include <StringUtils.h>
#include "DataEntity.h"

#define NUMITEMS(arg) ((unsigned int)(sizeof(arg) / sizeof(arg[0])))
template <typename T, size_t N>
size_t ArraySize(T (&)[N]) { return N; }

byte up_Arrow[8] = {
    B00100,
    B01110,
    B11111,
    B00100,
    B00100,
    B00100,
    B00100,
    B00000,
};
byte down_Arrow[8] = {
    B00000,
    B00100,
    B00100,
    B00100,
    B00100,
    B11111,
    B01110,
    B00100,
};
byte right_Arrow[8] = {
    B00000,
    B11000,
    B11100,
    B11110,
    B11111,
    B11110,
    B11100,
    B11000,
};
byte left_Arrow[8] = {
    B00000,
    B00011,
    B00111,
    B01111,
    B11111,
    B01111,
    B00111,
    B00011,
};
byte underLine[8] = {
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
};
enum DISPLAY_STATE
{
    HOMEPAGE,
    SETTINGS,
    EDIT,
    RUN,
    TIME
};
int8_t currentDisplayMode = -1;
int8_t cursorPosition = 0;
bool daysPosition = false;
bool monthPosition = false;
bool yearsPosition = false;
bool hoursPosition = false;
bool minutesPosition = false;
bool secondsPosition = false;
String curDay;
String curMonth;
String curYear;
String curHour;
String curMinutes;
String curSeconds = "00";

long backilghtStartTime = 0L;

DataEntity data[] = {
    // Changable
    {"Flush time(s) ", 32, 32, 0, 300, DataEntity::DataType::VALUE},
    {"Flush count ", 3, 3, 1, 20, DataEntity::DataType::VALUE},
    {"Air time(s) ", 60, 60, 0, 60, DataEntity::DataType::VALUE},
    {"Air count ", 3, 3, 1, 24, DataEntity::DataType::VALUE},
    {"Vent time(m) ", 30, 60, 1, 24, DataEntity::DataType::VALUE},
    {"Vent count ", 3, 3, 1, 24, DataEntity::DataType::VALUE},
    {"Recovery (s) ", 30, 360, 1, 24, DataEntity::DataType::VALUE},
    // Runnable
    {"Test sequence", 1, 1, 1, 1, DataEntity::DataType::RUNNABLE},
    {"Set defaults", 1, 1, 1, 1, DataEntity::DataType::RUNNABLE},
    // Date
    {"Date time", 1, 1, 1, 1, DataEntity::DataType::DATETIME}};

int selectedMenuItem = 0;
bool isMenueItemChanged = false;
enum DATETIME_ELEMENTS
{
    DAYS,
    MONTH,
    YEARS,
    HOURS,
    MINUTES,
    SECONDS
};
void setSelectedAsTrue(DATETIME_ELEMENTS elem)
{
    daysPosition = false;
    monthPosition = false;
    yearsPosition = false;
    hoursPosition = false;
    minutesPosition = false;
    secondsPosition = false;
    switch (elem)
    {
    case DAYS:
        daysPosition = true;
        break;
    case MONTH:
        monthPosition = true;
        break;
    case YEARS:
        yearsPosition = true;
        break;
    case HOURS:
        hoursPosition = true;
        break;
    case MINUTES:
        minutesPosition = true;
        break;
    case SECONDS:
        secondsPosition = true;
        break;
    default:
        break;
    }
}

int8_t getMenuDataSize()
{
    return ArraySize(data);
}

String addZeroIfRequired(String data)
{
    if (data.length() == 1)
    {
        return "0" + data;
    }
    else
    {
        return data;
    }
}


void updateTimeForBacklightTimeout()
{
    backilghtStartTime = millis();
}

extern char *__brkval;
int freeMemory()
{
    char top;
    return &top - __brkval;
}
int lowestSRAM = 2048;
void printFreeMem()
{
    int curSRAM = freeMemory();
    if (lowestSRAM > curSRAM)
        lowestSRAM = curSRAM;
    Serial.print(F("Free SRAM: "));
    Serial.print(curSRAM);
    Serial.print(F("    Lowest was: "));
    Serial.println(lowestSRAM);
}

#endif