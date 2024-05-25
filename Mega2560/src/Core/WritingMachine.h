#ifndef WRITING_MACHINE_H
#define WRITING_MACHINE_H

#include "CommandBuffer.h"

class WritingMachine
{
public:
    WritingMachine() = default;
    ~WritingMachine() = default;

    void Initialize();

    CommandBuffer& GetCommandBuffer() { return m_CommandBuffer; }
    void SetInitialPenPosition(Vector2 position) { m_PenPosition = position; } // TODO: finish

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