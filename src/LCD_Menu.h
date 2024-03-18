#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Clock.h"
#include "Global.h"
#include "Relays.h"
#include "WaterFlowSensor.h"
LiquidCrystal_I2C lcd(0x3f, 20, 4);

String curDateTime = "";
////////////////////////
// Methods declaration zone
void drawHomePage();
void renderDisplay();
void initDisplay();
void drawMenuPage();
void drawMenuItems();
void clearRow(byte rowToClear);
void clearMenuRows();
void drawEditPage();
void drawRunnablePage();
void drawDateEditPage();
void setCurTimeVariable();
// End - Methods declaration zone

////////////////////////
// Methods implementation zone
int8_t previousState = HOMEPAGE;
void renderDisplay()
{
    if (currentDisplayMode == HOMEPAGE)
    {
        if (previousState != HOMEPAGE)
        {
            lcd.clear();
        }
        drawHomePage();
        previousState = HOMEPAGE;
    }
    if (currentDisplayMode == SETTINGS)
    {
        if (previousState != SETTINGS)
        {
            lcd.clear();
        }
        drawMenuPage();
        previousState = SETTINGS;
    }
    if (currentDisplayMode == EDIT)
    {
        if (previousState != EDIT)
        {
            lcd.clear();
        }
        drawEditPage();
        previousState = EDIT;
    }
    if (currentDisplayMode == RUN)
    {
        if (previousState != RUN)
        {
            lcd.clear();
        }
        drawRunnablePage();
        previousState = RUN;
    }
    if (currentDisplayMode == TIME)
    {
        if (previousState != TIME)
        {
            lcd.clear();
        }
        drawDateEditPage();
        previousState = TIME;
    }
}

void drawMenuPage()
{
    lcd.setCursor(3, 0);
    lcd.print("SETTINGS MENU");
    drawMenuItems();
}

void drawMenuItems()
{
    if (isMenueItemChanged)
    {
        clearMenuRows();
    }
    if (selectedMenuItem < 3)
    {
        lcd.setCursor(1, 1);
        lcd.print(data[0].name);
        lcd.setCursor(1, 2);
        lcd.print(data[1].name);
        lcd.setCursor(1, 3);
        lcd.print(data[2].name);

        lcd.setCursor(0, selectedMenuItem + 1);
        lcd.print(">");
    }
    if (selectedMenuItem >= 3 && selectedMenuItem < getMenuDataSize())
    {
        lcd.setCursor(1, 1);
        lcd.print(data[selectedMenuItem - 2].name);
        lcd.setCursor(1, 2);
        lcd.print(data[selectedMenuItem - 1].name);
        lcd.setCursor(1, 3);
        lcd.print(data[selectedMenuItem].name);

        lcd.setCursor(0, 3);
        lcd.print(">");
    }
}

void drawDateEditPage()
{
    lcd.createChar(0, underLine);
    lcd.setCursor(5, 0);
    lcd.print(data[selectedMenuItem].name);

    lcd.setCursor(0, 1);
    lcd.print(addZeroIfRequired(curDay) + "/" + addZeroIfRequired(curMonth) + "/" +
              curYear + "  " + addZeroIfRequired(curHour) + ":" +
              addZeroIfRequired(curMinutes) + ":" +
              addZeroIfRequired(curSeconds));
    lcd.setCursor(0, 2);
    lcd.print("                    ");

    if (cursorPosition >= 0 && cursorPosition < 3)
    {
        lcd.setCursor(0, 2);
        lcd.print(char(0));
        lcd.setCursor(1, 2);
        lcd.print(char(0));
        setSelectedAsTrue(DAYS);
    }
    else if (cursorPosition >= 3 && cursorPosition < 5)
    {
        lcd.setCursor(3, 2);
        lcd.print(char(0));
        lcd.setCursor(4, 2);
        lcd.print(char(0));
        setSelectedAsTrue(MONTH);
    }
    else if (cursorPosition >= 6 && cursorPosition < 11)
    {
        lcd.setCursor(6, 2);
        lcd.print(char(0));
        lcd.setCursor(7, 2);
        lcd.print(char(0));
        lcd.setCursor(8, 2);
        lcd.print(char(0));
        lcd.setCursor(9, 2);
        lcd.print(char(0));
        setSelectedAsTrue(YEARS);
    }
    else if (cursorPosition >= 12 && cursorPosition < 14)
    {
        lcd.setCursor(12, 2);
        lcd.print(char(0));
        lcd.setCursor(13, 2);
        lcd.print(char(0));
        setSelectedAsTrue(HOURS);
    }
    else if (cursorPosition >= 15 && cursorPosition < 17)
    {
        lcd.setCursor(15, 2);
        lcd.print(char(0));
        lcd.setCursor(16, 2);
        lcd.print(char(0));
        setSelectedAsTrue(MINUTES);
    }
    else if (cursorPosition >= 18 && cursorPosition < 20)
    {
        lcd.setCursor(18, 2);
        lcd.print(char(0));
        lcd.setCursor(19, 2);
        lcd.print(char(0));
        setSelectedAsTrue(SECONDS);
    }

    lcd.setCursor(0, 3);
    lcd.print("< Cancel");
    lcd.setCursor(14, 3);
    lcd.print("Save >");
}
void drawRunnablePage()
{
    lcd.setCursor(3, 0);
    lcd.print(data[selectedMenuItem].name);
    lcd.setCursor(0, 3);
    lcd.print("NO");
    lcd.setCursor(17, 3);
    lcd.print("YES");
}

void drawHomePage()
{
    lcd.setCursor(0, 0);
    lcd.print("WATER CONTROL SYSTEM");

    lcd.setCursor(0, 1);
    lcd.print("Activity: WORKING...");

    lcd.setCursor(0, 2);
    // lcd.print("Status: OK/WARNING/ALERT");
    lcd.print("T1=" + String(getTempFromSensorOnTransformer()) + ", T2=" + String(rtc.getTemperature()));
    // lcd.print(",T2="+t2);
    lcd.setCursor(0, 3);
    lcd.print(addZeroIfRequired(String(rtc.getDate())));
    lcd.print("/");
    lcd.print(addZeroIfRequired(String(rtc.getMonth())));
    lcd.print("/");
    lcd.print(rtc.getYear());
    lcd.setCursor(12, 3);

    lcd.print(addZeroIfRequired(String(rtc.getHours())));
    lcd.print(":");
    lcd.print(addZeroIfRequired(String(rtc.getMinutes())));
    lcd.print(":");
    lcd.print(addZeroIfRequired(String(rtc.getSeconds())));
}

void initDisplay()
{
    clockInit();
    lcd.init();
    lcd.backlight();
    setTempDateValues();
}

void clearRow(byte rowToClear)
{
    lcd.setCursor(0, rowToClear);
    lcd.print("                    ");
}

void clearMenuRows()
{
    clearRow(1);
    clearRow(2);
    clearRow(3);
    isMenueItemChanged = false;
}

void disableBackLightIfTimeEnd()
{
    if (millis() - backilghtStartTime > backLightTimeout)
    {
        lcd.noBacklight();
    }
    else
    {
        lcd.backlight();
    }
}

void drawEditPage()
{
    lcd.createChar(0, right_Arrow);
    lcd.createChar(1, left_Arrow);
    lcd.setCursor(0, 0);
    lcd.print(data[selectedMenuItem].name);
    lcd.setCursor(9, 2);
    lcd.print(data[selectedMenuItem].tempValue);
    lcd.setCursor(13, 2);
    lcd.print(char(0));
    lcd.setCursor(6, 2);
    lcd.print(char(1));
    lcd.setCursor(0, 3);
    lcd.print("< Cancel");
    lcd.setCursor(14, 3);
    lcd.print("Save >");
}

// END - Methods implementation zone
