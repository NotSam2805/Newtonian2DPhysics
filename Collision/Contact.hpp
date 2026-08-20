#pragma once
#include "../Maths/Maths.hpp"
#include "../Physics/Rigidbody.hpp"
#include <vector>

namespace n2p{
    struct CollisionPair{
        Rigidbody* bodyA;
        Rigidbody* bodyB;
    };

    struct Contact {
        Vector2 point;
        float penetration;
    };

    struct Manifold {
        CollisionPair pair;

        Vector2 normal;

        float penetration;

        std::vector<Vector2> contacts;

        bool colliding;
    };
}