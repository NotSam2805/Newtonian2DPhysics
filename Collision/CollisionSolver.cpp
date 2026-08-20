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

        // Resolve velocity for linear impulse
        /*
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
        */

        // Resolve velocity for impulse at contact point, apply rotational forces
        for (const Vector2& contact : manifold.contacts){
            Vector2 rA = contact - bodyA.GetPosition();
            Vector2 rB = contact - bodyB.GetPosition();

            // Velocity at contact
            Vector2 velocityA = bodyA.GetVelocity() + Vector2::Cross(bodyA.GetRotationalVelocity(), rA);
            Vector2 velocityB = bodyB.GetVelocity() + Vector2::Cross(bodyB.GetRotationalVelocity(), rB);

            Vector2 relativeVelocity = velocityB - velocityA;

            float normalVelocity = relativeVelocity.Dot(manifold.normal);

            // Already seperating
            if (normalVelocity < 0.0f){
                continue;
            }

            float raCrossN = Vector2::Cross(rA, manifold.normal);
            float rbCrossN = Vector2::Cross(rB, manifold.normal);

            float denominator = bodyA.GetInverseMass() + bodyB.GetInverseMass() +
                (raCrossN * raCrossN * bodyA.GetInverseInertia()) +
                (rbCrossN * rbCrossN * bodyB.GetInverseInertia());

            if (denominator <= 0.0f){
                continue;
            }

            float impulseMagnitude = -(1.0f + restitution) * normalVelocity;
            impulseMagnitude /= denominator;

            Vector2 impulse = manifold.normal * impulseMagnitude;
            bodyA.ApplyImpulse(-impulse, contact);
            bodyB.ApplyImpulse(impulse, contact);
        }
    }
}