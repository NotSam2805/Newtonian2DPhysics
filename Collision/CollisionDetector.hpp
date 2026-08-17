#pragma once
#include "Contact.hpp"
#include "AABB.hpp"
#include <vector>

namespace n2p{
    class CollisionDetector
    {
        private:
            AABB GetAABB(const Transform& transform, Circle shape);
            AABB GetAABB(const Transform& transform, Polygon shape);

            std::vector<CollisionPair> BroadPhase(const std::vector<Rigidbody*> bodies);
            std::vector<Manifold> NarrowPhase(const std::vector<CollisionPair> potentialPairs);
        public:
            CollisionDetector();
            ~CollisionDetector();

            // Returns collision manifolds for all confirmed collisions
            std::vector<Manifold> DetectCollisions(const std::vector<Rigidbody*> bodies);
    };
}