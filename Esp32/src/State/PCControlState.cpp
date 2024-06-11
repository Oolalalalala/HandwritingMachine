#include "PCControlState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"
#include "../Core/WifiInterface.h"

struct PCControlData
{
    uint8_t* ReadBuffer;
    unsigned long ReadBufferSize;
    bool HasClient;
    bool Paused;
};

static PCControlData s_Data;

void PCControlState::OnEnter()
{
    IO::ClearDisplay();

    s_Data.HasClient = WifiInterface::HasClient();   
    if (s_Data.HasClient)
    {
        IO::DisplayMessage(0, "PC Control");
        IO::DisplayMessage(1, "Press Cancel to exit");

        s_Data.ReadBufferSize = CommandBuffer::Capacity() * sizeof(MachineCommand) + 1;
        s_Data.ReadBuffer = (uint8_t*)malloc(s_Data.ReadBufferSize);
    }
    else
    {
        IO::DisplayMessage(0, "No client connected");
        IO::DisplayMessage(1, "Press Cancel to exit");
    }

    s_Data.Paused = true;
}

void PCControlState::OnUpdate(float dt)
{
    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);

        // Notify client
        if (s_Data.HasClient)
            WifiInterface::SendBytesToClient((uint8_t*)"e", 1);

        return;
    }

    if (!s_Data.HasClient)
        return;

    if (s_Data.Paused)
    {
        if (!WifiInterface::IncomingFromClient())
            return;

        // Client asked to start/resume
        WifiInterface::ReadBytesFromClient(s_Data.ReadBuffer, s_Data.ReadBufferSize);
        if (s_Data.ReadBuffer[0] == 'p')
            s_Data.Paused = false;
    }

    auto& writingMachine = ESP32Program::Get().GetWritingMachine();
    auto& commandBuffer = writingMachine.GetCommandBuffer();

    // Send request for more commands
    if (commandBuffer.Empty())
    {
        uint8_t request[2];
        request[0] = 'r';
        request[1] = (uint8_t)CommandBuffer::Capacity();

        WifiInterface::SendBytesToClient(request, 2);
    }

    // Parse the respond
    uint8_t* end = WifiInterface::ReadBytesFromClient(s_Data.ReadBuffer, s_Data.ReadBufferSize);
    int commandCount = s_Data.ReadBuffer[0];
    
    // 0 to pause, 00 to end state
    if (commandCount == 0)
    {
        if (end == s_Data.ReadBuffer + 1)
            s_Data.Paused = true;
        else
            ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    // Error check
    if (end - s_Data.ReadBuffer != 1 + commandCount * sizeof(MachineCommand))
    {
        WifiInterface::SendBytesToClient((uint8_t*)"e", 1);
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }


    MachineCommand command;
    for (int i = 0; i < commandCount; i++)
    {
        memcpy(&command, s_Data.ReadBuffer + 1 + i * sizeof(MachineCommand), sizeof(MachineCommand));
        commandBuffer.PushCommand(command);
    }


    writingMachine.OnUpdate(dt);
}

void PCControlState::OnExit()
{
    auto& writingMachine = ESP32Program::Get().GetWritingMachine();
    writingMachine.GetCommandBuffer().Clear();

    free(s_Data.ReadBuffer);
}