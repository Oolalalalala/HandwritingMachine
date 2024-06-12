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

    void SetAbsolutePosition(Vector2 position); // (mm) Used to calibrate the absolute position
    void SetCurrentPositionOrigin(); // Set the current position as the origin
    void SetMaxSpeed(float speed); // (mm/s)

    void Move(Vector2 delta, long duration); // Delta in (mm)
    void MoveTo(Vector2 position, long duration); // Position in (mm)
    void WaitFinish(); // Block until the current move is finished

    void SetBoundaryCheck(bool enabled) { m_BoundaryCheck = enabled; }
    bool IsMoving();

private:
    Vector2Int CalculateStepperCoordinate(Vector2 position); // (mm) to (steps)
    Vector2 ClampToBoundary(Vector2 position);

private:
    Vector2 m_Origin;
    Vector2 m_Position; // (mm)
    Vector2Int m_StepperCoordinate; // (steps)

    bool m_BoundaryCheck = false;
};

#endif 