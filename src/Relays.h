#ifndef Relays_H
#define Relays_H

#include <Arduino.h>
#include "Global.h"
#define fillSolenoid 10
#define flushSolenoid 11
#define fan 9
#define FULL_ON 255
#define OFF 0
#define lowWaterLevel 12
#define fullWaterLevel 3
#define alertWaterLevel 4
#define relay1 5
#define relay2 6
#define relay3 7
#define tempOfStartFanOn25PercentsOfPower 30
#define tempOfFanFor50PercentsOfPower 40
#define tempOfFanFor100PercentsOfPower 50
#define tempOfWarning 60
#define tempOfALERT 65

int internalTemp;
int tankIsFull = 0;
int tankIsOverFilled = 0;

////////////////////////
// Methods declaration zone
void initRelays();
void openFillSolenoid();
void closeFillSolenoid();
void startFlush();
void stopFlush();
void turnOnFan();
void initSequence();
void openPowerfullRelay();
void closePowerfullRelay();
void openAirVompressorRelay();
void closeAirVompressorRelay();
void openVentilationRelay();
void closeVentilationRelay();
void initTempControlSequence();
int getInnerTemp();
void initLevelSensors();
// End - Methods declaration zone

////////////////////////
// Methods implementation zone
void initLevelSensors()
{
    pinMode(alertWaterLevel, INPUT);
    pinMode(fullWaterLevel, INPUT);
    pinMode(lowWaterLevel, INPUT);
}
void updateLevelSensorsValue()
{
    int tempFullWaterLevel = 0;
    for (int i = 0; i < 10; i++)
    {
        tempFullWaterLevel = tempFullWaterLevel + digitalRead(fullWaterLevel);
    }
    if (tempFullWaterLevel == 10)
    {
        tankIsFull = 1;
    }
    else
    {
        tankIsFull = 0;
    }

    int tempTankIsOverFilled = 0;
    for (int i = 0; i < 10; i++)
    {
        tempTankIsOverFilled = tempTankIsOverFilled + digitalRead(alertWaterLevel);
    }
    if (tempTankIsOverFilled == 10)
    {
        tankIsOverFilled = 1;
    }
    else
    {
        tankIsOverFilled = 0;
    }
}
void initRelays()
{
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);
    pinMode(fillSolenoid, OUTPUT);
    pinMode(flushSolenoid, OUTPUT);
    pinMode(fan, OUTPUT);
    closeVentilationRelay();
}
void openPowerfullRelay()
{
    digitalWrite(relay2, HIGH);
}
void closePowerfullRelay()
{
    digitalWrite(relay2, LOW);
}

void openAirVompressorRelay()
{
    digitalWrite(relay1, HIGH);
}
void closeAirVompressorRelay()
{
    digitalWrite(relay1, LOW);
}
void openVentilationRelay()
{
    digitalWrite(relay3, LOW);
}
void closeVentilationRelay()
{
    digitalWrite(relay3, HIGH);
}
void openFillSolenoid()
{
    analogWrite(fillSolenoid, FULL_ON);
}
void closeFillSolenoid()
{
    analogWrite(fillSolenoid, OFF);
}
void startFlush()
{
    analogWrite(flushSolenoid, FULL_ON);
}
void stopFlush()
{
    analogWrite(flushSolenoid, OFF);
}
void turnOnFan(int pwm)
{
    analogWrite(fan, pwm);
}
int getInnerTemp()
{
    internalTemp = max(getTempFromSensorOnTransformer(), rtc.getTemperature());
    return internalTemp;
}
void initTempControlSequence()
{
    int temp = getInnerTemp();

    if (temp < tempOfStartFanOn25PercentsOfPower)
    {
        turnOnFan(0);
    }
    if (temp >= tempOfStartFanOn25PercentsOfPower && temp < tempOfFanFor50PercentsOfPower)
    {
        turnOnFan(100);
    }
    if (temp >= tempOfFanFor50PercentsOfPower && temp < tempOfFanFor100PercentsOfPower)
    {
        turnOnFan(180);
    }
    if (temp >= tempOfFanFor100PercentsOfPower && temp < tempOfWarning)
    {
        turnOnFan(255);
    }
    if (temp >= tempOfWarning && temp < tempOfALERT)
    {
        Serial.println("Fan WARNING");
        // here should be periodical BEEP once per 5 sec
    }
    if (temp >= tempOfALERT)
    {
        Serial.println("Fan ALERT");
        // here should be instant BEEP and relay OFF
    }
}
void initSequence()
{
    openFillSolenoid();
    delay(1000);
    closeFillSolenoid();
    delay(1000);
    startFlush();
    delay(1000);
    stopFlush();
    delay(1000);
}
// END  Methods implementation zone

#endif