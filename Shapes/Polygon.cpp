#include "Polygon.h"
#include "Shape.h"
#include <vector>

namespace n2p{
    // Constructor
    explicit Polygon::Polygon(std::vector<Vector2*> vertices) : vertices(vertices) {}

    // Getters
    const ShapeType Polygon::GetType() const{ return ShapeType::Polygon; }

    const std::vector<Vector2*>* Polygon::GetVertices() const { return &vertices; }
}