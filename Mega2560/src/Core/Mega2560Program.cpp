#include "Mega2560Program.h"

#include "IO.h"
#include "../Utils/Math.h"
#include <Arduino.h>

static char* s_MainMenuOptions[] = { " Writing", " Tic Tac Toe", " AAA", " BBB", " CCC", " DDD", " EEE", " FFF", " GGG", " HHH", " III"};
static const int s_MainMenuOptionCount = 11;
static const char s_ArrowCharacter = 0x7E;

void Mega2560Program::Initialize()
{
    IO::Initialize();
    m_WritingMachine.Initialize();

    m_State = State::MainMenu;

    Serial.begin(115200); // For testing
}

void Mega2560Program::OnUpdate()
{
    IO::PullData();

    float dt = m_Timer.Tick();
    
    switch (m_State)
    {
        case State::MainMenu:
        {   
            OnMainMenuUpdate();
            break;
        }
    }

    // Test
    m_WritingMachine.OnUpdate(dt);
}

void Mega2560Program::OnMainMenuEnter()
{
    m_MainMenuData.SelectedIndex = 0;
    m_MainMenuData.ViewWindowBegin = 0;
}

void Mega2560Program::OnMainMenuUpdate()
{
    if (IO::IsButtonDown(Button::Enter))
    {
        switch (m_MainMenuData.SelectedIndex)
        {
            case 0:
            {
                m_State = State::Writing;
                return;
            }
            case 1:
            {
                m_State = State::TicTacToe;
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