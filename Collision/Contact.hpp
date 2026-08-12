#include "Maths/Maths.hpp"
#include "Physics/Rigidbody.hpp"

namespace n2p{
    struct Contact {
        Rigidbody* a;
        Rigidbody* b;

        Vector2 normal;
        float penetration;
        Vector2 contactPoint;
    };
}