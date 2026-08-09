#pragma once
#include "../Maths/Maths.h"

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
        explicit Rigidbody (float mass = 1.0f, const Transform& transform = Transform());

        void AddForce(const Vector2& force);

        void Integrate(float dt);

        void ClearForces();

        // Getters
        const Vector2& GetPosition() const;

        const Vector2& GetVelocity() const;

        float GetMass() const;

        // Setters
        void SetPosition(const Vector2& position);
    };
}
