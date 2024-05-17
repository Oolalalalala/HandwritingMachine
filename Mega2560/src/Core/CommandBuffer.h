#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include "../Utils/Math.h"
#include "../Utils/Buffer.h"
#include "Command.h"


class CommandBuffer
{
public:
    CommandBuffer() = default;
    ~CommandBuffer() = default;

    void SetLiftPenThresholdDistance(float distance);

    void MoveTo(Vector2 point);


    // [Line Function]: p = start + t * (end - start), 0 <= t <= 1
    void DrawLine(Vector2 start, Vector2 end);

    // [Parametrized Quadratic Function]: 0 <= t <= 1
    void DrawQuadraticCurve(const QuadraticFunction& x, const QuadraticFunction& y);

    // [Quadratic bezier curve]: p = p0 * (1 - t)^2 + 2 * p1 * (1 - t) * t + p2 * t^2, 0 <= t <= 1
    void DrawQuadraticBezier(const Vector2& p0, const Vector2& p1, const Vector2& p2);

    // [Parametrized Arc Function]: p = center + radius * <cos(t), sin(t)>, startAngle <= t <= endAngle
    void DrawArc(Vector2 center, float radius, float startAngle, float endAngle, bool clockwise);

    void Clear();

    size_t GetSize() const { return m_Size; }
    static size_t GetCapacity() { return s_Capacity; }
    bool Full() const { return m_Size == s_Capacity; }

private:
    static const size_t s_Capacity = 100;
    Command m_Commands[s_Capacity];
    size_t m_Size = 0;
};

#endif