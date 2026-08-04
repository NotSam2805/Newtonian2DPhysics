#include <cmath>
#include <iostream>

class Vector2D{
    public:
        float x;
        float y;

        // Constructors
        constexpr Vector2D() : x(0.0f), y(0.0f) {}
        constexpr Vector2D(float x, float y) : x(x), y(y) {}
        
        static constexpr Vector2D Zero(){
            return Vector2D(0.0f, 0.0f);
        }

        float Magnitude() const{
            return std::sqrt(x*x + y*y);
        }


        // Avoids expensive sqrt
        float MagnitudeSqrd() const{
            return (x*x + y*y);
        }

        Vector2D Normalised() const{
            float magnitude = Magnitude();

            if (magnitude == 0.0f){
                return Vector2D::Zero();
            }

            return Vector2D(x / magnitude, y / magnitude);
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
        Vector2D operator+(Vector2D& other) const{
            return Vector2D(x + other.x, y + other.y);
        }

        Vector2D operator-(Vector2D& other) const{
            return Vector2D(x - other.x, y - other.y);
        }

        Vector2D operator-() const{
            return Vector2D(-x, -y);
        }

        Vector2D operator*(float scalar) const{
            return Vector2D(x * scalar, y * scalar);
        }

        Vector2D operator/(float scalar) const{
            return Vector2D(x / scalar, y / scalar);
        }

        Vector2D& operator+=(const Vector2D& other)
        {
            x += other.x;
            y += other.y;

            return *this;
        }


        Vector2D& operator-=(const Vector2D& other)
        {
            x -= other.x;
            y -= other.y;

            return *this;
        }


        Vector2D& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;

            return *this;
        }


        bool operator==(const Vector2D& other) const
        {
            return x == other.x && y == other.y;
        }


        bool operator!=(const Vector2D& other) const
        {
            return !(*this == other);
        }

        // Debug printing
        friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vector)
        {
            stream << "(" << vector.x << ", " << vector.y << ")";
            return stream;
        }
};

// Allow scalar * vector
inline Vector2D operator*(float scalar, const Vector2D& vector)
{
    return vector * scalar;
}