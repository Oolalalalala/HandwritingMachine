#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include "../Utils/Math.h"
#include "../Utils/RingBuffer.h"
#include "Command.h"


class CommandBuffer
{
public:
    CommandBuffer() = default;
    ~CommandBuffer() = default;

    // Configure parameters
    void SetPenWriteSpeed(float speed);
    void SetPenMoveSpeed(float speed);
    void SetLiftPenThresholdDistance(float distance);

    // Lifts the pen and moves to the specified point
    void MoveTo(Vector2 point);

    // [Point]: p = point
    void DrawDot(Vector2 point);

    // [Line Function]: p = start + t * (end - start), 0 <= t <= 1
    void DrawLine(Vector2 start, Vector2 end);

    // [Parametrized Quadratic Function]: 0 <= t <= 1
    void DrawQuadraticCurve(const QuadraticFunction& x, const QuadraticFunction& y);

    // [Quadratic bezier curve]: p = p0 * (1 - t)^2 + 2 * p1 * (1 - t) * t + p2 * t^2, 0 <= t <= 1
    void DrawQuadraticBezier(const Vector2& p0, const Vector2& p1, const Vector2& p2);

    // [Parametrized Arc Function]: p = center + radius * <cos(t), sin(t)>, startAngle <= t <= endAngle
    void DrawArc(Vector2 center, float radius, float startAngle, float endAngle, bool clockwise);

    // Accessors
    static size_t Capacity() { return s_Capacity; }
    size_t Size() const { return m_Buffer.Size(); }
    bool Full() const { return m_Buffer.Full(); }

    // Modifiers
    void Clear() { m_Buffer.Clear(); }

private:
    static const int s_Capacity = 100;
    RingBuffer<Command, s_Capacity> m_Buffer;
};

#endif