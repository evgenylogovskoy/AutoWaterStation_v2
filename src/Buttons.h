#include <GyverButton.h>
#include "Global.h"

#define analogButtonsPin A0
#define backLightTimeout 30

String currentStatus = "OK/ALERT";     // OK/ALERT
String currentActivity = "WORKING..."; // NO/Fill/Flush/Air/Vent

GButton button_down;
GButton button_up;
GButton button_left;
GButton button_right;
GButton button_enter;
GButton button_back;
bool isHomePageSown = true;

void configureButtons()
{
    button_down.setDebounce(80);
    button_down.setType(LOW_PULL);
    button_down.setDirection(NORM_OPEN);

    button_up.setDebounce(80);
    button_up.setType(LOW_PULL);
    button_up.setDirection(NORM_OPEN);

    button_left.setDebounce(80);
    button_left.setType(LOW_PULL);
    button_left.setDirection(NORM_OPEN);

    button_right.setDebounce(80);
    button_right.setType(LOW_PULL);
    button_right.setDirection(NORM_OPEN);

    button_enter.setDebounce(80);
    button_enter.setType(LOW_PULL);
    button_enter.setDirection(NORM_OPEN);
    button_enter.setTickMode(AUTO);

    button_back.setDebounce(80);
    button_back.setType(LOW_PULL);
    button_back.setDirection(NORM_OPEN);
    button_back.setTickMode(AUTO);
}

void ENTER_CLICK()
{
    updateTimeForBacklightTimeout();
    if (currentDisplayMode == SETTINGS)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
        {
            currentDisplayMode = EDIT;
        }
        if (data[selectedMenuItem].type == DataEntity::DataType::RUNNABLE)
        {
            currentDisplayMode = RUN;
        }
        if (data[selectedMenuItem].type == DataEntity::DataType::DATETIME)
        {
            currentDisplayMode = TIME;
        }
    }
    else if (currentDisplayMode == EDIT)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
        {
            data[selectedMenuItem].saveChangedValue();
            Serial.println("Saved");
            currentDisplayMode = SETTINGS;
        }
    }
    else if (currentDisplayMode == RUN)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::RUNNABLE)
        {
            // Action on YES
            currentDisplayMode = SETTINGS;
            Serial.println("Run executed.");
        }
    }
    else if (currentDisplayMode == TIME)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::DATETIME)
        {
            saveTimeValues();
            currentDisplayMode = SETTINGS;
            cursorPosition = 0;
            Serial.println("Time saved.");
        }
    }
    Serial.println("ENTER");
}

void ENTER_LONG_PRESS()
{
    updateTimeForBacklightTimeout();
    if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
    {
        selectedMenuItem = 0;
        currentDisplayMode = SETTINGS;
    }

    Serial.println("ENTER HOLDED");
}
void BACK_CLICK()
{
    updateTimeForBacklightTimeout();
    if (currentDisplayMode == EDIT)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
        {
            data[selectedMenuItem].discardChanges();
            currentDisplayMode = SETTINGS;
            Serial.println("Changes discarded.");
        }
    }
    if (currentDisplayMode == RUN)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::RUNNABLE)
        {
            currentDisplayMode = SETTINGS;
            Serial.println("Run cancelled.");
        }
    }
    if (currentDisplayMode == TIME)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::DATETIME)
        {
            currentDisplayMode = SETTINGS;
            cursorPosition = 0;
            setTempDateValues();
            Serial.println("DateTime set cancelled.");
        }
    }

    Serial.println("BACK");
}
void BACK_LONG_PRESS()
{
    updateTimeForBacklightTimeout();
    currentDisplayMode = HOMEPAGE;
    Serial.println("BACK HOLDED");
    selectedMenuItem = 0;
}
void DOWN_CLICK()
{
    updateTimeForBacklightTimeout();
    if (currentDisplayMode == SETTINGS)
    {
        if (selectedMenuItem >= 0 && selectedMenuItem < getMenuDataSize() - 1)
        {
            selectedMenuItem++;
            isMenueItemChanged = true;
        }
    }
    if (currentDisplayMode == TIME)
    {
        if (daysPosition && curDay.toInt() > 1 && curDay.toInt() < 31)
        {
            curDay = curDay.toInt() - 1;
        }
        if (monthPosition && curMonth.toInt() > 1 && curMonth.toInt() < 12)
        {
            curMonth = curMonth.toInt() - 1;
        }
        if (yearsPosition && curYear.toInt() > 2023 && curYear.toInt() < 2033)
        {
            curYear = curYear.toInt() - 1;
        }
        if (hoursPosition && curHour.toInt() > 1 && curHour.toInt() < 24)
        {
            curHour = curHour.toInt() - 1;
        }
        if (minutesPosition && curMinutes.toInt() > 1 && curMinutes.toInt() < 60)
        {
            curMinutes = curMinutes.toInt() - 1;
        }
        if (secondsPosition && curSeconds.toInt() > 1 && curSeconds.toInt() < 60)
        {
            curSeconds = curSeconds.toInt() - 1;
        }
    }
    Serial.println("DOWN");
}
void UP_CLICK()
{
    updateTimeForBacklightTimeout();
    if (currentDisplayMode == SETTINGS)
    {
        if (selectedMenuItem > 0 && selectedMenuItem < getMenuDataSize())
        {
            selectedMenuItem--;
            isMenueItemChanged = true;
        }
    }
    if (currentDisplayMode == TIME)
    {
        if (daysPosition && curDay.toInt() >= 1 && curDay.toInt() < 31)
        {
            curDay = curDay.toInt() + 1;
        }
        if (monthPosition && curMonth.toInt() >= 1 && curMonth.toInt() < 12)
        {
            curMonth = curMonth.toInt() + 1;
        }
        if (yearsPosition && curYear.toInt() >= 2024 && curYear.toInt() < 2033)
        {
            curYear = curYear.toInt() + 1;
        }
        if (hoursPosition && curHour.toInt() >= 1 && curHour.toInt() < 24)
        {
            curHour = curHour.toInt() + 1;
        }
        if (minutesPosition && curMinutes.toInt() >= 1 && curMinutes.toInt() < 60)
        {
            curMinutes = curMinutes.toInt() + 1;
        }
        if (secondsPosition && curSeconds.toInt() >= 1 && curSeconds.toInt() < 60)
        {
            curSeconds = curSeconds.toInt() + 1;
        }
    }
    Serial.println("UP");
}
void LEFT_CLICK()
{
    updateTimeForBacklightTimeout();
    if (currentDisplayMode == EDIT)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
        {
            data[selectedMenuItem].decrementTempValue();
        }
    }
    if (currentDisplayMode == TIME)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::DATETIME)
        {
            if (cursorPosition > 0 && cursorPosition < 20)
            {
                cursorPosition = cursorPosition - 3;
            }
        }
    }

    Serial.println("LEFT");
}
void RIGHT_CLICK()
{
    updateTimeForBacklightTimeout();
    if (currentDisplayMode == EDIT)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
        {
            data[selectedMenuItem].incrementTempValue();
        }
    }
    if (data[selectedMenuItem].type == DataEntity::DataType::DATETIME)
    {
        if (cursorPosition >= 0 && cursorPosition < 19)
        {
            cursorPosition = cursorPosition + 3;
        }
    }
    Serial.println("RIGHT");
}

void proceedButtons()
{
    int analog = analogRead(analogButtonsPin);
    button_left.tick(analog < 1024 && analog > 1000);
    button_right.tick(analog < 550 && analog > 500);
    button_up.tick(analog < 750 && analog > 700);
    button_down.tick(analog < 900 && analog > 850);
    button_back.tick(analog < 345 && analog > 310);
    button_enter.tick(analog < 175 && analog > 150);
    if (button_enter.isHolded())
    {
        ENTER_LONG_PRESS();
        button_enter.resetStates();
        button_back.resetStates();
    }
    if (button_back.isHolded())
    {
        BACK_LONG_PRESS();
        button_enter.resetStates();
        button_back.resetStates();
    }
    if (button_down.isClick())
    {
        DOWN_CLICK();
    }
    if (button_up.isClick())
    {
        UP_CLICK();
    }
    if (button_left.isClick())
    {
        LEFT_CLICK();
    }
    if (button_right.isClick())
    {
        RIGHT_CLICK();
    }
    if (button_enter.isClick())
    {
        ENTER_CLICK();
    }
    if (button_back.isClick())
    {
        BACK_CLICK();
    }
}
