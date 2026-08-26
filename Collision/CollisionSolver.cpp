#include "CollisionSolver.hpp"
#include <algorithm>
#include <cmath>

namespace n2p{
    void CollisionSolver::ResolveCollision(Manifold& manifold){
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
        size_t nContacts = manifold.contacts.size();
        for (size_t i = 0; i < nContacts; ++i){
            Contact& contact = manifold.contacts[i];

            Vector2 rA = contact.point - bodyA.GetPosition();
            Vector2 rB = contact.point - bodyB.GetPosition();

            // Velocity at contact
            Vector2 velocityA = bodyA.GetVelocityAtPoint(contact.point);
            Vector2 velocityB = bodyB.GetVelocityAtPoint(contact.point);

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

            contact.normalImpulse = impulseMagnitude;

            bodyA.ApplyImpulse(-normalImpulse, contact.point);
            bodyB.ApplyImpulse(normalImpulse, contact.point);
        }


        for (size_t i = 0; i < nContacts; ++i){
            Contact& contact = manifold.contacts[i];

            Vector2 rA = contact.point - bodyA.GetPosition();
            Vector2 rB = contact.point - bodyB.GetPosition();

            // Velocity at contact needs to be recalculated as normal impulse will have changed it
            Vector2 velocityA = bodyA.GetVelocityAtPoint(contact.point);
            Vector2 velocityB = bodyB.GetVelocityAtPoint(contact.point);

            Vector2 relativeVelocity = velocityB - velocityA;

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

            float maxFriction = std::sqrt(bodyA.friction * bodyB.friction) * contact.normalImpulse;

            frictionMagnitude = std::clamp(frictionMagnitude, maxFriction, -maxFriction);

            if (frictionMagnitude < impulseSlop && -impulseSlop < frictionMagnitude){
                // Avoid jitter
                frictionMagnitude = 0.0f;
            }

            Vector2 friction = tangent * frictionMagnitude;

            contact.frictionImpulse = frictionMagnitude;

            bodyA.ApplyImpulse(-friction, contact.point);
            bodyB.ApplyImpulse(friction, contact.point);
        }
    }
}