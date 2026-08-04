#include "Maths/Maths.h"
#include "Physics/Rigidbody.h"

namespace n2p{
    struct Contact {
        Rigidbody* a;
        Rigidbody* b;

        Vector2 normal;
        float penetration;
        Vector2 contactPoint;
    };
}