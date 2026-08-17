#include "CollisionDetector.hpp"

namespace n2p{
    AABB CollisionDetector::GetAABB(const Transform& transform, Circle shape){
        Vector2 extent(shape.GetRadius(), shape.GetRadius());

        return AABB(transform.position - extent, transform.position + extent);
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, Polygon shape){
        // TODO
    }

    std::vector<CollisionPair> CollisionDetector::BroadPhase(const std::vector<Rigidbody*> bodies) {
        size_t nBodies = bodies.size();

        for(size_t i = 0; i < nBodies; ++i){
            Rigidbody* bodyA = bodies[i];
            AABB boxA;
        }
    }
}