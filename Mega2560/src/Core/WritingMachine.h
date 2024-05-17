#ifndef WRITING_MACHINE_H
#define WRITING_MACHINE_H

#include "CommandBuffer.h"

class WritingMachine
{
public:
    WritingMachine(Vector2 initialPenPosition);
    ~WritingMachine() = default;

    void SetCommandBuffer(CommandBuffer& commandBuffer); // Draw data from the command buffer

    void OnUpdate(float dt);
    void Execute();
    void Pause();
    bool IsIdle();

private:
    struct Config
    {
        float PenLiftThresholdDistance = 0.0f;
        float PenWriteVelocity = 0.0f;
        float PenMoveVelocity = 0.0f;
    } m_Config;
    
    Vector2 m_PenPosition;
    int m_CommandIndex;

    CommandBuffer* m_CommandBuffer = nullptr;
};

#endif