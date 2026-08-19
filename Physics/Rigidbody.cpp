#include "Rigidbody.hpp"
#include <memory>
#include <cmath>

namespace n2p{
    Rigidbody::Rigidbody (float mass, const Transform& transform, std::unique_ptr<Shape> shape, float inertia) :
    mass(mass), inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f), transform(transform), shape(std::move(shape)),
    velocity(Vector2::Zero()), accumulatedForces(Vector2::Zero()), inertia(inertia), inverseInertia(inertia > 0.0f ? 1.0f / inertia : 0.0f),
    accumulatedTorque(0.0f), rotationalVelocity(0.0f){}

    void Rigidbody::Integrate (float dt) {
        // Semi-implicit Euler
        if (inverseMass != 0.0f){
            Vector2 acceleration = accumulatedForces * inverseMass;
            velocity += acceleration * dt;
            transform.position += velocity * dt;
        }

        if (inverseInertia != 0.0f){
            float rotationalAcceleration = accumulatedTorque * inverseInertia;
            rotationalVelocity += rotationalAcceleration * dt;
            transform.rotation += rotationalVelocity * dt;
            transform.rotation = std::fmodf(transform.rotation, 2.0f * PI);
        }
    }

    void Rigidbody::IntegratePosition (float dt) {
        // Semi-implicit Euler
        transform.position += velocity * dt;

        transform.rotation += rotationalVelocity * dt;
        transform.rotation = std::fmodf(transform.rotation, 2.0f * PI);
    }

    void Rigidbody::IntegrateVelocity (float dt) {
        // Semi-implicit Euler
        if (inverseMass != 0.0f){
            Vector2 acceleration = accumulatedForces * inverseMass;
            velocity += acceleration * dt;
        }

        if (inverseInertia != 0.0f){
            float rotationalAcceleration = accumulatedTorque * inverseInertia;
            rotationalVelocity += rotationalAcceleration * dt;
        }
    }

    void Rigidbody::ClearForces () {
        accumulatedForces = Vector2::Zero();
    }

    void Rigidbody::ClearTorques() {
        accumulatedTorque = 0.0f;
    }

    void Rigidbody::AddForce (const Vector2& force) {
        accumulatedForces += force;
    }

    void Rigidbody::AddTorque(const float torque){
        accumulatedTorque += torque;
    }

    // Getters
    const Vector2& Rigidbody::GetPosition() const { return transform.position; }

    const Vector2& Rigidbody::GetVelocity() const { return velocity; }

    float Rigidbody::GetMass() const { return mass; }

    const float Rigidbody::GetRotationalVelocity() const{ return rotationalVelocity; }

    const float Rigidbody::GetRotation() const{ return transform.rotation; }

    // Setters
    void Rigidbody::SetPosition(const Vector2& position) { transform.position = position; }

    const Shape* Rigidbody::GetShape() const { return shape.get(); }

    const Transform& Rigidbody::GetTransform() const{ return transform; }
}