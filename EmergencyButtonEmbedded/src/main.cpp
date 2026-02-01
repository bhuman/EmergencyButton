#include <Arduino.h>

#define BUTTON_PIN 5
#ifndef EMERGENCY_BUTTON_SERIAL_BAUDRATE
#define EMERGENCY_BUTTON_SERIAL_BAUDRATE 9600
#endif

void setup() 
{
    Serial.begin(EMERGENCY_BUTTON_SERIAL_BAUDRATE);
    pinMode(BUTTON_PIN, INPUT);

    //Disable LEDs
    pinMode(LED_BUILTIN_TX, INPUT);
    pinMode(LED_BUILTIN_RX, INPUT);
}

void loop()
{
    if(Serial.availableForWrite())
        Serial.write(digitalRead(BUTTON_PIN) ? "n" : "y");
}
