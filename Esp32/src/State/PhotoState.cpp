#include "PhotoState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"
#include "../Core/Camera.h"
#include "../Core/WifiInterface.h"


#define IMAGE_HEADER_BYTE 'h'
#define IMAGE_DATA_BYTE 'i'

#define PEN_SPEED 20 // (mm/s)
#define SEGMENT_TIME 0.1f // (s)
#define JOYSTICK_DEADZONE 100

struct PhotoStateData
{
    unsigned long LastPhotoTime = 0; // (ms)
    bool HasClient;
};

static PhotoStateData s_Data;

void PhotoState::OnEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "-Photo Mode");

    s_Data.HasClient = WifiInterface::HasClient();
    if (s_Data.HasClient)
    {
        IO::DisplayMessage(1, "-Enter to take photo");
        IO::DisplayMessage(2, "-Cancel to exit");
    }
    else
    {
        IO::DisplayMessage(1, "-No client connected");
        IO::DisplayMessage(2, "-Cancel to exit");
    
    }
}

void PhotoState::OnUpdate(float dt)
{
    IO::PullData();

    // Exit mode
    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    if (!s_Data.HasClient)
        return;

    // Take and send photo
    if (IO::IsButtonDown(Button::Enter) && millis() - s_Data.LastPhotoTime > 1000) // 1 second interval
    {   
        Camera::Capture();
        camera_fb_t* fb = Camera::GetFramebuffer();

        s_Data.LastPhotoTime = millis();

        if (fb == nullptr)
        {
            IO::DisplayMessage(3, "Failed to take photo");
            return;
        }
        else
        {
            IO::DisplayMessage(3, "Photo taken");
        }

        // Send photo
        uint8_t header[5];
        header[0] = 'p';
        header[1] = fb->width & 0xFF;
        header[2] = fb->width >> 8;
        header[3] = fb->height & 0xFF;
        header[4] = fb->height >> 8;

        WifiInterface::SendBytesToClient(header, 5);
        WifiInterface::SendBytesToClient((uint8_t*)fb->buf, fb->len);

        Camera::ReleaseFramebuffer();
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
    delta = delta / (512 - JOYSTICK_DEADZONE) * PEN_SPEED * SEGMENT_TIME;

    auto& coreXY = ESP32Program::Get().GetCoreXY();
    coreXY.Move(delta, 1000000.0f * SEGMENT_TIME);
    coreXY.WaitFinish();
}

void PhotoState::OnExit()
{

}