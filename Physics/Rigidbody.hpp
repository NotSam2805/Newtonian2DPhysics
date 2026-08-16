#pragma once
#include "../Maths/Maths.hpp"
#include "../Shapes/Shapes.hpp"
#include <memory>

namespace n2p{
    class Rigidbody{
    private:
        float mass;
        float inverseMass;// To avoid expensive division

        Transform transform;

        Vector2 velocity;

        Vector2 accumulatedForces;

        // for rotational dynamics
        float rotationalVelocity;
        float accumulatedTorque;
        float inertia;
        float inverseInertia;

        std::unique_ptr<Shape> shape;
    public:
        // Constructor
        explicit Rigidbody (
            float mass = 1.0f,
            const Transform& transform = Transform(),
            std::unique_ptr<Shape> shape = std::make_unique<Circle>(1.0f, Colour{255,255,255,255}),
            float inertia = 1.0f
        );

        ~Rigidbody() = default;

        void AddForce(const Vector2& force);

        void AddTorque(const float torque);

        void Integrate(float dt);

        void ClearForces();

        void ClearTorques();

        // Getters
        const Vector2& GetPosition() const;

        const Vector2& GetVelocity() const;

        const float GetRotationalVelocity() const;

        const float GetRotation() const;

        float GetMass() const;

        Shape* GetShape();
        const Shape* GetShape() const;

        const Transform& GetTransform() const;

        // Setters
        void SetPosition(const Vector2& position);
    };
}
