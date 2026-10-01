#pragma once
#include <iostream>

namespace n2p{
    class Vector2{
    public:
        float x;
        float y;

        // Constructors
        constexpr Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
        
        static constexpr Vector2 Zero() { return Vector2(); }

        float Magnitude() const;

        // Avoids expensive sqrt
        float MagnitudeSqrd() const;

        Vector2 Normalised() const;

        void Normalise();

        // Give the vector rotated around an origin, rotation given in radians clockwise
        Vector2 Rotate(float rotation, const Vector2& origin = Vector2::Zero()) const;

        float Dot(const Vector2& other) const;

        static float Cross(const Vector2& a, const Vector2& b);

        static Vector2 Cross(float angularVelocity, const Vector2& r);
        
        
        // Operators

        Vector2 operator+(const Vector2& other) const;

        Vector2 operator-(const Vector2& other) const;

        // To give the negative
        Vector2 operator-() const;

        Vector2 operator*(float scalar) const;

        Vector2 operator/(float scalar) const;

        Vector2& operator+=(const Vector2& other);

        Vector2& operator-=(const Vector2& other);

        Vector2& operator*=(float scalar);

        bool operator==(const Vector2& other);

        bool operator!=(const Vector2& other);

        // Debug printing
        friend std::ostream& operator<<(std::ostream& stream, const Vector2& vector)
        {
            stream << "(" << vector.x << ", " << vector.y << ")";
            return stream;
        }
    };
}

