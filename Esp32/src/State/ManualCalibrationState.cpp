#include "ManualCalibrationState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"


#define PEN_SPEED 20 // (mm/s)
#define SEGMENT_TIME 0.1f // (s)
#define JOYSTICK_DEADZONE 512



void ManualCalibrationState::OnEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "-Calibration Mode");
    IO::DisplayMessage(1, "-Enter to set origin");
    IO::DisplayMessage(2, "(Bottom-left corner)");
    IO::DisplayMessage(3, "-Cancel to exit");
}

void ManualCalibrationState::OnUpdate(float dt)
{
    IO::PullData();

    auto& coreXY = ESP32Program::Get().GetCoreXY();

    if (IO::IsButtonDown(Button::Enter))
    {
        coreXY.SetAbsolutePosition({0.0f, 0.0f});
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    Vector2Int joystick = IO::GetJoystickPosition();

    // Deadzone
    if (joystick.X > JOYSTICK_DEADZONE)
        joystick.X -= JOYSTICK_DEADZONE;
    else if (joystick.X < -JOYSTICK_DEADZONE)
        joystick.X += JOYSTICK_DEADZONE;
    else
        joystick.X = 0;

    if (joystick.Y > JOYSTICK_DEADZONE)
        joystick.Y -= JOYSTICK_DEADZONE;
    else if (joystick.Y < -JOYSTICK_DEADZONE)
        joystick.Y += JOYSTICK_DEADZONE;
    else
        joystick.Y = 0;
    
    if (joystick.X == 0 && joystick.Y == 0)
        return;


    // Map square to circle
    float length = Length(joystick);
    float ratio = max(abs(joystick.X), abs(joystick.Y)) / length; // Used to normalize (due to square to circle mapping)

    Vector2 delta = Vector2(joystick.X, joystick.Y) * ratio;
    delta = delta / (2048 - JOYSTICK_DEADZONE) * PEN_SPEED * SEGMENT_TIME;

    coreXY.Move(delta, 1000000.0f * SEGMENT_TIME);
    coreXY.WaitFinish();
}

void ManualCalibrationState::OnExit()
{

}