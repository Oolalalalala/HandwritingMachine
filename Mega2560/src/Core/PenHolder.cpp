#include "PenHolder.h"

#include <Arduino.h>

#define SERVO_PIN 3
#define PEN_LIFT_TIME 100 // (ms)
#define PEN_DROP_TIME 300 // (ms)

void PenHolder::Initialize()
{
    m_Servo.attach(SERVO_PIN);
}

void PenHolder::Lift()
{
    if (m_LiftTime > m_DropTime)
        return;
    
    m_Servo.write(0);
    m_LiftTime = millis();
}

void PenHolder::Drop()
{
    if (m_DropTime > m_LiftTime)
        return;

    m_Servo.write(90);
    m_DropTime = millis();
}

bool PenHolder::Contacting()
{
    unsigned long currentTime = millis();

    if (m_LiftTime > m_DropTime)
        return false;

    if (currentTime - m_DropTime >= PEN_DROP_TIME)
        return true;

    return false;
}

bool PenHolder::Hovering()
{
    unsigned long currentTime = millis();

    if (m_DropTime > m_LiftTime)
        return false;

    if (currentTime - m_LiftTime >= PEN_LIFT_TIME)
        return true;

    return false;
}