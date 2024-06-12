#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include "../Utils/Math.h"

enum class MachineCommandType : uint32_t
{
    None = 0, SetConfig, Move, DrawDot, DrawLine, DrawQuadraticCurve, DrawArc
};


union MachineCommand
{
    MachineCommandType Type; // For command that does not require additional data

    struct
    {
        MachineCommandType Type;
        float StrokeSegmentLength;
        float StrokeSpeed;
        float HoverSpeed;
    } SetConfig;

    struct
    {
        MachineCommandType Type;
        Vector2 Start;
        Vector2 End;
    } Move;

    struct
    {
        MachineCommandType Type;
        Vector2 Point;
    } DrawDot;

    struct
    {
        MachineCommandType Type;
        Vector2 Start;
        Vector2 End;
    } DrawLine;

    struct
    {
        MachineCommandType Type;
        QuadraticFunction X;
        QuadraticFunction Y;
    } DrawQuadraticCurve;

    struct
    {
        MachineCommandType Type;
        Vector2 Center;
        float Radius;
        float StartAngle;
        float EndAngle;
    } DrawArc;


    // Default constructor
    MachineCommand() 
        : Type(MachineCommandType::None)
    {
    }

    Vector2 GetStartPosition()
    {
        switch (Type)
        {
            case MachineCommandType::Move:
                return Move.Start;
            case MachineCommandType::DrawDot:
                return DrawDot.Point;
            case MachineCommandType::DrawLine:
                return DrawLine.Start;
            case MachineCommandType::DrawQuadraticCurve:
                return { DrawQuadraticCurve.X.Evaluate(0.0f), DrawQuadraticCurve.Y.Evaluate(0.0f) };
            case MachineCommandType::DrawArc:
                return DrawArc.Center + Vector2{ (float)cosf(DrawArc.StartAngle), (float)sinf(DrawArc.StartAngle) } * DrawArc.Radius;
        }

        return Vector2();
    }
};

#endif