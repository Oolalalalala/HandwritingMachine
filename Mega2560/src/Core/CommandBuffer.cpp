#include "CommandBuffer.h"


void CommandBuffer::SetConfig(float strokeSegmentLength, float strokeSpeed, float hoverSpeed)
{
    MachineCommand command;
    command.SetConfig.Type = MachineCommandType::SetConfig;
    command.SetConfig.StrokeSegmentLength = strokeSegmentLength;
    command.SetConfig.StrokeSpeed = strokeSpeed;
    command.SetConfig.HoverSpeed = hoverSpeed;
    m_Buffer.Push(command);
}

void CommandBuffer::Move(Vector2 start, Vector2 end)
{
    MachineCommand command;
    command.Move.Type = MachineCommandType::Move;
    command.Move.Start = start;
    command.Move.End = end;
    m_Buffer.Push(command);
}

void CommandBuffer::DrawDot(Vector2 point)
{
    MachineCommand command;
    command.DrawDot.Type = MachineCommandType::DrawDot;
    command.DrawDot.Point = point;
    m_Buffer.Push(command);
}

void CommandBuffer::DrawLine(Vector2 start, Vector2 end)
{
    MachineCommand command;
    command.DrawLine.Type = MachineCommandType::DrawLine;
    command.DrawLine.Start = start;
    command.DrawLine.End = end;
    m_Buffer.Push(command);
}

// [Parametrized Quadratic Function]: 0 <= t <= 1
void CommandBuffer::DrawQuadraticCurve(const QuadraticFunction &x, const QuadraticFunction &y)
{
    MachineCommand command;
    command.DrawQuadraticCurve.Type = MachineCommandType::DrawQuadraticCurve;
    command.DrawQuadraticCurve.X = x;
    command.DrawQuadraticCurve.Y = y;
    m_Buffer.Push(command);
}

// [Quadratic bezier curve]: p = p0 * (1 - t)^2 + 2 * p1 * (1 - t) * t + p2 * t^2, 0 <= t <= 1
void CommandBuffer::DrawQuadraticBezier(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2)
{
    QuadraticFunction x, y;
    x.A = p0.X - 2.0f * p1.X + p2.X;
    x.B = 2.0f * (p1.X - p0.X);
    x.C = p0.X;

    y.A = p0.Y - 2.0f * p1.Y + p2.Y;
    y.B = 2.0f * (p1.Y - p0.Y);
    y.C = p0.Y;

    DrawQuadraticCurve(x, y);
}

// [Parametrized Arc Function]: p = center + radius * <cos(t), sin(t)>, startAngle <= t <= endAngle
void CommandBuffer::DrawArc(Vector2 center, float radius, float startAngle, float endAngle)
{
    MachineCommand command;
    command.DrawArc.Type = MachineCommandType::DrawArc;
    command.DrawArc.Center = center;
    command.DrawArc.Radius = radius;
    command.DrawArc.StartAngle = startAngle;
    command.DrawArc.EndAngle = endAngle;
    m_Buffer.Push(command);
}
