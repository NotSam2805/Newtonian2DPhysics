#include "CollisionSolver.hpp"
#include <algorithm>
#include <cmath>

namespace n2p{

    void CollisionSolver::CorrectPosition(const Manifold& manifold){
        Rigidbody& bodyA = *manifold.pair.bodyA;
        Rigidbody& bodyB = *manifold.pair.bodyB;


        float totalInverseMass = bodyA.GetInverseMass() + bodyB.GetInverseMass();

        // Positional correction
        float correctionAmount = (manifold.penetration / totalInverseMass) * correctionPercent;
        correctionAmount = correctionAmount < slop ? 0.0f : correctionAmount;
        Vector2 correction = manifold.normal * correctionAmount;

        bodyA.Move(-correction * bodyA.GetInverseMass());
        bodyB.Move(correction * bodyB.GetInverseMass());
    }

    void CollisionSolver::FindTargetVelocities(Manifold& manifold){
        Rigidbody& bodyA = *manifold.pair.bodyA;
        Rigidbody& bodyB = *manifold.pair.bodyB;

        float restitution = bodyA.restitution * bodyB.restitution;

        size_t nContacts = manifold.contacts.size();
        for (size_t i = 0; i < nContacts; ++i){
            Contact& contact = manifold.contacts[i];

            Vector2 relativeV = bodyB.GetVelocityAtPoint(contact.point) - bodyA.GetVelocityAtPoint(contact.point);

            float normalV = relativeV.Dot(manifold.normal);

            contact.targetVelocity = -normalV * restitution;
        }
    }

    void CollisionSolver::SolveNormal(Manifold& manifold){
        Rigidbody& bodyA = *manifold.pair.bodyA;
        Rigidbody& bodyB = *manifold.pair.bodyB;
        
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

            float raCrossN = Vector2::Cross(rA, manifold.normal);
            float rbCrossN = Vector2::Cross(rB, manifold.normal);

            float denominator = bodyA.GetInverseMass() + bodyB.GetInverseMass() +
                (raCrossN * raCrossN * bodyA.GetInverseInertia()) +
                (rbCrossN * rbCrossN * bodyB.GetInverseInertia());

            if (denominator <= 0.0f){
                continue;
            }

            float deltaImpulse = (contact.targetVelocity - normalVelocity) / denominator;
            
            float newImpulse = contact.normalImpulse + deltaImpulse;

            // Impulse cannot be negative, this would cause collisions to pull bodies together
            newImpulse = std::max(newImpulse, 0.0f);

            float impulseChange = newImpulse - contact.normalImpulse;

            contact.normalImpulse = newImpulse;

            if(impulseChange < impulseSlop){
                continue;
            }

            // Only apply the change
            Vector2 impulse = manifold.normal * impulseChange;

            bodyA.ApplyImpulse(-impulse, contact.point);
            bodyB.ApplyImpulse(impulse, contact.point);
        }
    }

    void CollisionSolver::SolveTangent(Manifold& manifold){
        // Solve for friction
        Rigidbody& bodyA = *manifold.pair.bodyA;
        Rigidbody& bodyB = *manifold.pair.bodyB;

        size_t nContacts = manifold.contacts.size();
        for (size_t i = 0; i < nContacts; ++i){
            Contact& contact = manifold.contacts[i];

            Vector2 rA = contact.point - bodyA.GetPosition();
            Vector2 rB = contact.point - bodyB.GetPosition();

            // Velocity at contact
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

            float frictionDelta = -velocityAlongTangent / frictionDenominator;

            float maxFriction = std::sqrt(bodyA.friction * bodyB.friction) * contact.normalImpulse;

            frictionDelta = std::clamp(frictionDelta, -maxFriction, maxFriction);

            
            contact.frictionImpulse += frictionDelta;

            Vector2 impulse = tangent * frictionDelta;

            bodyA.ApplyImpulse(-impulse, contact.point);
            bodyB.ApplyImpulse(impulse, contact.point);
        }
    }

    void CollisionSolver::SolveCollision(Manifold& manifold){
        if (!manifold.colliding){
            return;
        }

        CorrectPosition(manifold);

        FindTargetVelocities(manifold);

        for (int i = 0; i < iterations; ++i){
            SolveNormal(manifold);
            SolveTangent(manifold);
        }
    }
}