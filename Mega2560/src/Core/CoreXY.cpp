#include "CoreXY.h"

#include <Arduino.h>
#include <A4988.h> // A4988 stepper motor driver
#include "../Vendor/TimedSyncDriver.h" // Sync control for two stepper motors

#define STEPPER_A_DIR_PIN 31
#define STEPPER_A_STEP_PIN 33
#define STEPPER_A_ENABLE_PIN 35
#define STEPPER_B_DIR_PIN 37
#define STEPPER_B_STEP_PIN 39
#define STEPPER_B_ENABLE_PIN 41

#define STEPPER_STEPS_PER_REVOLUTION 200
#define STEPPER_MICROSTEPS 16 // Set ms1, ms2, ms3 of A4988 to low
#define STEPPER_WHEEL_RADIUS 5.0f // (mm)
#define DISPLACEMENT_PER_STEP (STEPPER_WHEEL_RADIUS * 2 * PI / STEPPER_STEPS_PER_REVOLUTION / STEPPER_MICROSTEPS) // (mm)
#define CONSECUTIVE_NEXT_ACTION_INTERVALS 0 // (us), if next action interval is less than this, we keep updating the steppers

#define CAP_RPM 240.0f // The max speed of the stepper motor, when too low, may cause the motors to desync

struct CoreXYData
{
    A4988 StepperMotorA = A4988(STEPPER_STEPS_PER_REVOLUTION, STEPPER_A_DIR_PIN, STEPPER_A_STEP_PIN, STEPPER_A_ENABLE_PIN);
    A4988 StepperMotorB = A4988(STEPPER_STEPS_PER_REVOLUTION, STEPPER_B_DIR_PIN, STEPPER_B_STEP_PIN, STEPPER_B_ENABLE_PIN);
    TimedSyncDriver Controller = TimedSyncDriver(StepperMotorA, StepperMotorB);
};

static CoreXYData s_Data;

void CoreXY::Initialize()
{
    s_Data.StepperMotorA.setEnableActiveState(LOW);
    s_Data.StepperMotorA.begin(CAP_RPM, STEPPER_MICROSTEPS);

    s_Data.StepperMotorB.setEnableActiveState(LOW);
    s_Data.StepperMotorB.begin(CAP_RPM, STEPPER_MICROSTEPS);

    m_Origin = { 0.0f, 0.0f };
    m_Position = { 0.0f, 0.0f };
    m_StepperCoordinate = { 0, 0 };
}

void CoreXY::OnUpdate()
{
    while (s_Data.Controller.isRunning())
    {
        long nextActionInterval = s_Data.Controller.nextAction();

        if (nextActionInterval >= CONSECUTIVE_NEXT_ACTION_INTERVALS || nextActionInterval == 0)
            return;
    }
}

void CoreXY::Move(Vector2 delta, long duration)
{
    Vector2Int coords = CalculateStepperCoordinate(m_Position + delta);
    Vector2Int deltaSteps = coords - m_StepperCoordinate;

    //s_Data.StepperMotorA.setRPM((float)deltaSteps.X / STEPPER_STEPS_PER_REVOLUTION / duration * 1000000.0f * 60.0f);
    //s_Data.StepperMotorB.setRPM((float)deltaSteps.Y / STEPPER_STEPS_PER_REVOLUTION / duration * 1000000.0f * 60.0f);
    s_Data.Controller.startMoveTimed(deltaSteps.X, deltaSteps.Y, duration);
    
    m_Position = m_Position + delta;
    m_StepperCoordinate = coords;

    // Debug
    //Serial.print(deltaSteps.X);
    //Serial.print(" ");
    //Serial.print(deltaSteps.Y);
    //Serial.print(" ");
    //Serial.println(duration);
}

void CoreXY::MoveTo(Vector2 position, long duration)
{
    Vector2Int coords = CalculateStepperCoordinate(m_Origin + position);
    Vector2Int deltaSteps = coords - m_StepperCoordinate;

    // Debug
    // Serial.print(deltaSteps.X);
    // Serial.print(" ");
    // Serial.print(deltaSteps.Y);
    // Serial.print(" ");
    // Serial.println(duration);

    s_Data.Controller.startMoveTimed(deltaSteps.X, deltaSteps.Y, duration);

    m_Position = m_Origin + position;
    m_StepperCoordinate = coords;
}

Vector2Int CoreXY::CalculateStepperCoordinate(Vector2 position)
{
    float a = position.X - position.Y;
    float b = position.X + position.Y;

    return { roundf(a / DISPLACEMENT_PER_STEP), roundf(b / DISPLACEMENT_PER_STEP) };
}

void CoreXY::WaitFinish()
{
    while (s_Data.Controller.isRunning())
    {
        s_Data.Controller.nextAction();
    }
}

void CoreXY::Enable()
{
    s_Data.StepperMotorA.enable();
    s_Data.StepperMotorB.enable();
}

void CoreXY::Disable()
{
    s_Data.StepperMotorA.disable();
    s_Data.StepperMotorB.disable();
}

void CoreXY::SetOrigin()
{
    m_Origin = m_Position;
}

bool CoreXY::IsMoving()
{
    return s_Data.Controller.isRunning();
}