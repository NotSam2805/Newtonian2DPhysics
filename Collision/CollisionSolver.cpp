#include "CollisionSolver.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace n2p{
    void CollisionSolver::ResolveCollision(const Manifold& manifold){
        if (!manifold.colliding){
            return;
        }

        Rigidbody& bodyA = *manifold.pair.bodyA;
        Rigidbody& bodyB = *manifold.pair.bodyB;


        float totalInverseMass = bodyA.GetInverseMass() + bodyB.GetInverseMass();

        // Positional correction
        float correctionAmount = (manifold.penetration / totalInverseMass) * correctionPercent;
        correctionAmount = correctionAmount < slop ? 0.0f : correctionAmount;
        Vector2 correction = manifold.normal * correctionAmount;

        bodyA.Move(correction * bodyA.GetInverseMass());
        bodyB.Move(-correction * bodyB.GetInverseMass());

        // Find restitution as multiplier. Could also use average or max.
        float restitution = bodyA.restitution * bodyB.restitution;

        // Resolve velocity for impulse at contact point
        std::cout << "Collision" << "\n";
        for (const Vector2& contact : manifold.contacts){
            Vector2 rA = contact - bodyA.GetPosition();
            Vector2 rB = contact - bodyB.GetPosition();

            // Velocity at contact
            Vector2 velocityA = bodyA.GetVelocityAtPoint(contact);
            Vector2 velocityB = bodyB.GetVelocityAtPoint(contact);

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

            if (impulseMagnitude < impulseSlop && -impulseSlop < impulseMagnitude){
                // Avoid jitter
                impulseMagnitude = 0.0f;
            }

            Vector2 normalImpulse = manifold.normal * impulseMagnitude;

            std::cout << "Normal: " << normalImpulse << "\n";

            bodyA.ApplyImpulse(-normalImpulse, contact);
            bodyB.ApplyImpulse(normalImpulse, contact);


            // Velocity at contact needs to be recalculated as normal impulse will have changed it
            velocityA = bodyA.GetVelocityAtPoint(contact);
            velocityB = bodyB.GetVelocityAtPoint(contact);

            relativeVelocity = velocityB - velocityA;

            // Find imulse due to friction
            Vector2 tangent = relativeVelocity - (manifold.normal * relativeVelocity.Dot(manifold.normal));

            if (tangent.MagnitudeSqrd() < 0.00001f){
                // Very small (or no) velocity along tangent
                tangent = Vector2::Zero();
            }
            tangent.Normalise();

            float velocityAlongTangent = relativeVelocity.Dot(tangent);

            float raCrossT = Vector2::Cross(rA, tangent);
            float rbCrossT = Vector2::Cross(rB, tangent);

            float frictionDenominator = bodyA.GetInverseMass() + bodyB.GetInverseMass() +
                (raCrossT * raCrossT * bodyA.GetInverseInertia()) +
                (rbCrossT * rbCrossT * bodyB.GetInverseInertia());

            float frictionMagnitude = -velocityAlongTangent / frictionDenominator;

            float maxFriction = std::sqrt(bodyA.friction * bodyB.friction) * impulseMagnitude;

            frictionMagnitude = std::clamp(frictionMagnitude, maxFriction, -maxFriction);

            if (frictionMagnitude < impulseSlop && -impulseSlop < frictionMagnitude){
                // Avoid jitter
                frictionMagnitude = 0.0f;
            }

            Vector2 friction = tangent * frictionMagnitude;

            std::cout << "Friction: " << friction << "\n";

            bodyA.ApplyImpulse(-friction, contact);
            bodyB.ApplyImpulse(friction, contact);
        }
    }
}