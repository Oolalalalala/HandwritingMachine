#include "Timer.h"

#include "Arduino.h"

Timer::Timer()
{
    m_Time = micros();
}

float Timer::Tick()
{
    unsigned long currentTime = micros();
    unsigned long delta = currentTime - m_Time;
    m_Time = currentTime;
    return delta * 0.000001f;
}