#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include "../Utils/Math.h"

enum class CommandType : uint8_t
{
    None = 0, SetLiftPenThresholdDistance, MoveTo, DrawLine, DrawQuadraticCurve, DrawArc
};


union Command
{
    CommandType Type;
    struct
    {
        CommandType Type;
        Vector2 Point;
    } MoveTo;

    struct
    {
        CommandType Type;
        float Distance;
    } SetLiftPenThresholdDistance;

    struct
    {
        CommandType Type;
        Vector2 Start;
        Vector2 End;
    } DrawLine;

    struct
    {
        CommandType Type;
        QuadraticFunction X;
        QuadraticFunction Y;
    } DrawQuadraticCurve;

    struct
    {
        CommandType Type;
        Vector2 Center;
        float Radius;
        float StartAngle;
        float EndAngle;
        bool Clockwise;
    } DrawArc;
};

#endif