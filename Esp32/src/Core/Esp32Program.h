#ifndef ESP32_PROGRAM_H
#define ESP32_PROGRAM_H

#include "../Utils/Timer.h"
#include "../State/State.h"
#include "WritingMachine.h"

class ESP32Program
{
public:
    ESP32Program();
    ~ESP32Program() = default;

    void Initialize(bool disableCamera);
    void OnUpdate();

    bool CameraDisabled();

public:
    // Singleton
    static ESP32Program& Get() { return *s_Instance; }
    void SwitchState(State state) { m_NextState = state;}

    WritingMachine& GetWritingMachine() { return m_WritingMachine; }
    CoreXY& GetCoreXY() { return m_CoreXY; }
    PenHolder& GetPenHolder() { return m_PenHolder; }

private:
    // Singleton
    static ESP32Program* s_Instance; 

private:
    Timer m_Timer;

    State m_State;
    State m_NextState; // For switching state

    WritingMachine m_WritingMachine;
    CoreXY m_CoreXY;
    PenHolder m_PenHolder;
};

#endif 