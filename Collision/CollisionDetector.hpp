#pragma once
#include "Contact.hpp"
#include "AABB.hpp"
#include <vector>

namespace n2p{
    class CollisionDetector
    {
        private:
            AABB GetAABB(const Transform& transform, const Circle shape);
            AABB GetAABB(const Transform& transform, const Polygon shape);
            AABB GetAABB(const Transform& transform, const Shape* shape);

            std::vector<CollisionPair> BroadPhase(const std::vector<Rigidbody*> bodies);
            std::vector<Manifold> NarrowPhase(const std::vector<CollisionPair> potentialPairs);

            bool DetectCollision(const Rigidbody* bodyA, const Rigidbody* bodyB, Manifold* manifold);
            bool CircleVCircle(const Rigidbody* circleBodyA, const Rigidbody* circleBodyB, Manifold* manifold);
            bool PolyVCircle(const Rigidbody* circleBodyA, const Rigidbody* circleBodyB, Manifold* manifold);
        public:
            CollisionDetector();
            ~CollisionDetector();

            // Returns collision manifolds for all confirmed collisions
            std::vector<Manifold> DetectCollisions(const std::vector<Rigidbody*> bodies);
    };
}