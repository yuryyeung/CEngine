#pragma once
#include <cmath>
#include <iostream>

struct Vector2
{
    float x, y;

    Vector2(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

    // Basic Addition
    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    
    // Magnitude (Length)
    float magnitude() const { return std::sqrt(x * x + y * y); }
};

struct Vector3
{
    float x, y, z;

    Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}

    // Basic Addition
    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }

    // Scalar Multiplication
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

    // Normalize
    Vector3 normalized() const {
        float mag = std::sqrt(x * x + y * y + z * z);
        if (mag > 0) return Vector3(x / mag, y / mag, z / mag);
        return Vector3(0, 0, 0);
    }
};

struct Vector2Int
{
    int x, y;

    Vector2Int() : x(0), y(0) {}
    Vector2Int(int x, int y) : x(x), y(y) {}

    // Basic Addition
    Vector2Int operator+(const Vector2Int &other) const { return Vector2Int(x + other.x, y + other.y); }

    bool operator==(const Vector2Int &other) const
    {
        return x == other.x && y == other.y;
    }
};