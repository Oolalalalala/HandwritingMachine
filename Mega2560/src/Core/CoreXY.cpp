#include "CoreXY.h"

#include <Arduino.h>

#define STEPPER_A_DIR_PIN 31
#define STEPPER_A_STEP_PIN 33
#define STEPPER_A_ENABLE_PIN 35
#define STEPPER_B_DIR_PIN 37
#define STEPPER_B_STEP_PIN 39
#define STEPPER_B_ENABLE_PIN 41

#define STEPPER_STEPS_PER_REVOLUTION 200
#define STEPPER_MICROSTEPS 1 // Set ms1, ms2, ms3 of A4988 to low
#define STEPPER_WHEEL_RADIUS 5.0f // (mm)
#define DISPLACEMENT_PER_STEP (STEPPER_WHEEL_RADIUS * 2 * PI / STEPPER_STEPS_PER_REVOLUTION / STEPPER_MICROSTEPS) // (mm)
#define CONSECUTIVE_NEXT_ACTION_INTERVALS 0 // (us), if next action interval is less than this, we keep updating the steppers

#define CAP_RPM 200.0f // The max speed of the stepper motor, when too low, may cause the motors to desync

struct CoreXYData
{
    A4988 StepperMotorA = A4988(STEPPER_STEPS_PER_REVOLUTION, STEPPER_A_DIR_PIN, STEPPER_A_STEP_PIN, STEPPER_A_ENABLE_PIN);
    A4988 StepperMotorB = A4988(STEPPER_STEPS_PER_REVOLUTION, STEPPER_B_DIR_PIN, STEPPER_B_STEP_PIN, STEPPER_B_ENABLE_PIN);
    TimedSyncDriver Controller = TimedSyncDriver(StepperMotorA, StepperMotorB);
};

static CoreXYData s_Data;

void CoreXY::Initialize()
{
    s_Data.StepperMotorA.begin(CAP_RPM, STEPPER_MICROSTEPS);
    s_Data.StepperMotorA.setEnableActiveState(LOW);

    s_Data.StepperMotorB.begin(CAP_RPM, STEPPER_MICROSTEPS);
    s_Data.StepperMotorB.setEnableActiveState(LOW);
}

void CoreXY::OnUpdate()
{
    while (s_Data.Controller.isRunning())
    {
        long nextActionInterval = s_Data.Controller.nextAction();

        if (nextActionInterval >= CONSECUTIVE_NEXT_ACTION_INTERVALS)
            return;
    }
}

void CoreXY::Move(Vector2 position, long duration)
{
    Vector2Int coords = CalculateStepperCoordinate(position);
    Vector2Int delta = coords - m_StepperCoordinate;

    s_Data.Controller.startMoveTimed(delta.X, delta.Y, duration);
    m_StepperCoordinate = coords;
}

Vector2Int CoreXY::CalculateStepperCoordinate(Vector2 position)
{
    float a = position.X + position.Y;
    float b = position.X - position.Y;

    return { a / DISPLACEMENT_PER_STEP, b / DISPLACEMENT_PER_STEP };
}

void CoreXY::SetOrigin()
{
    m_StepperCoordinate = { 0, 0 };
}

bool CoreXY::IsMoving()
{
    return s_Data.Controller.isRunning();
}