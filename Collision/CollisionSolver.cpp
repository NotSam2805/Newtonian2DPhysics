#include "CollisionSolver.hpp"

namespace n2p{
    void CollisionSolver::ResolveCollision(const Manifold& manifold){
        if (!manifold.colliding){
            return;
        }

        Rigidbody& bodyA = *manifold.pair.bodyA;
        Rigidbody& bodyB = *manifold.pair.bodyB;


        float totalInverseMass = bodyA.GetInverseMass() + bodyB.GetInverseMass();

        // Positional correction
        float correctionAmount = std::max((manifold.penetration - slop), 0.0f) * correctionPercent;
        Vector2 correction = manifold.normal * (correctionAmount / totalInverseMass);

        bodyA.Move(correction * bodyA.GetInverseMass());
        bodyB.Move(-correction * bodyB.GetInverseMass());

        // Find restitution as max. Could also use average or multiplier.
        float restitution = std::max(bodyA.restitution, bodyB.restitution);

        // Resolve velocity
        Vector2 relativeVelocity = bodyB.GetVelocity() - bodyA.GetVelocity();

        float normalVelocity = relativeVelocity.Dot(manifold.normal);

        if (normalVelocity < 0.0f){
            return;
        }

        float impulseMagnitude = -(1.0f + restitution) * normalVelocity;

        impulseMagnitude /= totalInverseMass;

        Vector2 impulse = manifold.normal * impulseMagnitude;

        bodyA.ApplyImpulse(-impulse);
        bodyB.ApplyImpulse(impulse);
    }
}