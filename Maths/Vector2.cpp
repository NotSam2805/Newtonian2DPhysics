#include "Vector2.hpp"
#include <cmath>

namespace n2p{
    float Vector2::Magnitude() const {
        return std::sqrt(x*x + y*y);
    }

    // Avoids expensive sqrt
    float Vector2::MagnitudeSqrd() const {
        return (x*x + y*y);
    }

    Vector2 Vector2::Normalised() const {
        float magnitude = Magnitude();

        if (magnitude == 0.0f){
            return Vector2::Zero();
        }

        return Vector2(x / magnitude, y / magnitude);
    }

    void Vector2::Normalise(){
        float magnitude = Magnitude();

        if (magnitude == 0.0f){
            return;
        }

        x /= magnitude;
        y /= magnitude;
    }

    float Vector2::Dot(const Vector2& other) const {
        return (x * other.x) + (y * other.y);
    }
    // Give the vector rotated around an origin, rotation given in radians clockwise
    Vector2 Vector2::Rotate(float rotation, const Vector2& origin) const{
        float c = std::cos(rotation);
        float s = std::sin(rotation);

        float rot_x = ((x - origin.x) * c) - ((y - origin.y) * s) + origin.x;
        float rot_y = ((x - origin.x) * s) + ((y - origin.y) * c) + origin.y;

        return Vector2(rot_x, rot_y);
    }

    // Operators
    Vector2 Vector2::operator+ (const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 Vector2::operator- (const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 Vector2::operator- () const {
        return Vector2(-x, -y);
    }

    Vector2 Vector2::operator* (float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 Vector2::operator/ (float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& Vector2::operator+= (const Vector2& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    Vector2& Vector2::operator-= (const Vector2& other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Vector2& Vector2::operator*= (float scalar) {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    bool operator== (const Vector2& left, const Vector2& right){
        return (left.x == right.x) && (right.y == left.y);
    }

    bool operator!= (const Vector2& left, const Vector2& right){
        return !(left==right);
    }

    // Allow scalar * vector
    inline Vector2 operator*(float scalar, const Vector2& vector)
    {
        return vector * scalar;
    }
}