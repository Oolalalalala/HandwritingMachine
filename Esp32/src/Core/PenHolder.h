#ifndef PEN_HOLDER_H
#define PEN_HOLDER_H

#include <ESP32Servo.h>

class PenHolder
{
public:
    PenHolder() = default;
    ~PenHolder() = default;

    void Initialize();

    void Lift();
    void Drop();

    bool Contacting(); // Contacting the paper
    bool Hovering();

private:
    Servo m_Servo;

    unsigned long m_LastActionTime;
    bool m_Lifting;
};

#endif 