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
    Serial.println("ENTER");
}

void ENTER_LONG_PRESS()
{
    if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
    {
        selectedMenuItem = 0;
        currentDisplayMode = SETTINGS;
    }

    Serial.println("ENTER HOLDED");
    updateTimeForBacklightTimeout();
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

    Serial.println("BACK");
}
void BACK_LONG_PRESS()
{
    currentDisplayMode = HOMEPAGE;
    Serial.println("BACK HOLDED");
    selectedMenuItem = 0;
    updateTimeForBacklightTimeout();
}
void DOWN_CLICK()
{
    if (currentDisplayMode == SETTINGS)
    {

        if (selectedMenuItem >= 0 && selectedMenuItem < getMenuDataSize() - 1)
        {
            selectedMenuItem++;
            isMenueItemChanged = true;
        }
    }
    updateTimeForBacklightTimeout();

    Serial.println("DOWN");
}
void UP_CLICK()
{
    if (currentDisplayMode == SETTINGS)
    {
        if (selectedMenuItem > 0 && selectedMenuItem < getMenuDataSize())
        {
            selectedMenuItem--;
            isMenueItemChanged = true;
        }
    }
    updateTimeForBacklightTimeout();
    Serial.println("UP");
}
void LEFT_CLICK()
{
    if (currentDisplayMode == EDIT)
    {
        if (data[selectedMenuItem].type == DataEntity::DataType::VALUE)
        {
            data[selectedMenuItem].decrementTempValue();
        }
    }
    updateTimeForBacklightTimeout();
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
