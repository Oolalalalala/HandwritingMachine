#include "src/Core/Mega2560Program.h"

Mega2560Program program;

void setup()
{
    program.Initialize();
}

void loop()
{
    program.OnUpdate();
}