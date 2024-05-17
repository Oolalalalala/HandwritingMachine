#include "CommandBuffer.h"

void CommandBuffer::SetLiftPenThresholdDistance(float distance)
{
    m_Commands[m_Size].SetLiftPenThresholdDistance.Type = CommandType::SetLiftPenThresholdDistance;
    m_Commands[m_Size].SetLiftPenThresholdDistance.Distance = distance;
    m_Size++;
}

void CommandBuffer::MoveTo(Vector2 point)
{
    m_Commands[m_Size].MoveTo.Type = CommandType::MoveTo;
    m_Commands[m_Size].MoveTo.Point = point;
    m_Size++;
}

void CommandBuffer::DrawLine(Vector2 start, Vector2 end)
{
    m_Commands[m_Size].DrawLine.Type = CommandType::DrawLine;
    m_Commands[m_Size].DrawLine.Start = start;
    m_Commands[m_Size].DrawLine.End = end;
    m_Size++;
}

// [Parametrized Quadratic Function]: 0 <= t <= 1
void CommandBuffer::DrawQuadraticCurve(const QuadraticFunction &x, const QuadraticFunction &y)
{
    m_Commands[m_Size].DrawQuadraticCurve.Type = CommandType::DrawQuadraticCurve;
    m_Commands[m_Size].DrawQuadraticCurve.X = x;
    m_Commands[m_Size].DrawQuadraticCurve.Y = y;
    m_Size++;
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
    m_Commands[m_Size].DrawArc.Type = CommandType::DrawArc;
    m_Commands[m_Size].DrawArc.Center = center;
    m_Commands[m_Size].DrawArc.Radius = radius;
    m_Commands[m_Size].DrawArc.StartAngle = startAngle;
    m_Commands[m_Size].DrawArc.EndAngle = endAngle;
    m_Commands[m_Size].DrawArc.Clockwise = clockwise;
    m_Size++;
}

void CommandBuffer::Clear()
{
    m_Size = 0;
}