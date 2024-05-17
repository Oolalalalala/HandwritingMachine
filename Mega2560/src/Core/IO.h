#ifndef IO_H
#define IO_H

#include "../Utils/Math.h"

class IO
{
public:
    static void PullData();

    static bool IsButtonPressed();
    static bool IsButtonDown();
    static bool IsButtonReleased();
    static Vector2 GetAnalogStickPosition();

    static void DisplayMessage(const char* message, int line);
};


#endif 