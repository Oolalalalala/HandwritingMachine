#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include "../Utils/Math.h"

enum class CommandType : uint8_t
{
    None = 0, SetConfig, Move, DrawDot, DrawLine, DrawQuadraticCurve, DrawArc
};


union Command
{
    CommandType Type; // For command that does not require additional data

    struct
    {
        CommandType Type;
        float StrokeSegmentLength;
        float StrokeSpeed;
        float HoverSpeed;
    } SetConfig;

    struct
    {
        CommandType Type;
        Vector2 Start;
        Vector2 End;
    } Move;

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
    } DrawArc;


    // Default constructor
    Command() 
        : Type(CommandType::None)
    {
    }

    Vector2 GetStartPosition()
    {
        switch (Type)
        {
            case CommandType::Move:
                return Move.Start;
            case CommandType::DrawDot:
                return DrawDot.Point;
            case CommandType::DrawLine:
                return DrawLine.Start;
            case CommandType::DrawQuadraticCurve:
                return { DrawQuadraticCurve.X.Evaluate(0.0f), DrawQuadraticCurve.Y.Evaluate(0.0f) };
            case CommandType::DrawArc:
                return DrawArc.Center + Vector2{ (float)cosf(DrawArc.StartAngle), (float)sinf(DrawArc.StartAngle) } * DrawArc.Radius;
        }

        return Vector2();
    }
};

#endif