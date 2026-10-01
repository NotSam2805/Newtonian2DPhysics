#pragma once
#include <vector>
#include "Rigidbody.hpp"
#include "../Collision/Collision.hpp"

namespace n2p{
    class PhysicsWorld {
    private:
        std::vector<Rigidbody*> bodies;
    public:
        Vector2 gravity = Vector2();

        PhysicsWorld();
        
        void Step(float dt);

        void AddBody(Rigidbody* body);

        const std::vector<Rigidbody*> GetBodies() const;
    };
}