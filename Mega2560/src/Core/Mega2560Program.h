#ifndef MEGA2560_PROGRAM_H
#define MEGA2560_PROGRAM_H

#include "Timer.h"
#include "WritingMachine.h"

class Mega2560Program
{
public:
    Mega2560Program();
    ~Mega2560Program() = default;

    void Initialize();
    void OnUpdate();

private:

    enum class State
    {
        Menu,
        PCControl,
        ManualWriting,
        TicTacToe,
        AutoCalibration,
        ManualCalibration,
        SetDrawRegion, 
        ConnectToWIFI,
        ShowLocalIP
    } m_State;

    struct MainMenuData
    {
        int SelectedIndex = 0;
        int ViewWindowBegin = 0;
    } m_MainMenuData;

    void OnMainMenuEnter();
    void OnMainMenuUpdate();

    void OnWritingEnter();
    void OnWritingUpdate(float dt);

    void OnManualWritingEnter();
    void OnManualWritingUpdate(float dt);

    void OnManualCalibrationEnter();
    void OnManualCalibrationUpdate(float dt);

private:
    Timer m_Timer;
    WritingMachine m_WritingMachine;

    CoreXY m_CoreXY;
    PenHolder m_PenHolder;
};

#endif 