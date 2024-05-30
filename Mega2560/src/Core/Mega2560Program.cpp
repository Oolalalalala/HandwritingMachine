#include "Mega2560Program.h"

#include "IO.h"
#include "../Utils/Math.h"
#include <Arduino.h>

#define CALIBRATION_MODE_SPEED 20 // (mm/s)
#define CALIBRATION_SEGMENT_TIME 0.1f // (s)
#define CALIBRATION_JOYSTICK_DEADZONE 100

static char* s_MainMenuOptions[] = { " Writing", " Manual Writing", " Tic Tac Toe", " Auto Calibration", " Manual Calibration" };
static const int s_MainMenuOptionCount = 5;
#define ARROW_CHARACTER 0x7E


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

    auto& commandBuffer = m_WritingMachine.GetCommandBuffer();
    //commandBuffer.DrawLine({0.0f, 0.0f}, {50.0f, 100.0f});
    commandBuffer.DrawQuadraticBezier({0.0f, 0.0f}, {0.0f, 100.0f}, {100.0f, 100.0f});
    commandBuffer.DrawArc({50.0f, 0.0f}, 50.0f, PI, 3 * PI);
    commandBuffer.DrawArc({30.0f, 0.0f}, 30.0f, PI, 2 * PI);
    commandBuffer.DrawArc({80.0f, 0.0f}, 20.0f, PI, 3 * PI);
    commandBuffer.DrawArc({30.0f, 0.0f}, 30.0f, 2 * PI, 3 * PI);

    m_CoreXY.Enable();

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
        case State::ManualWriting:
        {
            OnManualWritingUpdate(dt);
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
                OnWritingEnter();
                return;
            }
            case 1: // Switch to manual writing state
            {
                m_State = State::ManualWriting;
                OnManualWritingEnter();
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
            s_MainMenuOptions[optionIndex][0] = ARROW_CHARACTER;
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

    if (m_CoreXY.IsMoving())
    {
        m_CoreXY.OnUpdate();
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
    
    if (joystick.X == 0 && joystick.Y == 0)
        return;


    // Map square to circle
    float length = Length(joystick);
    float ratio = max(abs(joystick.X), abs(joystick.Y)) / length; // Used to normalize (due to square to circle mapping)

    Vector2 delta = Vector2(joystick.X, joystick.Y) * ratio;
    delta = delta / (512 - CALIBRATION_JOYSTICK_DEADZONE) * CALIBRATION_MODE_SPEED * CALIBRATION_SEGMENT_TIME;

    m_CoreXY.Move(delta, 1000000.0f * CALIBRATION_SEGMENT_TIME);
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

    // Toggle pen Lift/Drop
    if (IO::IsButtonDown(Button::Enter))
    {
        if (m_PenHolder.Contacting())
            m_PenHolder.Lift();
        else if (m_PenHolder.Hovering())
            m_PenHolder.Drop();
    }

    // Exit mode
    if (IO::IsButtonDown(Button::Cancel))
    {
        m_State = State::MainMenu;
        return;
    }

    //if (m_CoreXY.IsMoving())
    //{
    //    //long x = micros();
    //    m_CoreXY.OnUpdate();
    //    //x = micros() - x;
    //    //Serial.praintln(x);
    //    return;
    //}

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
    
    if (joystick.X == 0 && joystick.Y == 0)
        return;

    // Map square to circle
    float length = Length(joystick);
    float ratio = max(abs(joystick.X), abs(joystick.Y)) / length; // Used to normalize (due to square to circle mapping)

    Vector2 delta = Vector2(joystick.X, joystick.Y) * ratio;
    delta = delta / (512 - CALIBRATION_JOYSTICK_DEADZONE) * CALIBRATION_MODE_SPEED * CALIBRATION_SEGMENT_TIME;

    m_CoreXY.Move(delta, 1000000.0f * CALIBRATION_SEGMENT_TIME);
    m_CoreXY.WaitFinish();
}

void Mega2560Program::OnWritingEnter()
{
    IO::ClearDisplay();
    IO::DisplayMessage(0, "-Writing Mode");

    m_WritingMachine.Enable(true);
}

void Mega2560Program::OnWritingUpdate(float dt)
{
    IO::PullData();

    m_WritingMachine.OnUpdate(dt);
}