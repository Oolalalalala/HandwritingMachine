#include "PenHolder.h"

#include <Arduino.h>

#define SERVO_PIN 23
#define PEN_LIFT_ANGLE 0
#define PEN_DROP_ANGLE 60
#define PEN_LIFT_TIME 100 // (ms)
#define PEN_DROP_TIME 300 // (ms)

void PenHolder::Initialize()
{
    m_Servo.attach(SERVO_PIN); 

    m_Servo.write(PEN_LIFT_ANGLE);

    //m_LiftTime = 500;
    //m_DropTime = 0;
    m_LastActionTime = millis();
    m_Lifting = true;
}

void PenHolder::Lift()
{
    if (m_Lifting)
        return;
    
    m_Servo.write(PEN_LIFT_ANGLE);
    m_LastActionTime = millis();
    m_Lifting = true;
    //m_LiftTime = millis();
}

void PenHolder::Drop()
{
    if (!m_Lifting)
        return;

    m_Servo.write(PEN_DROP_ANGLE);
    m_LastActionTime = millis();
    m_Lifting = false;
    //m_DropTime = millis();
}

bool PenHolder::Contacting()
{
    unsigned long currentTime = millis();

    if (!m_Lifting && currentTime - m_LastActionTime > PEN_DROP_TIME)
        return true;
    return false;
}

bool PenHolder::Hovering()
{
    unsigned long currentTime = millis();
    
    if (m_Lifting && currentTime - m_LastActionTime > PEN_LIFT_TIME)
        return true;
    return false;
}