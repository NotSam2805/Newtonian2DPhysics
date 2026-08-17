#include "Circle.hpp"

namespace n2p{
    // Constructor
    Circle::Circle(float radius, Colour colour) : radius(radius), colour(colour) {}

    // Getters
    float Circle::GetRadius() const { return radius; }

    const ShapeType Circle::GetType() const { return ShapeType::circle; }
}