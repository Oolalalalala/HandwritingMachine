#include "IO.h"

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include <Arduino.h>



#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define ENTER_BUTTON_PIN 2
#define CANCEL_BUTTON_PIN 3


struct IOData
{
    hd44780_I2Cexp LCD;
    char WriteBuffer[21];

    bool LastFrameButtonStates[7];
    bool ButtonStates[7];
    
    Vector2Int JoystickPosition;
    Vector2Int JoystickDirection;
};

static IOData s_IOData;

void IO::Initialize()
{
    s_IOData.LCD.begin(20, 4);
    s_IOData.LCD.clear();

    for (int i = 0; i < 7; i++)
    {
        s_IOData.LastFrameButtonStates[i] = false;
        s_IOData.ButtonStates[i] = false;
    }

    s_IOData.WriteBuffer[20] = '\0';
}

void IO::PullData()
{
    // Save last frame button states
    memcpy(s_IOData.LastFrameButtonStates, s_IOData.ButtonStates, sizeof(s_IOData.ButtonStates));

    // Buttons
    s_IOData.ButtonStates[(int)Button::Enter] = digitalRead(ENTER_BUTTON_PIN);
    s_IOData.ButtonStates[(int)Button::Cancel] = digitalRead(CANCEL_BUTTON_PIN);


    // Joystick
    s_IOData.JoystickPosition.X = analogRead(JOYSTICK_X_PIN) - 512;
    s_IOData.JoystickPosition.Y = analogRead(JOYSTICK_Y_PIN) - 512;

    // Set four directions to false
    memset(s_IOData.ButtonStates + (int)Button::JoystickUp, 0, 4);

    // Determine joystick direction
    if (abs(s_IOData.JoystickPosition.X) > abs(s_IOData.JoystickPosition.Y))
    {
        if (s_IOData.JoystickPosition.X > 256)
        {
            s_IOData.JoystickDirection = { 1, 0 };
            s_IOData.ButtonStates[(int)Button::JoystickRight] = true;
        }
        else if (s_IOData.JoystickPosition.X < -256)
        {
            s_IOData.JoystickDirection = { -1, 0 };
            s_IOData.ButtonStates[(int)Button::JoystickLeft] = true;
        }
        else
            s_IOData.JoystickDirection = { 0, 0 };
    }
    else
    {
        if (s_IOData.JoystickPosition.Y > 256)
        {
            s_IOData.JoystickDirection = { 0, 1 };
            s_IOData.ButtonStates[(int)Button::JoystickUp] = true;
        }
        else if (s_IOData.JoystickPosition.Y < -256)
        {
            s_IOData.JoystickDirection = { 0, -1 };
            s_IOData.ButtonStates[(int)Button::JoystickDown] = true;
        }
        else
            s_IOData.JoystickDirection = { 0, 0 };
    }
}

bool IO::IsButtonPressed(Button button)
{
    return s_IOData.ButtonStates[(int)button];
}

bool IO::IsButtonDown(Button button)
{
    return !s_IOData.LastFrameButtonStates[(int)button] && s_IOData.ButtonStates[(int)button];
}

bool IO::IsButtonUp(Button button)
{
    return s_IOData.LastFrameButtonStates[(int)button] && !s_IOData.ButtonStates[(int)button];
}

Vector2Int IO::GetJoystickPosition()
{
    return s_IOData.JoystickPosition;
}

Vector2Int IO::GetJoystickDirection()
{
    return s_IOData.JoystickDirection;
}

void IO::ClearDisplay()
{
    s_IOData.LCD.clear();
}

void IO::DisplayMessage(int line, const char* message)
{
    // Copy string to buffer
    memset(s_IOData.WriteBuffer, ' ', 20);
    memcpy(s_IOData.WriteBuffer, message, min(20, strlen(message))); 

    s_IOData.LCD.setCursor(0, line);
    s_IOData.LCD.print(s_IOData.WriteBuffer);
}