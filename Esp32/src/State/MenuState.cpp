#include "MenuState.h"

#include <Arduino.h>
#include "../Core/Esp32Program.h"
#include "../Core/IO.h"


static char s_MenuItems[][21] = { " Modes",                        // 0
                                      " Control By PC",            // 1
                                      " Local Modes",              // 2
                                          " Manual Writing",       // 3
                                          " Tic Tac Toe",          // 4
                                      " Internet Modes",           // 5
                                          " Take Photo",           // 6
                                          " ???",                  // 7 
                                  " Calibration",                  // 8
                                      " Set Draw Region(A4)",      // 9
                                      " Auto Calibration",         // 10 
                                      " Manual Calibration",       // 11 
                                  " Settings",                     // 12
                                      " Connect to WIFI",          // 13
                                      " Accept Connection"         // 14
                                };

static const int s_NextItemIndex[] = { 8, 2, 5, 4, 2, 0, 7, 5, 12, 10, 11, 8, 0, 14, 12 }; // Set next item to parent item for the last items
static const int s_LastItemIndex[] = { -1, -1, 1, -1, 3, 2, -1, 6, 0, -1, 9, 10, 8, -1, 13 }; // Set last item to -1 for the first items
static const int s_MainMenuOptionCount = sizeof(s_MenuItems) / sizeof(s_MenuItems[0]);
#define ARROW_CHARACTER 0x7E
#define MINIMUM_INPUT_INTERVAL 250 // (ms)


struct MenuStateData
{
    int SelectedIndex;
    int ViewWindowBegin;
    bool RequireRefreshScreen;
    unsigned long LastInputTime;
};

static MenuStateData s_Data;

void MenuState::OnEnter()
{
    s_Data.SelectedIndex = 0;
    s_Data.ViewWindowBegin = 0;
    s_Data.RequireRefreshScreen = true;
    s_Data.LastInputTime = millis();
}

void MenuState::OnUpdate(float dt)
{
    IO::PullData();
    
    if (!s_Data.RequireRefreshScreen && millis() - s_Data.LastInputTime < MINIMUM_INPUT_INTERVAL)
        return;


    if (IO::IsButtonDown(Button::Enter))
    {
        s_Data.LastInputTime = millis();

        if (s_Data.SelectedIndex == 1)
        {
            ESP32Program::Get().SwitchState(State::PCControl);
            return;
        }
        
        if (s_Data.SelectedIndex == 3)
        {
            ESP32Program::Get().SwitchState(State::ManualWriting);
            return;
        }

        if (s_Data.SelectedIndex == 4)
        {
            ESP32Program::Get().SwitchState(State::TicTacToe);
            return;
        }

        if (s_Data.SelectedIndex == 6)
        {
            ESP32Program::Get().SwitchState(State::Photo);
            return;
        }

        if (s_Data.SelectedIndex == 9)
        {
            ESP32Program::Get().SwitchState(State::SetDrawRegion);
            return;
        }

        if (s_Data.SelectedIndex == 9)
        {
            ESP32Program::Get().SwitchState(State::SetDrawRegion);
            return;
        }

        if (s_Data.SelectedIndex == 10)
        {
            ESP32Program::Get().SwitchState(State::AutoCalibration);
            return;
        }

        if (s_Data.SelectedIndex == 11)
        {
            ESP32Program::Get().SwitchState(State::ManualCalibration);
            return;
        }
        
        if (s_Data.SelectedIndex == 13)
        {
            ESP32Program::Get().SwitchState(State::ConnectToWifi);
            return;
        }

        if (s_Data.SelectedIndex == 14)
        {
            ESP32Program::Get().SwitchState(State::AcceptConnection);
            return;
        }

        // Go to sub menu
        s_Data.SelectedIndex++;
        s_Data.SelectedIndex %= s_MainMenuOptionCount;
        s_Data.ViewWindowBegin = s_Data.SelectedIndex;
        s_Data.RequireRefreshScreen = true;
    }

    if (IO::IsButtonDown(Button::Cancel))
    {
        s_Data.LastInputTime = millis();
        
        int parentIndex = s_Data.SelectedIndex;

        while (s_NextItemIndex[parentIndex] > parentIndex)
        {
            parentIndex = s_NextItemIndex[parentIndex];
        } 

        parentIndex = s_NextItemIndex[parentIndex];
        s_Data.SelectedIndex = parentIndex;

        s_Data.ViewWindowBegin = parentIndex;
        while (s_LastItemIndex[s_Data.ViewWindowBegin] != -1)
            s_Data.ViewWindowBegin = s_LastItemIndex[s_Data.ViewWindowBegin];

        while (true)
        {
            int viewWindowEnd = s_Data.ViewWindowBegin;
            for (int i = 0; i < 3; i++)
            {
                if (s_NextItemIndex[viewWindowEnd] < viewWindowEnd)
                    break;

                viewWindowEnd = s_NextItemIndex[viewWindowEnd];
            }

            if (viewWindowEnd < s_Data.SelectedIndex)
                s_Data.ViewWindowBegin = s_NextItemIndex[s_Data.ViewWindowBegin];
            else
                break;
        }
        
        s_Data.RequireRefreshScreen = true;
    }
    
    if (IO::IsButtonDown(Button::JoystickUp))
    {
        s_Data.LastInputTime = millis();
        
        if (s_LastItemIndex[s_Data.SelectedIndex] != -1)
            s_Data.SelectedIndex = s_LastItemIndex[s_Data.SelectedIndex];

        if (s_Data.ViewWindowBegin > s_Data.SelectedIndex)
            s_Data.ViewWindowBegin = s_Data.SelectedIndex;

        s_Data.RequireRefreshScreen = true;
    }

    if (IO::IsButtonDown(Button::JoystickDown))
    {
        s_Data.LastInputTime = millis();
        
        if (s_NextItemIndex[s_Data.SelectedIndex] > s_Data.SelectedIndex)
            s_Data.SelectedIndex = s_NextItemIndex[s_Data.SelectedIndex];

        int viewWindowEnd = s_Data.ViewWindowBegin;
        for (int i = 0; i < 3; i++)
        {
            if (s_NextItemIndex[viewWindowEnd] < viewWindowEnd)
                break;

            viewWindowEnd = s_NextItemIndex[viewWindowEnd];
        }

        if (viewWindowEnd < s_Data.SelectedIndex)
            s_Data.ViewWindowBegin = s_NextItemIndex[s_Data.ViewWindowBegin];

        s_Data.RequireRefreshScreen = true;
    }


    if (!s_Data.RequireRefreshScreen)
        return;
        
    IO::ClearDisplay();

    for (int i = 0, optionIndex = s_Data.ViewWindowBegin; i < 4; i++, optionIndex = s_NextItemIndex[optionIndex])
    {
        if (optionIndex == s_Data.SelectedIndex)
        {
            s_MenuItems[optionIndex][0] = ARROW_CHARACTER;
            IO::DisplayMessage(i, s_MenuItems[optionIndex]);
        }
        else
        {
            s_MenuItems[optionIndex][0] = ' ';
            IO::DisplayMessage(i, s_MenuItems[optionIndex]);
        }

        if (optionIndex > s_NextItemIndex[optionIndex])
            break;
    }

    s_Data.RequireRefreshScreen = false;
}

void MenuState::OnExit()
{
    
}