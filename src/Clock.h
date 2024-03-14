#include <Arduino.h>
#include <microDS3231.h>
#include "Global.h"
MicroDS3231 rtc;
////////////////////////
// Methods declaration zone
void clockInit();
void setCompileTime();
String getDateTime();

// End - Methods declaration zone









////////////////////////
// Methods implementation zone
MicroDS3231 getClockInstance(){
    return rtc;
}

void setCompileTime(){
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
            rtc.getYear() + "   " + 
            addZeroIfRequired(String(rtc.getHours())) + ":" + 
            addZeroIfRequired(String(rtc.getMinutes()) + ":" + 
            addZeroIfRequired(String(rtc.getSeconds())));
   
}

// END - Methods implementation zone