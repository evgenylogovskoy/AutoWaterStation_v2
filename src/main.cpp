#include <Arduino.h>
#include "LCD_Menu.h"
#include "Buttons.h"

void setup()
{
  Serial.begin(9600);
  // setCompileTime(); // Uncomment to set compile time to the RTS module
  initDisplay();
  currentDisplayMode = HOMEPAGE;
  configureButtons();
  
}

void loop()
{
  renderDisplay();
  proceedButtons();
  disableBackLightIfTimeEnd();
}
