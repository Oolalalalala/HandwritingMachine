#include "src/Core/Esp32Program.h"
#include "src/Core/WifiInterface.h"
#include "src/Core/Camera.h"


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