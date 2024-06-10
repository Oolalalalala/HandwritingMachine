#ifndef ESP32_PROGRAM_H
#define ESP32_PROGRAM_H

#include "../Utils/Timer.h"
#include "WritingMachine.h"

class Esp32Program
{
public:
    Esp32Program();
    ~Esp32Program() = default;

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
        bool RequireRefreshScreen = false;
        unsigned long LastInputTime = 0;
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