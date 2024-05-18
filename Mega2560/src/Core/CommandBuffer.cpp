#include "CommandBuffer.h"


void CommandBuffer::SetPenWriteSpeed(float speed)
{
    Command command;
    command.SetPenWriteSpeed.Type = CommandType::SetPenWriteSpeed;
    command.SetPenWriteSpeed.Speed = speed;
    m_Buffer.Push(command);
}

void CommandBuffer::SetPenMoveSpeed(float speed)
{
    Command command;
    command.SetPenMoveSpeed.Type = CommandType::SetPenMoveSpeed;
    command.SetPenMoveSpeed.Speed = speed;
    m_Buffer.Push(command);

}

void CommandBuffer::SetLiftPenThresholdDistance(float distance)
{
    Command command;
    command.SetLiftPenThresholdDistance.Type = CommandType::SetLiftPenThresholdDistance;
    command.SetLiftPenThresholdDistance.Distance = distance;
    m_Buffer.Push(command);
}

void CommandBuffer::MoveTo(Vector2 point)
{
    Command command;
    command.MoveTo.Type = CommandType::MoveTo;
    command.MoveTo.Point = point;
    m_Buffer.Push(command);
}

void CommandBuffer::DrawDot(Vector2 point)
{
    Command command;
    command.DrawDot.Type = CommandType::DrawDot;
    command.DrawDot.Point = point;
    m_Buffer.Push(command);
}

void CommandBuffer::DrawLine(Vector2 start, Vector2 end)
{
    Command command;
    command.DrawLine.Type = CommandType::DrawLine;
    command.DrawLine.Start = start;
    command.DrawLine.End = end;
    m_Buffer.Push(command);
}

// [Parametrized Quadratic Function]: 0 <= t <= 1
void CommandBuffer::DrawQuadraticCurve(const QuadraticFunction &x, const QuadraticFunction &y)
{
    Command command;
    command.DrawQuadraticCurve.Type = CommandType::DrawQuadraticCurve;
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
void CommandBuffer::DrawArc(Vector2 center, float radius, float startAngle, float endAngle, bool clockwise)
{
    Command command;
    command.DrawArc.Type = CommandType::DrawArc;
    command.DrawArc.Center = center;
    command.DrawArc.Radius = radius;
    command.DrawArc.StartAngle = startAngle;
    command.DrawArc.EndAngle = endAngle;
    command.DrawArc.Clockwise = clockwise;
    m_Buffer.Push(command);
}
