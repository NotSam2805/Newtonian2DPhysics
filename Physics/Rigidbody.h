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
        explicit Rigidbody (float mass = 1.0f, const Transform& transform = Transform()) : 
        mass(mass), transform(transform), velocity(Vector2()), accumulatedForces(Vector2()), inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f) {}

        void AddForce(const Vector2& force){
            accumulatedForces += force;
        }

        void Integrate(float dt){
            // Semi-implicit Euler
            if (inverseMass == 0.0f){ return; }

            Vector2 acceleration = accumulatedForces * inverseMass;
            velocity += acceleration * dt;
            transform.position += velocity * dt;
        }

        void ClearForces(){
            accumulatedForces = Vector2::Zero();
        }

        // Getters
        const Vector2& GetPosition() const { return transform.position; }
        const Vector2& GetVelocity() const { return velocity; }
        float GetMass() const { return mass; }

        // Setters
        void SetPosition(const Vector2& position ){ transform.position = position; }
    };
}
