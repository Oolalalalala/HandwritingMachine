#include "src/Core/Mega2560Program.h"


Mega2560Program program;


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