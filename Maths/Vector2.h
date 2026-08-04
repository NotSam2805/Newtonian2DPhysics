#include <cmath>
#include <iostream>

class Vector2{
    public:
        float x;
        float y;

        // Constructors
        constexpr Vector2() : x(0.0f), y(0.0f) {}
        constexpr Vector2(float x, float y) : x(x), y(y) {}
        
        static constexpr Vector2 Zero(){
            return Vector2(0.0f, 0.0f);
        }

        float Magnitude() const{
            return std::sqrt(x*x + y*y);
        }


        // Avoids expensive sqrt
        float MagnitudeSqrd() const{
            return (x*x + y*y);
        }

        Vector2 Normalised() const{
            float magnitude = Magnitude();

            if (magnitude == 0.0f){
                return Vector2::Zero();
            }

            return Vector2(x / magnitude, y / magnitude);
        }

        void Normalise(){
            float magnitude = Magnitude();

            if (magnitude == 0.0f){
                return;
            }

            x /= magnitude;
            y /= magnitude;
        }


        // Operators
        Vector2 operator+(Vector2& other) const{
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator-(Vector2& other) const{
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 operator-() const{
            return Vector2(-x, -y);
        }

        Vector2 operator*(float scalar) const{
            return Vector2(x * scalar, y * scalar);
        }

        Vector2 operator/(float scalar) const{
            return Vector2(x / scalar, y / scalar);
        }

        Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }


        Vector2& operator-=(const Vector2& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }


        Vector2& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }


        bool operator==(const Vector2& other) const
        {
            return x == other.x && y == other.y;
        }


        bool operator!=(const Vector2& other) const
        {
            return !(*this == other);
        }

        // Debug printing
        friend std::ostream& operator<<(std::ostream& stream, const Vector2& vector)
        {
            stream << "(" << vector.x << ", " << vector.y << ")";
            return stream;
        }
};

// Allow scalar * vector
inline Vector2 operator*(float scalar, const Vector2& vector)
{
    return vector * scalar;
}