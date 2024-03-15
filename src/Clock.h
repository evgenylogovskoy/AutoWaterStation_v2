#include <Arduino.h>
#include <microDS3231.h>
#include "Global.h"
MicroDS3231 rtc;

////////////////////////
// Methods declaration zone
void clockInit();
void setCompileTime();
String getDateTime();
void saveTimeValues();

// End - Methods declaration zone

////////////////////////
// Methods implementation zone
MicroDS3231 getClockInstance()
{
    return rtc;
}

void setCompileTime()
{
    rtc.setTime(COMPILE_TIME);
}

void clockInit()
{
    if (!rtc.begin())
    {
        Serial.println("DS3231 not found");
        for (;;)
            ;
    }
}
String getDateTime()
{
    return addZeroIfRequired(String(rtc.getDate())) + "/" +
           addZeroIfRequired(String(rtc.getMonth())) + "/" +
           rtc.getYear() + "  " +
           addZeroIfRequired(String(rtc.getHours())) + ":" +
           addZeroIfRequired(String(rtc.getMinutes()) + ":" +
                             addZeroIfRequired(String(rtc.getSeconds())));
}
void setTempDateValues()
{
    curDay = addZeroIfRequired(String(rtc.getDay()));
    curMonth = addZeroIfRequired(String(rtc.getMonth()));
    curYear = addZeroIfRequired(String(rtc.getYear()));
    curHour = addZeroIfRequired(String(rtc.getHours()));
    curMinutes = addZeroIfRequired(String(rtc.getMinutes()));
}

void saveTimeValues()
{
    rtc.setTime(curSeconds.toInt(), curMinutes.toInt(), curHour.toInt(), curDay.toInt(), curMonth.toInt(), curYear.toInt());
}

// END - Methods implementation zone