#include "Rigidbody.hpp"
#include <memory>

namespace n2p{
    Rigidbody::Rigidbody (float mass, const Transform& transform, std::unique_ptr<Shape> shape) :
    mass(mass), inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f), transform(transform), shape(std::move(shape)), velocity(Vector2::Zero()), accumulatedForces(Vector2::Zero()) {}

    void Rigidbody::Integrate (float dt) {
        // Semi-implicit Euler
        if (inverseMass == 0.0f){ return; }

        Vector2 acceleration = accumulatedForces * inverseMass;
        velocity += acceleration * dt;
        transform.position += velocity * dt;
    }

    void Rigidbody::ClearForces () {
        accumulatedForces = Vector2::Zero();
    }

    void Rigidbody::AddForce (const Vector2& force) {
        accumulatedForces += force;
    }

    // Getters
    const Vector2& Rigidbody::GetPosition() const { return transform.position; }

    const Vector2& Rigidbody::GetVelocity() const { return velocity; }

    float Rigidbody::GetMass() const { return mass; }

    // Setters
    void Rigidbody::SetPosition(const Vector2& position) { transform.position = position; }

    const Shape* Rigidbody::GetShape() const { return shape.get(); }

    const Transform& Rigidbody::GetTransform() const{ return transform; }
}