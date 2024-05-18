#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include "../Utils/Math.h"

enum class CommandType : uint8_t
{
    None = 0, SetPenWriteSpeed, SetPenMoveSpeed, SetLiftPenThresholdDistance, MoveTo, DrawDot, DrawLine, DrawQuadraticCurve, DrawArc
};


union Command
{
    CommandType Type; // For command that does not require additional data

    struct
    {
        CommandType Type;
        float Speed;
    } SetPenWriteSpeed;

    struct
    {
        CommandType Type;
        float Speed;
    } SetPenMoveSpeed;

    struct
    {
        CommandType Type;
        float Distance;
    } SetLiftPenThresholdDistance;

    struct
    {
        CommandType Type;
        Vector2 Point;
    } MoveTo;

    struct
    {
        CommandType Type;
        Vector2 Point;
    } DrawDot;

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


    // Default constructor
    Command() 
        : Type(CommandType::None)
    {
    }
};

#endif