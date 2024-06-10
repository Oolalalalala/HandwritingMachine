#ifndef WRITING_MACHINE_H
#define WRITING_MACHINE_H

#include "CommandBuffer.h"
#include "CoreXY.h"
#include "PenHolder.h"

class WritingMachine
{
public:
    WritingMachine(CoreXY& coreXY, PenHolder& penHolder);
    ~WritingMachine() = default;

    void Initialize();

    CommandBuffer& GetCommandBuffer() { return m_CommandBuffer; }

    void OnUpdate(float dt);
    void Enable(bool enabled);
    bool IsWriting();

private:
    void NextStroke();

private:
    struct Config
    {
        float StrokeSegmentLength = 1.0f; // (mm)
        float StrokeSpeed = 10.0f; // (mm/s)
        float HoverSpeed = 10.0f; // (mm/s)
    } m_Config;

    long m_StrokeSegmentTime; // (us) Cached value for the time it takes to draw a stroke segment

    CoreXY& m_CoreXY;
    PenHolder& m_PenHolder;
    
    float m_StrokeProgress; // 0.0f - 1.0f
    bool m_Enabled = false;
    bool m_Writing = false;

    MachineCommand m_ExecutingCommand;

    CommandBuffer m_CommandBuffer;
};

#endif