#include "Polygon.hpp"
#include "Shape.hpp"
#include <vector>

namespace n2p{
    // Constructor
    explicit Polygon::Polygon(std::vector<Vector2*> vertices) : vertices(vertices) {}

    // Getters
    const ShapeType Polygon::GetType() const{ return ShapeType::polygon; }

    const std::vector<Vector2*>* Polygon::GetVertices() const { return &vertices; }
}