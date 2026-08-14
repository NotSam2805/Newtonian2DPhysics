#include "Polygon.hpp"
#include "Shape.hpp"
#include <vector>

namespace n2p{
    // Constructor
    Polygon::Polygon(std::vector<Vector2*> vertices, Colour colour) : vertices(vertices), colour(colour) {}

    // Getters
    const ShapeType Polygon::GetType() const{ return ShapeType::polygon; }

    const std::vector<Vector2*>* Polygon::GetVertices() const { return &vertices; }

    const size_t Polygon::GetVertexCount() const { return vertices.size(); }

    const Vector2& Polygon::GetWorldVertex(int index, Transform transform) const{
        Vector2* vertex = vertices[index];
        Vector2 worldVertex = vertex->Rotate(transform.rotation);
        worldVertex += transform.position;
        return worldVertex;
    }
}