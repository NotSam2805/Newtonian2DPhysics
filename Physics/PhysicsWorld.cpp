#include "PhysicsWorld.hpp"
#include <vector>

namespace n2p{
    // Constructor
    PhysicsWorld::PhysicsWorld() : collisionDetector() {}

    void PhysicsWorld::Step(float dt) {
        /* 
        Performs one time step, moving dt seconds.
        Apply forces body -> Integrate body
        Loop through all bodies
        */

        for (Rigidbody* body : bodies){
            body->IntegrateVelocity(dt);
            body->ClearForces();
            body->ClearTorques();
        }

        std::vector<Manifold> collisionManifolds = collisionDetector.DetectCollisions(bodies);

        for (Manifold& manifold : collisionManifolds){
            // collisionSolver.Resolve(manifold)
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