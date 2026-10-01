#pragma once
#include "Contact.hpp"
#include "AABB.hpp"
#include <vector>

namespace n2p{

    struct Projection{
        float min;
        float max;
    };
    
    class CollisionDetector
    {
        private:
            static Projection ProjectPolygon(const Polygon& polygon, const Transform& transform, const Vector2& axis);
            static Projection ProjectCircle(const Circle& circle, const Transform& transform, const Vector2& axis);
            static bool Overlap(const Projection& a, const Projection& b);
            static float GetOverlap(const Projection& a, const Projection& b);

            static int ClipSegment(Vector2 out[2], const Vector2 in[2], const Vector2& normal, float offset);

            static AABB GetAABB(const Transform& transform, const Circle& shape);
            static AABB GetAABB(const Transform& transform, const Polygon& shape);
            static AABB GetAABB(const Transform& transform, const Shape& shape);

            static std::vector<CollisionPair> BroadPhase(const std::vector<Rigidbody*>& bodies);
            static std::vector<Manifold> NarrowPhase(const std::vector<CollisionPair> potentialPairs);

            static bool DetectCollision(const Rigidbody& bodyA, const Rigidbody& bodyB, Manifold& manifold);
            static bool CircleVCircle(const Rigidbody& circleBodyA, const Rigidbody& circleBodyB, Manifold& manifold);
            static bool PolyVCircle(const Rigidbody& polyBody, const Rigidbody& circleBody, Manifold& manifold);
            static bool PolyVPoly(const Rigidbody& polyBodyA, const Rigidbody& polyBodyB, Manifold& manifold);
        public:
            // Returns collision manifolds for all confirmed collisions
            static std::vector<Manifold> DetectCollisions(const std::vector<Rigidbody*>& bodies);
    };
}