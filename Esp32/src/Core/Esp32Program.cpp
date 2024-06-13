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
#include "../State/AcceptConnectionState.h"
#include "../State/TicTacToeState.h"

ESP32Program* ESP32Program::s_Instance = nullptr;
static bool s_DisableCamera = false;

bool ESP32Program::CameraDisabled()
{
    return s_DisableCamera;
}

ESP32Program::ESP32Program()
    : m_NextState(State::None), m_WritingMachine(m_CoreXY, m_PenHolder)
{
}

void ESP32Program::Initialize(bool disableCamera)
{
    IO::Initialize();
    m_CoreXY.Initialize();
    m_PenHolder.Initialize();
    m_WritingMachine.Initialize();

    s_DisableCamera = true;
    if (!s_DisableCamera)
    {
        Camera::Initialize();
        delay(1500); // Wait for camera to initialize
    }

    WifiInterface::Initialize();

    s_Instance = this;

    m_State = State::Menu;
    MenuState::OnEnter();

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
        case State::AcceptConnection:
        {
            AcceptConnectionState::OnUpdate(dt);
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
            case State::AcceptConnection:
            {
                AcceptConnectionState::OnExit();
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
            case State::AcceptConnection:
            {
                AcceptConnectionState::OnEnter();
                break;
            }
        }

        m_State = m_NextState;
        m_NextState = State::None;
    }
}