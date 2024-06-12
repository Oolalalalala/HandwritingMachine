#include "ESP32Program.h"

#include "IO.h"
#include "../Utils/Math.h"
#include <Arduino.h>

#include "Camera.h"
#include "WifiInterface.h"
#include "../State/AutoCalibrationState.h"
#include "../State/ConnectToWIFIState.h"
#include "../State/ManualCalibrationState.h"
#include "../State/ManualWritingState.h"
#include "../State/MenuState.h"
#include "../State/PCControlState.h"
#include "../State/PhotoState.h"
#include "../State/SetDrawRegionState.h"
#include "../State/AllowConnectionState.h"
#include "../State/TicTacToeState.h"

ESP32Program* ESP32Program::s_Instance = nullptr;

ESP32Program::ESP32Program()
    : m_NextState(State::None), m_WritingMachine(m_CoreXY, m_PenHolder)
{
}

void ESP32Program::Initialize()
{
    IO::Initialize();
    m_CoreXY.Initialize();
    m_PenHolder.Initialize();
    m_WritingMachine.Initialize();

    Camera::Initialize();
    delay(2000); // Wait for camera to initialize

    WifiInterface::Initialize();

    s_Instance = this;

    m_State = State::Menu;
    MenuState::OnEnter();

    //auto& commandBuffer = m_WritingMachine.GetCommandBuffer();
    //commandBuffer.DrawLine({0.0f, 0.0f}, {50.0f, 100.0f});
    //commandBuffer.DrawQuadraticBezier({0.0f, 0.0f}, {0.0f, 100.0f}, {100.0f, 100.0f});
    //commandBuffer.DrawArc({50.0f, 0.0f}, 50.0f, PI, 3 * PI);
    //commandBuffer.DrawArc({30.0f, 0.0f}, 30.0f, PI, 2 * PI);
    //commandBuffer.DrawArc({80.0f, 0.0f}, 20.0f, PI, 3 * PI);
    //commandBuffer.DrawArc({30.0f, 0.0f}, 30.0f, 2 * PI, 3 * PI);

}

void ESP32Program::OnUpdate()
{
    float dt = m_Timer.Tick();

    switch (m_State)
    {
        case State::Menu:
        {   
            MenuState::OnUpdate(dt);
            break;
        }
        case State::PCControl:
        {
            PCControlState::OnUpdate(dt);
            break;
        }
        case State::ManualWriting:
        {
            ManualWritingState::OnUpdate(dt);
            break;
        }
        case State::TicTacToe:
        {
            TicTacToeState::OnUpdate(dt);
            break;
        }
        case State::Photo:
        {
            PhotoState::OnUpdate(dt);
            break;
        }
        case State::ConnectToWifi:
        {
            ConnectToWifiState::OnUpdate(dt);
            break;
        }
        case State::AutoCalibration:
        {
            AutoCalibrationState::OnUpdate(dt);
            break;
        }
        case State::ManualCalibration:
        {
            ManualCalibrationState::OnUpdate(dt);
            break;
        }
        case State::SetDrawRegion:
        {
            SetDrawRegionState::OnUpdate(dt);
            break;
        }
        case State::ShowServerIP:
        {
            AllowConnectionState::OnUpdate(dt);
            break;
        }
    }

    // Check switch state
    if (m_NextState != State::None)
    {
        switch (m_State)
        {
            case State::Menu:
            {
                MenuState::OnExit();
                break;
            }
            case State::PCControl:
            {
                PCControlState::OnExit();
                break;
            }
            case State::ManualWriting:
            {
                ManualWritingState::OnExit();
                break;
            }
            case State::TicTacToe:
            {
                TicTacToeState::OnExit();
                break;
            }
            case State::Photo:
            {
                PhotoState::OnExit();
                break;
            }
            case State::ConnectToWifi:
            {
                ConnectToWifiState::OnExit();
                break;
            }
            case State::AutoCalibration:
            {
                AutoCalibrationState::OnExit();
                break;
            }
            case State::ManualCalibration:
            {
                ManualCalibrationState::OnExit();
                break;
            }
            case State::SetDrawRegion:
            {
                SetDrawRegionState::OnExit();
                break;
            }
            case State::ShowServerIP:
            {
                AllowConnectionState::OnExit();
                break;
            }
        }

        switch (m_NextState)
        {
            case State::Menu:
            {
                MenuState::OnEnter();
                break;
            }
            case State::PCControl:
            {
                PCControlState::OnEnter();
                break;
            }
            case State::ManualWriting:
            {
                ManualWritingState::OnEnter();
                break;
            }
            case State::TicTacToe:
            {
                TicTacToeState::OnEnter();
                break;
            }
            case State::Photo:
            {
                PhotoState::OnEnter();
                break;
            }
            case State::ConnectToWifi:
            {
                ConnectToWifiState::OnEnter();
                break;
            }
            case State::AutoCalibration:
            {
                AutoCalibrationState::OnEnter();
                break;
            }
            case State::ManualCalibration:
            {
                ManualCalibrationState::OnEnter();
                break;
            }
            case State::SetDrawRegion:
            {
                SetDrawRegionState::OnEnter();
                break;
            }
            case State::ShowServerIP:
            {
                AllowConnectionState::OnEnter();
                break;
            }
        }

        m_State = m_NextState;
        m_NextState = State::None;
    }
}