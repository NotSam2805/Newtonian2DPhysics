#pragma once
#include "Shape.hpp"
#include <vector>
#include "../Maths/Vector2.hpp"
#include "Colour.hpp"

namespace n2p {
    class Polygon : public Shape{
        private:
            // Polygon is bound by straight lines between verticies. Vertices are defined in local space, world position = vertice + shape.worldPosition.
            std::vector<Vector2*> vertices;
        public:
            Colour colour;

            // Constructor
            explicit Polygon(std::vector<Vector2*> vertices, Colour colour);

            // Getters
            const ShapeType GetType() const override;

            const std::vector<Vector2*>* GetVertices() const;
    };
}