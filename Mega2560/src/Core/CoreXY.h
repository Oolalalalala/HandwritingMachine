#ifndef COREXY_H
#define COREXY_H


#include <Arduino.h>
#include <A4988.h> // A4988 stepper motor driver
#include "../Vendor/TimedSyncDriver.h" // Sync control for two stepper motors
#include "../Utils/Math.h"

class CoreXY
{
public:
    void Initialize();
    void OnUpdate(); // Should be called as frequently as possible

    void Move(Vector2 position, long duration); // Position in (mm)
    bool IsMoving();

private:
    Vector2Int CalculateStepperCoordinate(Vector2 position); // (mm) to (steps)

private:
    Vector2 m_Position; // (mm)
    Vector2Int m_StepperCoordinate; // (steps)
};

#endif 