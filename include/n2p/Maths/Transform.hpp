#pragma once
#include "Vector2.hpp"

namespace n2p{
    struct Transform {
        Vector2 position;
        float rotation;

        Vector2 transform(const Vector2& point) const {
            return (point.Rotate(rotation) + position);
        }
    };
}
