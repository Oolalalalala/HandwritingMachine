#ifndef IO_H
#define IO_H

#include "../Utils/Math.h"

enum class Button
{
    None = 0,
    Enter, 
    Cancel,
    JoystickUp,
    JoystickDown,
    JoystickLeft,
    JoystickRight
};

class IO
{
public:
    static void Initialize();
    static void PullData();

    static bool IsButtonPressed(Button button);
    static bool IsButtonDown(Button button);
    static bool IsButtonUp(Button button);
    static Vector2Int GetJoystickPosition();
    static Vector2Int GetJoystickDirection();

    static void ClearDisplay();
    static void DisplayMessage(int line, const char* message);
};


#endif 