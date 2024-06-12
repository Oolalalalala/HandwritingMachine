#include "ShowServerIPState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"
#include "../Core/WifiInterface.h"

void ShowServerIPState::OnEnter()
{
    IO::ClearDisplay();

    if (WifiInterface::IsConnected())
    {
        if (WifiInterface::HasClient())
            IO::DisplayMessage(0, "Client connected");
        else
            IO::DisplayMessage(0, "Waiting for client");

        const char* ip = WifiInterface::GetIPAddress().toString().c_str();
        IO::DisplayMessage(1, "Server IP: ");
        IO::DisplayMessage(2, ip);
        IO::DisplayMessage(3, "Port: 8000");
    }
    else
    {
        IO::DisplayMessage(0, "Wifi not connected");
    }

}

void ShowServerIPState::OnUpdate(float dt)
{
    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }
}

void ShowServerIPState::OnExit()
{

}