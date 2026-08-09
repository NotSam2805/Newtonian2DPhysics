#pragma once
#include "Circle.h"

namespace n2p{
    explicit Circle::Circle(float radius) : radius(radius) {}

    float Circle::GetRadius() const { return radius; }

    ShapeType Circle::GetType() const { return ShapeType::Circle; }
}