#include "CollisionDetector.hpp"
#include <limits>
#include <cmath>

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

    AABB CollisionDetector::GetAABB(const Transform& transform, const Circle& shape){
        Vector2 extent(shape.GetRadius(), shape.GetRadius());

        return AABB(transform.position - extent, transform.position + extent);
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Polygon& shape){
        size_t vertexCount = shape.GetVertexCount();
        AABB box(
            Vector2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
            Vector2(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max())
        );

        for (size_t i = 0; i < vertexCount; ++i){
            Vector2 vertex = shape.GetWorldVertex(i, transform);

            if (vertex.x < box.min.x){
                box.min.x = vertex.x;
            }
            else if (vertex.x > box.max.x){
                box.max.x = vertex.x;
            }

            if (vertex.y < box.min.y){
                box.min.y = vertex.y;
            }
            else if (vertex.y > box.max.y){
                box.max.y = vertex.y;
            }
        }

        return box;
    }

    AABB CollisionDetector::GetAABB(const Transform& transform, const Shape& shape){
        ShapeType type = shape.GetType();

        switch (type)
        {
        case ShapeType::circle:
            return GetAABB(transform, static_cast<const Circle&>(shape));
            break;
        
        case ShapeType::polygon:
            return GetAABB(transform, static_cast<const Polygon&>(shape));
            break;
        
        default:
            break;
        }

        return AABB();
    }

    int CollisionDetector::ClipSegment(Vector2 out[2], const Vector2 in[2], const Vector2& normal, float offset)
    {
        int count = 0;

        float distanceA = normal.Dot(in[0]) - offset;
        
        float distanceB = normal.Dot(in[1]) - offset;

        // Point A is inside
        if (distanceA <= 0.0f) {
            out[count++] = in[0];
        }

        // Point B is inside
        if (distanceB <= 0.0f) {
            out[count++] = in[1];
        }

        // Segment crosses the plane
        if ((distanceA < 0.0f && distanceB > 0.0f) || (distanceA > 0.0f && distanceB < 0.0f)) {
            float t = distanceA / (distanceA - distanceB);
            
            Vector2 intersection = in[0] + (in[1] - in[0]) * t;

            out[count++] = intersection;
        }

        return count;
    }

    // Uses AABB to find potential pairs
    std::vector<CollisionPair> CollisionDetector::BroadPhase(const std::vector<Rigidbody*>& bodies) {
        std::vector<CollisionPair> potentialPairs;

        size_t nBodies = bodies.size();

        for(size_t i = 0; i < nBodies; ++i){
            Rigidbody* bodyA = bodies[i];
            AABB boxA = GetAABB(bodyA->GetTransform(), *bodyA->GetShape());

            for(size_t j = i+1; j < nBodies; ++j){
                Rigidbody* bodyB = bodies[j];
                AABB boxB = GetAABB(bodyB->GetTransform(), *bodyB->GetShape());

                if (boxA.Overlaps(boxB)){
                    potentialPairs.push_back(CollisionPair{bodyA, bodyB});
                }
            }
        }

        return potentialPairs;
    }

    bool CollisionDetector::CircleVCircle(const Rigidbody& circleBodyA, const Rigidbody& circleBodyB, Manifold& manifold){
        const Circle* circleA = static_cast<const Circle*>(circleBodyA.GetShape());
        const Circle* circleB = static_cast<const Circle*>(circleBodyB.GetShape());

        Vector2 difference = circleBodyB.GetPosition() - circleBodyA.GetPosition();

        float radius = circleA->GetRadius() + circleB->GetRadius();

        float distanceSqrd = difference.MagnitudeSqrd();

        if (distanceSqrd < radius * radius){
            manifold.normal = difference.Normalised();
            manifold.penetration = std::sqrt(distanceSqrd);
            manifold.contacts.push_back(
                Contact{circleBodyA.GetPosition() + (difference.Normalised() * manifold.penetration)}
            );
            manifold.colliding = true;

            return true;
        }

        return false;
    }

    bool CollisionDetector::PolyVCircle(const Rigidbody& polyBody, const Rigidbody& circleBody, Manifold& manifold){
        const Polygon* poly = static_cast<const Polygon*>(polyBody.GetShape());
        const Circle* circle = static_cast<const Circle*>(circleBody.GetShape());

        float minimumOverlap = std::numeric_limits<float>::max();

        Vector2 collisionNormal;

        size_t vertexCount = poly->GetVertexCount();

        for(size_t i = 0; i < vertexCount; ++i){
            // Find edge, and normal to edge (axis)
            Vector2 a = poly->GetWorldVertex(i, polyBody.GetTransform());
            Vector2 b = poly->GetWorldVertex((i + 1) % vertexCount, polyBody.GetTransform());

            Vector2 edge = b - a;

            Vector2 axis = Vector2(-edge.y, edge.x).Normalised();

            // Project shapes to axis, check for overlap
            Projection polyProjection = ProjectPolygon(*poly, polyBody.GetTransform(), axis);
            Projection circleProjection = ProjectCircle(*circle, circleBody.GetTransform(), axis);

            if (!Overlap(polyProjection, circleProjection)){
                return false;
            }

            // Find the penetration, as the minimum overlap
            float overlap = GetOverlap(polyProjection, circleProjection);
            if (overlap < minimumOverlap){
                minimumOverlap = overlap;
                collisionNormal = axis;
            }
        }

        // Check axis from closest vertex to circle
        Vector2 circleCentre = circleBody.GetPosition();

        Vector2 closestVertex;

        float closestDistanceSqrd = std::numeric_limits<float>::max();

        for(size_t i = 0; i < vertexCount; ++i){
            Vector2 vertex = poly->GetWorldVertex(i, polyBody.GetTransform());

            Vector2 difference = vertex - circleCentre;

            float distanceSqrd = difference.MagnitudeSqrd();
            if (distanceSqrd < closestDistanceSqrd){
                closestDistanceSqrd = distanceSqrd;
                closestVertex = vertex;
            }
        }

        Vector2 axis = closestVertex - circleCentre;
        axis.Normalise();

        Projection circleProjection = ProjectCircle(*circle, circleBody.GetTransform(), axis);
        Projection polyProjection = ProjectPolygon(*poly, polyBody.GetTransform(), axis);

        if (!Overlap(circleProjection, polyProjection)){
            return false;
        }

        float overlap = GetOverlap(polyProjection, circleProjection);
        if (overlap < minimumOverlap){
            minimumOverlap = overlap;
            collisionNormal = axis;
        }

        /* Calculate manifold:
         * - Determine direction of normal
         * - Find contact point as point on circumference of circle
         */
        Vector2 direction = polyBody.GetPosition() - circleBody.GetPosition();

        if (direction.Dot(collisionNormal) < 0.0f){
            collisionNormal = -collisionNormal;
        }

        manifold.normal = collisionNormal;
        manifold.penetration = minimumOverlap;
        manifold.contacts.push_back(
            Contact{circleBody.GetPosition() + (collisionNormal * circle->GetRadius())}
        );
        
        return true;
    }

    bool CollisionDetector::PolyVPoly(const Rigidbody& bodyA, const Rigidbody& bodyB, Manifold& manifold){
        const Polygon* polyA = static_cast<const Polygon*>(bodyA.GetShape());
        const Polygon* polyB = static_cast<const Polygon*>(bodyB.GetShape());

        float minimumOverlap = std::numeric_limits<float>::max();
        Vector2 collisionNormal;
        Vector2 referenceEdgeA;
        Vector2 referenceEdgeB;
        bool referenceIsA = true;

        // Check all axis from edges from polyA
        size_t vertexCount = polyA->GetVertexCount();

        for (size_t i = 0; i < vertexCount; ++i){
            Vector2 a = polyA->GetWorldVertex(i, bodyA.GetTransform());
            Vector2 b = polyA->GetWorldVertex((i + 1) % vertexCount, bodyA.GetTransform());

            Vector2 edge = b - a;
            Vector2 axis(-edge.y, edge.x);
            axis.Normalise();

            Projection projectionA = ProjectPolygon(*polyA, bodyA.GetTransform(), axis);
            Projection projectionB = ProjectPolygon(*polyB, bodyB.GetTransform(), axis);

            if (!Overlap(projectionA, projectionB)){
                return false;
            }

            float overlap = GetOverlap(projectionA, projectionB);

            if (overlap <= minimumOverlap){
                minimumOverlap = overlap;
                collisionNormal = axis;

                referenceEdgeA = a;
                referenceEdgeB = b;
            }
        }

        // Repeat for polyB
        vertexCount = polyB->GetVertexCount();

        for (size_t i = 0; i < vertexCount; ++i){
            Vector2 a = polyB->GetWorldVertex(i, bodyB.GetTransform());
            Vector2 b = polyB->GetWorldVertex((i + 1) % vertexCount, bodyB.GetTransform());

            Vector2 edge = b - a;
            Vector2 axis(-edge.y, edge.x);
            axis.Normalise();

            Projection projectionA = ProjectPolygon(*polyA, bodyA.GetTransform(), axis);
            Projection projectionB = ProjectPolygon(*polyB, bodyB.GetTransform(), axis);

            if (!Overlap(projectionA, projectionB)){
                return false;
            }

            float overlap = GetOverlap(projectionA, projectionB);

            if (overlap <= minimumOverlap){
                minimumOverlap = overlap;
                collisionNormal = axis;

                referenceEdgeA = a;
                referenceEdgeB = b;
                referenceIsA = false;
            }
        }

        /* Calculate manifold:
         * - Determine direction of normal
         * - Find contact points
         *      + Use normal to find corresponding incident edge
         *      + Clips incident edge along reference edge
         */

        Vector2 direction = bodyB.GetPosition() - bodyA.GetPosition();

        if (direction.Dot(collisionNormal) > 0.0f){
            collisionNormal = -collisionNormal;
        }

        manifold.normal = collisionNormal;
        manifold.penetration = minimumOverlap;

        
        const Polygon* referencePoly = referenceIsA ? polyA : polyB;
        const Rigidbody& referenceBody = referenceIsA ? bodyA : bodyB;
        const Polygon* incidentPoly = referenceIsA ? polyB : polyA;
        const Rigidbody& incidentBody = referenceIsA ? bodyB : bodyA;

        Vector2 referenceNormal = referenceIsA ? -collisionNormal : collisionNormal;

        // Find the vertices of the incident edge by finding the edge on the incident object with the smallest dot from the collision normal
        Vector2 incidentEdgeA;
        Vector2 incidentEdgeB;
        float dot = std::numeric_limits<float>::max();

        vertexCount = incidentPoly->GetVertexCount();
        for (size_t i = 0; i < vertexCount; ++i){
            Vector2 a = incidentPoly->GetWorldVertex(i, incidentBody.GetTransform());
            Vector2 b = incidentPoly->GetWorldVertex((i+1)%vertexCount, incidentBody.GetTransform());

            Vector2 edge = b - a;
            Vector2 edgeNormal = Vector2(-edge.y, edge.x).Normalised();

            if (edgeNormal.Dot(referenceNormal) <= dot){
                incidentEdgeA = a;
                incidentEdgeB = b;
                dot = edgeNormal.Dot(referenceNormal);
            }
        }

        /* Find points along reference edge that fall on incident edge by clipping.
         * Normally 2 points, one at the "lowest" and one at the "highest".
        */

        Vector2 tangent = (referenceEdgeB - referenceEdgeA).Normalised();

        Vector2 clipPoints[2] = {
            incidentEdgeA,
            incidentEdgeB
        };

        Vector2 output[2];

        // Clip against first side of referenceEdge
        int count = ClipSegment(output, clipPoints, -tangent, -tangent.Dot(referenceEdgeA));

        if (count < 2) {
            // No valid incident edge
            return false;
        }

        // Clip against other side of referenceEdge
        Vector2 clippedPoints[2];

        count = ClipSegment(clippedPoints, output, tangent, tangent.Dot(referenceEdgeB));

        if (count == 0){
            // No valid edge
            return false;
        }

        float referenceOffset = referenceNormal.Dot(referenceEdgeA);

        for (int i = 0; i < count; ++i){
            float seperation = referenceNormal.Dot(clippedPoints[i]) - referenceOffset;

            if (seperation <= 0.0f){
                manifold.contacts.push_back(Contact{clippedPoints[i]});
            }
        }

        return true;
    }

    // Collsions for Polygons uses SAT - which only works for convex shapes
    bool CollisionDetector::DetectCollision(const Rigidbody& bodyA, const Rigidbody& bodyB, Manifold& manifold){
        ShapeType typeA = bodyA.GetShape()->GetType();
        ShapeType typeB = bodyB.GetShape()->GetType();

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
            if (DetectCollision(*pair.bodyA, *pair.bodyB, manifold)){
                manifold.colliding = true;
                manifolds.push_back(manifold);
            }
        }

        return manifolds;
    }

    std::vector<Manifold> CollisionDetector::DetectCollisions(const std::vector<Rigidbody*>& bodies){
        std::vector<CollisionPair> potentialPairs = BroadPhase(bodies);
        return NarrowPhase(potentialPairs);
    }
}