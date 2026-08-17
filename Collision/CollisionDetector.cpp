#include "CollisionDetector.hpp"

namespace n2p{
    AABB CollisionDetector::GetAABB(const Transform& transform, const Circle shape){
        Vector2 extent(shape.GetRadius(), shape.GetRadius());

        return AABB(transform.position - extent, transform.position + extent);
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Polygon shape){
        // TODO
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Shape* shape){
        ShapeType type = shape->GetType();

        switch (type)
        {
        case ShapeType::circle:
            GetAABB(transform, static_cast<const Circle*>(shape));
            break;
        
        default:
            break;
        }
    }

    std::vector<CollisionPair> CollisionDetector::BroadPhase(const std::vector<Rigidbody*> bodies) {
        std::vector<CollisionPair> potentialPairs;

        size_t nBodies = bodies.size();

        for(size_t i = 0; i < nBodies; ++i){
            Rigidbody* bodyA = bodies[i];
            AABB boxA = GetAABB(bodyA->GetTransform(), bodyA->GetShape());

            for(size_t j = i+1; j < nBodies; ++j){
                Rigidbody* bodyB = bodies[j];
                AABB boxB = GetAABB(bodyB->GetTransform(), bodyB->GetShape());

                if (boxA.Overlaps(boxB)){
                    potentialPairs.push_back(CollisionPair{bodyA, bodyB});
                }
            }
        }

        return potentialPairs;
    }

    bool CollisionDetector::CircleVCircle(const Rigidbody* circleBodyA, const Rigidbody* circleBodyB, Manifold* manifold){
        const Circle* circleA = static_cast<const Circle*>(circleBodyA->GetShape());
        const Circle* circleB = static_cast<const Circle*>(circleBodyB->GetShape());

        Vector2 difference = circleBodyB->GetPosition() - circleBodyA->GetPosition();

        float radius = circleA->GetRadius() + circleB->GetRadius();

        float distanceSqrd = difference.MagnitudeSqrd();

        if (distanceSqrd < radius * radius){
            manifold->normal = difference.Normalised();
            manifold->penetration = std::sqrt(distanceSqrd);
            manifold->contacts.push_back(
                Contact{
                    circleBodyA->GetPosition() + (difference.Normalised() * manifold->penetration),
                    manifold->penetration
                });
            manifold->colliding = true;

            return true;
        }

        return false;
    }

    bool CollisionDetector::DetectCollision(const Rigidbody* bodyA, const Rigidbody* bodyB, Manifold* manifold){
        ShapeType typeA = bodyA->GetShape()->GetType();
        ShapeType typeB = bodyB->GetShape()->GetType();

        switch (typeA)
        {
            case ShapeType::circle:
                switch (typeB)
                {
                    case ShapeType::circle:
                        return CircleVCircle(bodyA, bodyB, manifold);
                        break;
                
                    default:
                        break;
                }
                break;
        }
    }
    
    std::vector<Manifold> CollisionDetector::NarrowPhase(const std::vector<CollisionPair> potentialPairs){
        for(CollisionPair pair : potentialPairs){
            Manifold manifold;
            manifold.pair = pair;
        }
    }

    std::vector<Manifold> CollisionDetector::DetectCollisions(const std::vector<Rigidbody*> bodies){
        std::vector<CollisionPair> potentialPairs = BroadPhase(bodies);
        return NarrowPhase(potentialPairs);
    }
}