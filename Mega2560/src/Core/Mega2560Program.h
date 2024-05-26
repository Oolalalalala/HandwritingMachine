#ifndef MEGA2560_PROGRAM_H
#define MEGA2560_PROGRAM_H

#include "Timer.h"
#include "WritingMachine.h"

class Mega2560Program
{
public:
    Mega2560Program() = default;
    ~Mega2560Program() = default;

    void Initialize();
    void OnUpdate();

private:

    enum class State
    {
        MainMenu,
        Writing,
        TicTacToe
    } m_State;

    struct MainMenuData
    {
        int SelectedIndex = 0;
        int ViewWindowBegin = 0;
    } m_MainMenuData;

    void OnMainMenuEnter();
    void OnMainMenuUpdate();

private:
    Timer m_Timer;
    WritingMachine m_WritingMachine;
};

#endif 