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
            body->Integrate(dt);
            body->ClearForces();
        }
    }

    void PhysicsWorld::AddBody(Rigidbody* body) {
        bodies.push_back(body);
    }

    const std::vector<Rigidbody*>& PhysicsWorld::GetBodies() const { return bodies; }
}