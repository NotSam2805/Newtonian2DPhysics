#include "Maths/Maths.h"

class Rigidbody{
    private:
        float mass;
        float inertia;

        Transform transform;
        Vector2 velocity;
        float angularVelocity;

        Vector2 accumulatedForces;
        float accumulatedTorque;
    public:
        // Constructor
        Rigidbody(float mass = 1.0, Transform transform = {Vector2(), 0}, float inertia = 1.0) : 
        mass(mass), inertia(inertia), transform(transform), velocity(Vector2()), angularVelocity(0), accumulatedForces(Vector2()), accumulatedTorque(0) {}

        void AddForce(Vector2 force){
            accumulatedForces += force;
        }

        void AddTorque(float torque){
            accumulatedTorque += torque;
        }

};