#include "CollisionDetector.hpp"

namespace n2p{

    Projection CollisionDetector::ProjectPolygon(const Polygon& polygon, const Transform& transform, const Vector2& axis) {
        float min = std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::lowest();

        for (size_t i = 0; i < polygon.GetVertexCount(); ++i)
        {
            Vector2 vertex = polygon.GetWorldVertex(i, transform);

            float projection = vertex.Dot(axis);

            min = std::min(min, projection);
            max = std::max(max, projection);
        }

        return { min, max };
    }

    Projection CollisionDetector::ProjectCircle(const Circle& circle, const Transform& transform, const Vector2& axis){
        float center = transform.position.Dot(axis);
        float radius = circle.GetRadius();

        return { center - radius, center + radius };
    }

    bool CollisionDetector::Overlap(const Projection& a, const Projection& b){
        return a.max >= b.min && b.max >= a.min;
    }

    float CollisionDetector::GetOverlap(const Projection& a, const Projection& b) {
        return std::min(a.max, b.max) - std::max(a.min, b.min);
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Circle* shape){
        Vector2 extent(shape->GetRadius(), shape->GetRadius());

        return AABB(transform.position - extent, transform.position + extent);
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Polygon* shape){
        // TODO
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Shape* shape){
        ShapeType type = shape->GetType();

        switch (type)
        {
        case ShapeType::circle:
            GetAABB(transform, static_cast<const Circle*>(shape));
            break;
        
        case ShapeType::polygon:
            GetAABB(transform, static_cast<const Polygon*>(shape));
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
                }
            );
            manifold->colliding = true;

            return true;
        }

        return false;
    }

    bool CollisionDetector::PolyVCircle(const Rigidbody* polyBody, const Rigidbody* circleBody, Manifold* manifold){
        const Polygon* poly = static_cast<const Polygon*>(polyBody->GetShape());
        const Circle* circle = static_cast<const Circle*>(polyBody->GetShape());

        float minimumOverlap = std::numeric_limits<float>::max();

        Vector2 collisionNormal;

        size_t vertexCount = poly->GetVertexCount();

        for(size_t i = 0; i < vertexCount; ++i){
            Vector2 a = poly->GetWorldVertex(i, polyBody->GetTransform());
            Vector2 b = poly->GetWorldVertex((i + 1) % vertexCount, polyBody->GetTransform());

            Vector2 edge = b - a;

            Vector2 axis = Vector2(-edge.y, edge.x).Normalised();

            Projection polyProjection = ProjectPolygon(*poly, polyBody->GetTransform(), axis);
            Projection circleProjection = ProjectCircle(*circle, circleBody->GetTransform(), axis);

            if (!Overlap(polyProjection, circleProjection)){
                return false;
            }

            float overlap = GetOverlap(polyProjection, circleProjection);
            if (overlap < minimumOverlap){
                minimumOverlap = overlap;
                collisionNormal = axis;
            }
        }


        Vector2 circleCentre = circleBody->GetPosition();

        Vector2 closestVertex;

        float closestDistanceSqrd = std::numeric_limits<float>::max();

        for(size_t i = 0; i < vertexCount; ++i){
            Vector2 vertex = poly->GetWorldVertex(i, polyBody->GetTransform());

            Vector2 difference = vertex - circleCentre;

            float distanceSqrd = difference.MagnitudeSqrd();
            if (distanceSqrd < closestDistanceSqrd){
                closestDistanceSqrd = distanceSqrd;
                closestVertex = vertex;
            }
        }

        Vector2 axis = closestVertex - circleCentre;
        axis.Normalise();

        Projection circleProjection = ProjectCircle(*circle, circleBody->GetTransform(), axis);
        Projection polyProjection = ProjectPolygon(*poly, polyBody->GetTransform(), axis);

        if (!Overlap(circleProjection, polyProjection)){
            return false;
        }

        float overlap = GetOverlap(polyProjection, circleProjection);
        if (overlap < minimumOverlap){
            minimumOverlap = overlap;
            collisionNormal = axis;
        }


        Vector2 direction = polyBody->GetPosition() - circleBody->GetPosition();

        if (direction.Dot(collisionNormal) < 0.0f){
            collisionNormal = -collisionNormal;
        }

        manifold->normal = collisionNormal;
        manifold->penetration = minimumOverlap;
        manifold->contacts.push_back(
            Contact{
                circleBody->GetPosition() + (collisionNormal * circle->GetRadius()),
                manifold->penetration
            }
        );
        
        return true;
    }

    bool CollisionDetector::PolyVPoly(const Rigidbody* polyBody, const Rigidbody* circleBody, Manifold* manifold){
        // TODO
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
                    
                    case ShapeType::polygon:
                        return PolyVCircle(bodyB, bodyA, manifold);
                    default:
                        break;
                }
                break;
            
            case ShapeType::polygon:
                switch (typeB)
                {
                    case ShapeType::circle:
                        return PolyVCircle(bodyA, bodyB, manifold);
                        break;
                    
                    case ShapeType::polygon:
                        return PolyVPoly(bodyB, bodyA, manifold);
                    default:
                        break;
                }
                break;
            
            default:
                break;
        }

        return false;
    }
    
    std::vector<Manifold> CollisionDetector::NarrowPhase(const std::vector<CollisionPair> potentialPairs){
        std::vector<Manifold> manifolds;

        for(CollisionPair pair : potentialPairs){
            Manifold manifold;
            manifold.pair = pair;
            if (DetectCollision(pair.bodyA, pair.bodyB, &manifold)){
                manifold.colliding = true;
                manifolds.push_back(manifold);
            }
        }

        return manifolds;
    }

    std::vector<Manifold> CollisionDetector::DetectCollisions(const std::vector<Rigidbody*> bodies){
        std::vector<CollisionPair> potentialPairs = BroadPhase(bodies);
        return NarrowPhase(potentialPairs);
    }
}