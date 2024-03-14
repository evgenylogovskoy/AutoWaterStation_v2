#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Clock.h"
#include "Global.h"

LiquidCrystal_I2C lcd(0x3f, 20, 4);
#define backLightTimeout 30
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
    lcd.print("Status: OK/ALERT");

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
    if (millis() - backilghtStartTime > backLightTimeout * 1000)
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
