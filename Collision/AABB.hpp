#pragma once
#include "../Maths/Maths.hpp"

namespace n2p{
    struct AABB{
        Vector2 min;
        Vector2 max;

        AABB() : min(Vector2()), max(Vector2()) {}
        AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}

        bool Overlaps(const AABB& other) const
        {
            return
                min.x <= other.max.x &&
                max.x >= other.min.x &&
                min.y <= other.max.y &&
                max.y >= other.min.y;
        }
    };
}