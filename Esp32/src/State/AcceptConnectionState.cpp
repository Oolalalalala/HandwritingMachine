#include "AcceptConnectionState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"
#include "../Core/WifiInterface.h"

struct AcceptConnectionData
{
    bool HasWifiConnection; 
};

static AcceptConnectionData s_Data;

void AcceptConnectionState::OnEnter()
{
    IO::ClearDisplay();

    s_Data.HasWifiConnection = WifiInterface::IsConnected();

    if (s_Data.HasWifiConnection)
    {
        if (WifiInterface::HasPreviousConnection())
            IO::DisplayMessage(0, "Client connected");
        else
            IO::DisplayMessage(0, "Waiting for client");

        String ip = WifiInterface::GetIPAddress().toString();
        IO::DisplayMessage(1, "Server IP: ");
        IO::DisplayMessage(2, ip.c_str());
        IO::DisplayMessage(3, "Port: 8000");

        if (!WifiInterface::ServerEnabled())
            WifiInterface::BeginServer();
    }
    else
    {
        IO::DisplayMessage(0, "Wifi not connected");
    }

}

void AcceptConnectionState::OnUpdate(float dt)
{
    IO::PullData();

    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    if (!s_Data.HasWifiConnection)
        return;

    bool result = WifiInterface::TryClientConnection();

    if (result)
    {
        IO::DisplayMessage(0, "Client connected");
        WifiInterface::DumpClient();
    }
}

void AcceptConnectionState::OnExit()
{
}