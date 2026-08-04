#include "Maths/Maths.h"

namespace n2p{
    class Rigidbody{
    private:
        float mass;
        float inverseMass;// To avoid expensive division

        Transform transform;
        Vector2 velocity;

        Vector2 accumulatedForces;
    public:
        // Constructor
        Rigidbody(float mass = 1.0, Transform transform = {Vector2(), 0}, float inertia = 1.0) : 
        mass(mass), transform(transform), velocity(Vector2()), accumulatedForces(Vector2()), inverseMass(1/mass) {}

        void AddForce(Vector2 force){
            accumulatedForces += force;
        }

        void Integrate(float dt){
            // Semi-implicit Euler
            Vector2 acceleration = accumulatedForces * inverseMass;
            velocity += acceleration * dt;
            transform.position += velocity * dt;

            accumulatedForces = Vector2::Zero();
        }
    };
}
