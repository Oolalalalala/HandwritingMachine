#include "src/Core/Esp32Program.h"


ESP32Program program;

void setup()
{
    Serial.begin(9600);
    Serial.println("Initialized");

    program.Initialize();
}


void loop()
{
    program.OnUpdate();
}