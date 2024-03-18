#include <Arduino.h>
#include "LCD_Menu.h"
#include "Buttons.h"
#include "Relays.h"
#include "WaterFlowSensor.h"

unsigned long timebefore = 0; // Same type as millis()

void setup()
{
  Serial.begin(9600);
  // setCompileTime(); // Uncomment to set compile time to the RTS module
  initDisplay();
  initRelays();
  currentDisplayMode = HOMEPAGE;
  configureButtons();
  initFlowSensor();
  initLevelSensors();
}

void loop()
{

  // Serial.print("#1 - ");
  // Serial.println(digitalRead(lowWaterLevel));
  // Serial.print("#2 - ");
  // Serial.println(digitalRead(fullWaterLevel));
  // Serial.print("#3 - ");
  // Serial.println(digitalRead(alertWaterLevel));
  // Serial.print("Flow is present: "); Serial.println(isFlowPresent());

  initTempControlSequence();
  renderDisplay();
  proceedButtons();
  disableBackLightIfTimeEnd();
  getCurrentFlow();
  updateLevelSensorsValue();
}
