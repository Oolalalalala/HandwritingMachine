#ifndef MATH_H
#define MATH_H

#include <math.h>
#include <stdint.h>

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// Vector2
struct Vector2
{
    float X = 0.0f;
    float Y = 0.0f;

    Vector2() = default;
    Vector2(float x, float y)
        : X(x), Y(y)
    {
    }
    Vector2(double x, double y)
        : X((float)x), Y((float)y)
    {
    }
};

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return { lhs.X + rhs.X, lhs.Y + rhs.Y };
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    return { lhs.X - rhs.X, lhs.Y - rhs.Y };
}

inline Vector2 operator*(const Vector2& lhs, float rhs)
{
    return { lhs.X * rhs, lhs.Y * rhs };
}

inline Vector2 operator/(const Vector2& lhs, float rhs)
{
    return { lhs.X / rhs, lhs.Y / rhs };
}

inline float Dot(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
}

inline float Length(const Vector2& v)
{
    return sqrtf(v.X * v.X + v.Y * v.Y);
}

inline float Distance(const Vector2& a, const Vector2& b)
{
    return Length(a - b);
}

inline Vector2 Normalize(const Vector2& v)
{
    float invLength = 1.0f / Length(v);
    return { v.X * invLength, v.Y * invLength };
}

inline Vector2 Lerp(const Vector2& a, const Vector2& b, float t)
{
    return a + (b - a) * t;
}

inline Vector2 QuadraticBezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t)
{
    return p0 * (1.0f - t) * (1.0f - t) + p1 * 2.0f * (1.0f - t) * t + p2 * t * t;
}

// Vector2Int
struct Vector2Int
{
    long X = 0;
    long Y = 0;

    Vector2Int() = default;
    Vector2Int(long x, long y)
        : X(x), Y(y)
    {
    }
};

inline Vector2Int operator+(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return { lhs.X + rhs.X, lhs.Y + rhs.Y };
}

inline Vector2Int operator-(const Vector2Int& lhs, const Vector2Int& rhs)
{
    return { lhs.X - rhs.X, lhs.Y - rhs.Y };
}

inline Vector2Int operator*(const Vector2Int& lhs, long rhs)
{
    return { lhs.X * rhs, lhs.Y * rhs };
}

inline Vector2Int operator/(const Vector2Int& lhs, long rhs)
{
    return { lhs.X / rhs, lhs.Y / rhs };
}


// Linear function
struct LinearFunction
{
    float A = 0.0f;
    float B = 0.0f;


    LinearFunction() = default;
    
    LinearFunction(float a, float b)
        : A(a), B(b)
    {
    }

    float Evaluate(float x)
    {
        return A * x + B;
    }

    float Solve(float rhs)
    {
        return (rhs - B) / A;
    }
};

// Quadratic function
struct QuadraticFunction
{
    float A = 0.0f;
    float B = 0.0f;
    float C = 0.0f;

    QuadraticFunction() = default;

    QuadraticFunction(float a, float b, float c)
        : A(a), B(b), C(c)
    {
    }

    float Evaluate(float x)
    {
        return A * x * x + B * x + C;
    }

    bool Solve(float rhs, float& x1, float& x2)
    {
        float delta = B * B - 4 * A * (C - rhs);

        if (delta < 0)
            return false;

        x1 = (-B + sqrt(delta)) / (2.0f * A);
        x2 = (-B - sqrt(delta)) / (2.0f * A);

        return true;
    }

    LinearFunction Derivative()
    {
        return { 2.0f * A, B };
    }
};

#endif