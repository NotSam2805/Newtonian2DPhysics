#include "PhysicsWorld.hpp"
#include <vector>

namespace n2p{
    // Constructor
    PhysicsWorld::PhysicsWorld() {}

    void PhysicsWorld::Step(float dt) {
        /* 
        Performs one time step, moving dt seconds.
        Apply forces body -> Integrate body
        Loop through all bodies
        */

        for (Rigidbody* body : bodies){
            body->AddForce(gravity * body->GetMass());
            body->IntegrateVelocity(dt);
            body->ClearForces();
            body->ClearTorques();
        }

        std::vector<Manifold> collisionManifolds = CollisionDetector::DetectCollisions(bodies);

        for (Manifold& manifold : collisionManifolds){
            CollisionSolver::SolveCollision(manifold);
        }

        for (Rigidbody* body : bodies){
            body->IntegratePosition(dt);
        }
    }

    void PhysicsWorld::AddBody(Rigidbody* body) {
        bodies.push_back(body);
    }

    const std::vector<Rigidbody*> PhysicsWorld::GetBodies() const { return bodies; }
}