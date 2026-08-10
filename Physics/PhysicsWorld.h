#pragma once
#include <vector>
#include "Rigidbody.h"

namespace n2p{
    class PhysicsWorld {
    private:
        std::vector<Rigidbody*> bodies;
    public:
        PhysicsWorld();
        
        void Step(float dt);

        void AddBody(Rigidbody* body);

        const std::vector<Rigidbody*>* GetBodies() const;
    };
}