#ifndef WRITING_MACHINE_H
#define WRITING_MACHINE_H

#include "CommandBuffer.h"

class WritingMachine
{
public:
    WritingMachine();
    ~WritingMachine() = default;

    void SetInitialPosition(Vector2 position) { m_PenPosition = position; }
    CommandBuffer& GetCommandBuffer() { return m_CommandBuffer; }

    void OnUpdate(float dt);
    void Execute();
    void Pause();
    bool IsIdle();

private:
    struct Config
    {
        float PenLiftThresholdDistance = 0.0f;
        float PenWriteSpeed = 0.0f;
        float PenMoveSpeed = 0.0f;
    } m_Config;
    
    Vector2 m_PenPosition;
    int m_CommandIndex;

    CommandBuffer m_CommandBuffer;
};

#endif