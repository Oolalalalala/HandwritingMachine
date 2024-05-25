#include "WritingMachine.h"

#include <Arduino.h>
#include <A4988.h> // A4988 stepper motor driver
#include <Servo.h> // Servo motor driver

#define STEPPER_STEPS_PER_REVOLUTION 200
#define STEPPER_MICROSTEPS 16 // Set ms1, ms2, ms3 of A4988 to high
#define STEPPER_A_DIR_PIN 31
#define STEPPER_A_STEP_PIN 33
#define STEPPER_A_ENABLE_PIN 35
#define STEPPER_B_DIR_PIN 37
#define STEPPER_B_STEP_PIN 39
#define STEPPER_B_ENABLE_PIN 41
#define PEN_SERVO_PIN 3

struct WritingMachineData
{
    A4988 StepperMotorA = A4988(STEPPER_STEPS_PER_REVOLUTION, STEPPER_A_DIR_PIN, STEPPER_A_STEP_PIN, STEPPER_A_ENABLE_PIN);
    A4988 StepperMotorB = A4988(STEPPER_STEPS_PER_REVOLUTION, STEPPER_B_DIR_PIN, STEPPER_B_STEP_PIN, STEPPER_B_ENABLE_PIN);
    Servo PenServo;
};

static WritingMachineData s_Data;

void WritingMachine::Initialize()
{
    s_Data.StepperMotorA.begin(60.0f/*RPM*/, STEPPER_MICROSTEPS);
    s_Data.StepperMotorA.setEnableActiveState(LOW);

    s_Data.StepperMotorB.begin(60.0f/*RPM*/, STEPPER_MICROSTEPS);
    s_Data.StepperMotorB.setEnableActiveState(LOW);

    s_Data.PenServo.attach(PEN_SERVO_PIN);
}

void WritingMachine::OnUpdate(float dt)
{
}

void WritingMachine::Execute()
{

}

void WritingMachine::Pause()
{

}

bool WritingMachine::IsIdle()
{

}