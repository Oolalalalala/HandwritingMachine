#ifndef COREXY_H
#define COREXY_H


#include "../Utils/Math.h"

class CoreXY
{
public:
    void Initialize();
    void OnUpdate(); // Should be called as frequently as possible

    void Enable();
    void Disable();

    void SetOrigin(); // Set the current position as the origin

    void Move(Vector2 delta, long duration); // Delta in (mm)
    void MoveTo(Vector2 position, long duration); // Position in (mm)
    void WaitFinish(); // Block until the current move is finished

    bool IsMoving();

private:
    Vector2Int CalculateStepperCoordinate(Vector2 position); // (mm) to (steps)

private:
    Vector2 m_Origin;
    Vector2 m_Position; // (mm)
    Vector2Int m_StepperCoordinate; // (steps)
};

#endif 