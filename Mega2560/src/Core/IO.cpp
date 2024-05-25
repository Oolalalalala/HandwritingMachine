#include "IO.h"

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <Arduino.h>



#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define ENTER_BUTTON_PIN 32
#define CANCEL_BUTTON_PIN 34


struct IOData
{
    hd44780_I2Cexp LCD;
    char WriteBuffer[21];

    bool LastFrameButtonStates[7];
    bool ButtonStates[7];
    
    Vector2Int JoystickPosition;
    Vector2Int JoystickDirection;
};

static IOData s_Data;

void IO::Initialize()
{
    s_Data.LCD.begin(20, 4);
    s_Data.LCD.clear();

    for (int i = 0; i < 7; i++)
    {
        s_Data.LastFrameButtonStates[i] = false;
        s_Data.ButtonStates[i] = false;
    }

    s_Data.WriteBuffer[20] = '\0';
}

void IO::PullData()
{
    // Save last frame button states
    memcpy(s_Data.LastFrameButtonStates, s_Data.ButtonStates, sizeof(s_Data.ButtonStates));

    // Buttons
    s_Data.ButtonStates[(int)Button::Enter] = digitalRead(ENTER_BUTTON_PIN);
    s_Data.ButtonStates[(int)Button::Cancel] = digitalRead(CANCEL_BUTTON_PIN);


    // Joystick
    s_Data.JoystickPosition.X = analogRead(JOYSTICK_X_PIN) - 512;
    s_Data.JoystickPosition.Y = analogRead(JOYSTICK_Y_PIN) - 512;

    // Set four directions to false
    for (int i = (int)Button::JoystickUp; i <= (int)Button::JoystickRight; i++)
        s_Data.ButtonStates[i] = false;

    // Determine joystick direction
    if (abs(s_Data.JoystickPosition.X) > abs(s_Data.JoystickPosition.Y))
    {
        if (s_Data.JoystickPosition.X > 256)
        {
            s_Data.JoystickDirection = { 1, 0 };
            s_Data.ButtonStates[(int)Button::JoystickRight] = true;
        }
        else if (s_Data.JoystickPosition.X < -256)
        {
            s_Data.JoystickDirection = { -1, 0 };
            s_Data.ButtonStates[(int)Button::JoystickLeft] = true;
        }
        else
            s_Data.JoystickDirection = { 0, 0 };
    }
    else
    {
        if (s_Data.JoystickPosition.Y > 256)
        {
            s_Data.JoystickDirection = { 0, 1 };
            s_Data.ButtonStates[(int)Button::JoystickUp] = true;
        }
        else if (s_Data.JoystickPosition.Y < -256)
        {
            s_Data.JoystickDirection = { 0, -1 };
            s_Data.ButtonStates[(int)Button::JoystickDown] = true;
        }
        else
            s_Data.JoystickDirection = { 0, 0 };
    }
}

bool IO::IsButtonPressed(Button button)
{
    return s_Data.ButtonStates[(int)button];
}

bool IO::IsButtonDown(Button button)
{
    return !s_Data.LastFrameButtonStates[(int)button] && s_Data.ButtonStates[(int)button];
}

bool IO::IsButtonUp(Button button)
{
    return s_Data.LastFrameButtonStates[(int)button] && !s_Data.ButtonStates[(int)button];
}

Vector2Int IO::GetJoystickPosition()
{
    return s_Data.JoystickPosition;
}

Vector2Int IO::GetJoystickDirection()
{
    return s_Data.JoystickDirection;
}

void IO::ClearDisplay()
{
    s_Data.LCD.clear();
}

void IO::DisplayMessage(int line, const char* message)
{
    // Copy string to buffer
    memset(s_Data.WriteBuffer, ' ', 20);
    memcpy(s_Data.WriteBuffer, message, min(20, strlen(message))); 

    s_Data.LCD.setCursor(0, line);
    s_Data.LCD.print(s_Data.WriteBuffer);
}