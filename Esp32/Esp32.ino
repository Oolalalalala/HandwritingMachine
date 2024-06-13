#include "src/Core/Esp32Program.h"


ESP32Program program;

void setup()
{
    Serial.begin(115200);
    Serial.println("Initialized");

    program.Initialize(true);
}


void loop()
{
    program.OnUpdate();
}