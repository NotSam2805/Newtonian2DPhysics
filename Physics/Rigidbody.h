#include "Maths/Maths.h"

class Rigidbody{
    public:
        Transform transform;
        Vector2D velocity;

        Vector2D accumulatedForces;
        float accumulatedTorque;
};