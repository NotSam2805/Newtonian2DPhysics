#include "Maths/Maths.h"
#include "Physics/Rigidbody.h"

struct Contact {
    Rigidbody* a;
    Rigidbody* b;

    Vector2 normal;
    float penetration;
    Vector2 contactPoint;
};