#pragma once
#include "Shape.h"
#include <vector>
#include "../Maths/Vector2.h"

namespace n2p {
    class Polygon : public Shape{
        private:
            // Polygon is bound by straight lines between verticies. Vertices are defined in local space, world position = vertice + shape.worldPosition.
            std::vector<Vector2*> vertices;
        public:
            // Constructor
            explicit Polygon(std::vector<Vector2*> vertices);

            // Getters
            const ShapeType GetType() const override;

            const std::vector<Vector2*>* GetVertices() const;
    };
}