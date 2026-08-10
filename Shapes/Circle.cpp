#include "Circle.h"

namespace n2p{
    // Constructor
    explicit Circle::Circle(float radius) : radius(radius) {}

    // Getters
    float Circle::GetRadius() const { return radius; }

    const ShapeType Circle::GetType() const { return ShapeType::Circle; }
}