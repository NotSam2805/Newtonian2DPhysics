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
        float normalImpulse = 0.0f;
        float frictionImpulse = 0.0f;
        Vector2 tangent = Vector2::Zero();
    };

    struct Manifold {
        CollisionPair pair;

        Vector2 normal;

        float penetration;

        std::vector<Contact> contacts;

        bool colliding;
    };
}