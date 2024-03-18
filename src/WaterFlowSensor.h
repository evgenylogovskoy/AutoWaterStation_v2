#ifndef WaterFlowSensor_H
#define WaterFlowSensor_H
#include <Arduino.h>
#include "Global.h"

volatile int flow_frequency;  // Measures flow sensor pulses
unsigned int l_hour;          // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

void flow() // Interrupt function
{
    flow_frequency++;
}
void initFlowSensor()
{
    pinMode(flowsensor, INPUT);
    digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up

    attachInterrupt(0, flow, RISING); // Setup Interrupt
    sei();                            // Enable interrupts
    currentTime = millis();
    cloopTime = currentTime;
}

unsigned int getCurrentFlow()
{
    currentTime = millis();
    // Every second, calculate and print litres/hour
    if (currentTime >= (cloopTime + 1000))
    {
        cloopTime = currentTime; // Updates cloopTime
        // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
        l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
        flow_frequency = 0;                   // Reset Counter
        // Serial.print(l_hour, DEC);            // Print litres/hour
        // Serial.println(" L/hour");
        flowValue = l_hour;
        return l_hour;
    }
}

bool isFlowPresent()
{
    for (int i = 0; i < 10; i++)
    {
        delay(5);
        int flow = getCurrentFlow();
        // Serial.print("Cur flow value: ");
        // Serial.println(flowValue);
        if (flowValue < 5)
        {
            return false;
        }
    }
    return true;
}

#endif