#include "ConnectToWifiState.h"

#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"
#include "../Core/WifiInterface.h"


struct ConnectToWifiStateData
{
    const char* Ssid = "oolala";
    const char* Password = "hahahahaha";
    float Timer;
    int DotCount;
    bool AlreadyConnected;
};

static ConnectToWifiStateData s_Data;

void ConnectToWifiState::OnEnter()
{
    IO::ClearDisplay();

    s_Data.AlreadyConnected = WifiInterface::IsConnected();

    if (s_Data.AlreadyConnected)
    {
        IO::DisplayMessage(0, "Enter to reconnect");
        IO::DisplayMessage(1, "Cancel to exit");
    }
    else
    {
        IO::DisplayMessage(0, "Connecting");

        char ssidStr[21] = "SSID: ";
        strncat(ssidStr, s_Data.Ssid, 14);

        IO::DisplayMessage(1, s_Data.Ssid);

        WifiInterface::Initialize();
        WifiInterface::TryConnect(s_Data.Ssid, s_Data.Password);

        s_Data.Timer = 0.0f;
        s_Data.DotCount = 0;
    }
}

void ConnectToWifiState::OnUpdate(float dt)
{
    if (s_Data.AlreadyConnected)
    {
        if (IO::IsButtonDown(Button::Enter))
        {
            WifiInterface::Disconnect();
            WifiInterface::Connect(s_Data.Ssid, s_Data.Password);
            return;
        }

        if (IO::IsButtonDown(Button::Cancel))
        {
            ESP32Program::Get().SwitchState(State::Menu);
            return;
        }
    }
    else
    {
        if (WifiInterface::IsConnected())
        {
            IO::DisplayMessage(0, "WiFi Connected");
            delay(1000);
            ESP32Program::Get().SwitchState(State::Menu);
            return;
        }

        if (IO::IsButtonDown(Button::Cancel))
        {
            ESP32Program::Get().SwitchState(State::Menu);
            return;
        }

        s_Data.Timer += dt;

        if (s_Data.Timer > 1.0f)
        {
            s_Data.Timer -= 1.0f;
            s_Data.DotCount = (s_Data.DotCount + 1) % 4;

            char message[21] = "Connecting";
            for (int i = 0; i < s_Data.DotCount; i++)
                message[10 + i] = '.';

            IO::DisplayMessage(0, message);
        }
    }
}

void ConnectToWifiState::OnExit()
{
}