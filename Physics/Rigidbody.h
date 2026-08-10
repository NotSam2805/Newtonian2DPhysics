#pragma once
#include "../Maths/Maths.h"
#include "../Shapes/Shape.h"
#include <memory>

namespace n2p{
    class Rigidbody{
    private:
        float mass;
        float inverseMass;// To avoid expensive division

        Transform transform;
        Vector2 velocity;

        Vector2 accumulatedForces;

        std::unique_ptr<Shape> shape;
    public:
        // Constructor
        explicit Rigidbody (float mass = 1.0f, const Transform& transform = Transform(), std::unique_ptr<Shape> shape);

        void AddForce(const Vector2& force);

        void Integrate(float dt);

        void ClearForces();

        // Getters
        const Vector2& GetPosition() const;

        const Vector2& GetVelocity() const;

        float GetMass() const;

        Shape* GetShape();
        const Shape* GetShape() const;

        // Setters
        void SetPosition(const Vector2& position);
    };
}
