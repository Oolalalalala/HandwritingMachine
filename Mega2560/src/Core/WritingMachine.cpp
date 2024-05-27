#include "WritingMachine.h"


#define STROKE_SEGMENT_TIME ((long)((STROKE_SEGMENT_LENGTH / m_Config.StrokeSpeed) * 1000000.0f)) // (us)


void WritingMachine::Initialize()
{
    m_PenHolder.Initialize();

    m_StrokeSegmentTime = (m_Config.StrokeSegmentLength / m_Config.StrokeSpeed) * 1000000;
}

void WritingMachine::OnUpdate(float dt)
{
    // Wait for last stroke to finish
    if (m_CoreXY.IsMoving())
    {
        m_CoreXY.OnUpdate();
        m_Writing = true;
        return;
    }

    // Previous command finished
    if (m_StrokeProgress == 1.0f)
    {
        if (m_CommandBuffer.Empty() || !m_Enabled)
        {
            m_Writing = false;
            return;
        }

        m_ExecutingCommand = m_CommandBuffer.NextCommand();
        m_StrokeProgress = 0.0f;
    }


    // New command begins
    if (m_StrokeProgress == 0.0f)
    {   
        if (m_ExecutingCommand.Type == CommandType::SetConfig)
        {
            m_Config.StrokeSegmentLength = m_ExecutingCommand.SetConfig.StrokeSegmentLength;
            m_Config.StrokeSpeed = m_ExecutingCommand.SetConfig.StrokeSpeed;
            m_Config.HoverSpeed = m_ExecutingCommand.SetConfig.HoverSpeed;
            m_StrokeSegmentTime = (m_Config.StrokeSegmentLength / m_Config.StrokeSpeed) * 1000000;
            
            m_StrokeProgress = 1.0f;
            return;
        }
        if (m_ExecutingCommand.Type == CommandType::Move && m_PenHolder.Contacting())
        {
            m_PenHolder.Lift();
            return;
        }
        if (m_ExecutingCommand.Type != CommandType::Move && m_PenHolder.Hovering())
        {
            m_PenHolder.Drop();
            return;
        }
    }

    NextStroke();
}

void WritingMachine::NextStroke()
{
    Vector2 targetPosition;
    float duration = m_StrokeSegmentTime;

    switch (m_ExecutingCommand.Type)
    {
        case CommandType::DrawDot: // No need to move
        {
            m_StrokeProgress = 1.0f;
            return;
        }
        case CommandType::Move:
        {
            Vector2& start = m_ExecutingCommand.Move.Start;
            Vector2& end = m_ExecutingCommand.Move.End;

            float length = Distance(start, end);
            float time = length / m_Config.HoverSpeed;

            // Not devided into segments
            targetPosition = end;
            m_StrokeProgress = 1.0f;

            break;
        }
        case CommandType::DrawLine:
        {
            Vector2& start = m_ExecutingCommand.DrawLine.Start;
            Vector2& end = m_ExecutingCommand.DrawLine.End;

            float length = Distance(start, end);
            float time = length / m_Config.StrokeSpeed;

            // Not devided into segments
            targetPosition = end;
            m_StrokeProgress = 1.0f;

            break;
        }
        case CommandType::DrawQuadraticCurve:
        {
            QuadraticFunction& x = m_ExecutingCommand.DrawQuadraticCurve.X;
            QuadraticFunction& y = m_ExecutingCommand.DrawQuadraticCurve.Y;

            float dx = x.Derivative().Evaluate(m_StrokeProgress);
            float dy = y.Derivative().Evaluate(m_StrokeProgress);
            float speed = sqrtf(dx * dx + dy * dy);

            float dt = m_Config.StrokeSegmentLength / speed;
            m_StrokeProgress += dt;
            if (m_StrokeProgress > 1.0f)
            {
                duration *= (1.0f - (m_StrokeProgress - dt)) / dt;
                m_StrokeProgress = 1.0f;
            }

            targetPosition = { x.Evaluate(m_StrokeProgress), y.Evaluate(m_StrokeProgress) };
            break;
        }
        case CommandType::DrawArc:
        {
            Vector2& center = m_ExecutingCommand.DrawArc.Center;
            float& radius = m_ExecutingCommand.DrawArc.Radius;
            float& startAngle = m_ExecutingCommand.DrawArc.StartAngle;
            float& endAngle = m_ExecutingCommand.DrawArc.EndAngle;

            float arcLength = fabs(endAngle - startAngle) * radius;
            float dt = m_Config.StrokeSegmentLength / arcLength;
            m_StrokeProgress += dt;
            if (m_StrokeProgress > 1.0f)
            {
                duration *= (1.0f - (m_StrokeProgress - dt)) / dt;
                m_StrokeProgress = 1.0f;
            }

            float angle = startAngle + (endAngle - startAngle) * m_StrokeProgress;
            targetPosition = center + Vector2(radius * cos(angle), radius * sin(angle));

            break;
        }
    }

    m_CoreXY.Move(targetPosition, duration);
}

void WritingMachine::Enable(bool enabled)
{
    m_Enabled = enabled;
}

bool WritingMachine::IsWriting()
{
    return m_Writing;
}