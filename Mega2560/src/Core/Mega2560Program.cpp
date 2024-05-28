#include "Mega2560Program.h"

#include "IO.h"
#include "../Utils/Math.h"
#include <Arduino.h>

#define CALIBRATION_MODE_SPEED 50 // (mm/s)
#define CALIBRATION_JOYSTICK_DEADZONE 10

static char* s_MainMenuOptions[] = { " Writing", " Manual Writing", " Tic Tac Toe", " Auto Calibration", " Manual Calibration" };
static const int s_MainMenuOptionCount = 4;
static const char s_ArrowCharacter = 0x7E;

Mega2560Program::Mega2560Program()
    : m_State(State::MainMenu), m_WritingMachine(m_CoreXY, m_PenHolder)
{
}

void Mega2560Program::Initialize()
{
    IO::Initialize();
    m_CoreXY.Initialize();
    m_PenHolder.Initialize();
    m_WritingMachine.Initialize();

    m_CoreXY.Enable();

    m_State = State::Writing; // Test

    Serial.begin(115200); // For testing
}

void Mega2560Program::OnUpdate()
{
    float dt = m_Timer.Tick();
    
    switch (m_State)
    {
        case State::MainMenu:
        {   
            OnMainMenuUpdate();
            break;
        }
        case State::ManualCalibration:
        {
            OnManualCalibrationUpdate(dt);
            break;
        }
        case State::Writing:
        {
            OnWritingUpdate(dt);
            break;
        }
    }
}

void Mega2560Program::OnMainMenuEnter()
{
    m_MainMenuData.SelectedIndex = 0;
    m_MainMenuData.ViewWindowBegin = 0;
}

void Mega2560Program::OnMainMenuUpdate()
{
    IO::PullData();
    
    if (IO::IsButtonDown(Button::Enter))
    {
        switch (m_MainMenuData.SelectedIndex)
        {
            case 0: // Switch to writing state
            {
                m_State = State::Writing;
                return;
            }
            case 1: // Switch to manual writing state
            {
                m_State = State::ManualWriting;
                return;
            }
            case 2: // Switch to tic tac toe state
            {
                m_State = State::TicTacToe;
                return;
            }
            case 3: // Switch to auto calibration state
            {
                m_State = State::AutoCalibration;
                return;
            }
            case 4: // Switch to manual calibration state
            {
                m_State = State::ManualCalibration;
                OnManualCalibrationEnter();
                return;
            }
        }
    }
    

    if (IO::IsButtonDown(Button::JoystickUp))
    {
        m_MainMenuData.SelectedIndex = min(s_MainMenuOptionCount - 1, m_MainMenuData.SelectedIndex + 1);
        if (m_MainMenuData.SelectedIndex == m_MainMenuData.ViewWindowBegin + 4)
            m_MainMenuData.ViewWindowBegin++;
    }

    if (IO::IsButtonDown(Button::JoystickDown))
    {
        m_MainMenuData.SelectedIndex = max(0, m_MainMenuData.SelectedIndex - 1);
        if (m_MainMenuData.SelectedIndex == m_MainMenuData.ViewWindowBegin - 1)
            m_MainMenuData.ViewWindowBegin--;
    }

    for (int i = 0; i < 4; i++)
    {
        int optionIndex = m_MainMenuData.ViewWindowBegin + i;

        if (optionIndex == s_MainMenuOptionCount)
            break;

        if (optionIndex == m_MainMenuData.SelectedIndex)
        {
            s_MainMenuOptions[optionIndex][0] = s_ArrowCharacter;
            IO::DisplayMessage(i, s_MainMenuOptions[optionIndex]);
        }
        else
        {
            s_MainMenuOptions[optionIndex][0] = ' ';
            IO::DisplayMessage(i, s_MainMenuOptions[optionIndex]);
        }
    }
}

void Mega2560Program::OnManualCalibrationEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "-Calibration Mode");
    IO::DisplayMessage(1, "-Enter to set origin");
    IO::DisplayMessage(2, "(Bottom-left corner)");
    IO::DisplayMessage(3, "-Cancel to exit");
}

void Mega2560Program::OnManualCalibrationUpdate(float dt)
{
    IO::PullData();


    if (IO::IsButtonDown(Button::Enter))
    {
        m_CoreXY.SetOrigin();
        m_State = State::MainMenu;
        return;
    }

    if (IO::IsButtonDown(Button::Cancel))
    {
        m_State = State::MainMenu;
        return;
    }


    Vector2Int joystick = IO::GetJoystickPosition();
    
    // Deadzone
    if (joystick.X > CALIBRATION_JOYSTICK_DEADZONE)
        joystick.X -= CALIBRATION_JOYSTICK_DEADZONE;
    else if (joystick.X < -CALIBRATION_JOYSTICK_DEADZONE)
        joystick.X += CALIBRATION_JOYSTICK_DEADZONE;
    else
        joystick.X = 0;

    if (joystick.Y > CALIBRATION_JOYSTICK_DEADZONE)
        joystick.Y -= CALIBRATION_JOYSTICK_DEADZONE;
    else if (joystick.Y < -CALIBRATION_JOYSTICK_DEADZONE)
        joystick.Y += CALIBRATION_JOYSTICK_DEADZONE;
    else
        joystick.Y = 0;


    Vector2 delta = Vector2((float)joystick.X, (float)joystick.Y) / Length(joystick) * CALIBRATION_MODE_SPEED * dt;

    m_CoreXY.Move(delta, dt);
    m_CoreXY.WaitFinish();
}

void Mega2560Program::OnManualWritingEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "-Manual Writing Mode");
    IO::DisplayMessage(1, "-Enter to toggle pen");
    IO::DisplayMessage(2, "-Cancel to exit");
}

void Mega2560Program::OnManualWritingUpdate(float dt)
{
    IO::PullData();

    if (IO::IsButtonDown(Button::Enter))
    {
        if (m_PenHolder.Contacting())
            m_PenHolder.Lift();
        else if (m_PenHolder.Hovering())
            m_PenHolder.Drop();
    }

    if (IO::IsButtonDown(Button::Cancel))
    {
        m_State = State::MainMenu;
        return;
    }

    Vector2Int joystick = IO::GetJoystickPosition();
    
    // Deadzone
    if (joystick.X > CALIBRATION_JOYSTICK_DEADZONE)
        joystick.X -= CALIBRATION_JOYSTICK_DEADZONE;
    else if (joystick.X < -CALIBRATION_JOYSTICK_DEADZONE)
        joystick.X += CALIBRATION_JOYSTICK_DEADZONE;
    else
        joystick.X = 0;

    if (joystick.Y > CALIBRATION_JOYSTICK_DEADZONE)
        joystick.Y -= CALIBRATION_JOYSTICK_DEADZONE;
    else if (joystick.Y < -CALIBRATION_JOYSTICK_DEADZONE)
        joystick.Y += CALIBRATION_JOYSTICK_DEADZONE;
    else
        joystick.Y = 0;


    Vector2 delta = Vector2((float)joystick.X, (float)joystick.Y) / Length(joystick) * CALIBRATION_MODE_SPEED * dt;

    m_CoreXY.Move(delta, dt);
    m_CoreXY.WaitFinish();
}

void Mega2560Program::OnWritingEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "-Writing Mode");
}

void Mega2560Program::OnWritingUpdate(float dt)
{
    IO::PullData();

    m_WritingMachine.OnUpdate(dt);
}