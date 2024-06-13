#include "PCControlState.h"


#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"
#include "../Core/WifiInterface.h"

#define ASK_FOR_COMMAND_BYTE 'a' // Ask for commands when paused, pc will no respond if no commands
#define END_BYTE 'e'

#define RETRY_INTERVAL 1.0f // (s) The time before requesting commands again

struct PCControlData
{
    uint8_t* ReadBuffer;
    unsigned long ReadBufferSize;
    bool HasClient;
    bool Paused;
    float PauseTimer;
};

static PCControlData s_Data;

void PCControlState::OnEnter()
{
    IO::ClearDisplay();

    s_Data.HasClient = WifiInterface::HasPreviousConnection();   
    if (s_Data.HasClient)
    {
        IO::DisplayMessage(0, "PC Controling");
        IO::DisplayMessage(1, "Press Cancel to exit");

        s_Data.ReadBufferSize = CommandBuffer::Capacity() * sizeof(MachineCommand) + 1;
        s_Data.ReadBuffer = (uint8_t*)malloc(s_Data.ReadBufferSize);
        s_Data.Paused = true;
        s_Data.PauseTimer = 0.0f;
    }
    else
    {
        IO::DisplayMessage(0, "No client connected");
        IO::DisplayMessage(1, "Press Cancel to exit");
    }

    s_Data.Paused = true;
    ESP32Program::Get().GetWritingMachine().Enable(true);
}

void PCControlState::OnUpdate(float dt)
{
    IO::PullData();

    if (IO::IsButtonDown(Button::Cancel))
    {
        ESP32Program::Get().SwitchState(State::Menu);

        // Notify client of end
        if (s_Data.HasClient)
        {
            uint8_t endMessage[1];
            endMessage[0] = END_BYTE;
            
            WifiInterface::WaitForClientConnection();
            WifiInterface::SendBytesToClient(endMessage, 1);
            WifiInterface::DumpClient();
        }

        return;
    }

    if (!s_Data.HasClient)
        return;


    if (WifiInterface::IncomingFromClient())
    {
        s_Data.Paused = false;
        s_Data.PauseTimer = 0.0f;

        ReadAndParseCommands();
    }

    if (s_Data.Paused)
    {
        s_Data.PauseTimer += dt;

        // Ask for commands
        if (s_Data.PauseTimer > RETRY_INTERVAL)
        {
            s_Data.PauseTimer = 0.0f;

            // Ask for commands
            uint8_t request[2];
            request[0] = ASK_FOR_COMMAND_BYTE;
            request[1] = (uint8_t)CommandBuffer::Capacity();
            
            WifiInterface::DumpClient();
            WifiInterface::WaitForClientConnection();
            WifiInterface::SendBytesToClient(request, 2);
        }

        return;
    }

    auto& writingMachine = ESP32Program::Get().GetWritingMachine();
    writingMachine.OnUpdate(dt);

    auto& commandBuffer = writingMachine.GetCommandBuffer();
    if (!writingMachine.IsWriting() && commandBuffer.Empty())
    {
        s_Data.Paused = true;
        s_Data.PauseTimer = 0.0f;
    }
}

void PCControlState::OnExit()
{
    auto& writingMachine = ESP32Program::Get().GetWritingMachine();
    writingMachine.GetCommandBuffer().Clear();

    free(s_Data.ReadBuffer);

    ESP32Program::Get().GetCoreXY().LiftPen();
}

void PCControlState::ReadAndParseCommands()
{
    uint8_t* end = WifiInterface::ReadBytesFromClient(s_Data.ReadBuffer, s_Data.ReadBufferSize);
    int commandCount = s_Data.ReadBuffer[0];

    // Error check
    if (end - s_Data.ReadBuffer != 1 + commandCount * sizeof(MachineCommand))
    {
        Serial.println("Error in ReadAndParseCommands");
        ESP32Program::Get().SwitchState(State::Menu);
        return;
    }

    auto& commandBuffer = ESP32Program::Get().GetWritingMachine().GetCommandBuffer();

    MachineCommand command;
    for (int i = 0; i < commandCount; i++)
    {
        memcpy(&command, s_Data.ReadBuffer + 1 + i * sizeof(MachineCommand), sizeof(MachineCommand));
        commandBuffer.PushCommand(command);
    }
}