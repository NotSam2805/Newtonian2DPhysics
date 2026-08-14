#pragma once
#include "Shape.hpp"
#include <vector>
#include "../Maths/Vector2.hpp"
#include "../Maths/Transform.hpp"
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

            static Polygon Square(Colour colour) {
                return Polygon({
                    new Vector2(-1.0f, 1.0f),
                    new Vector2(1.0f, 1.0f),
                    new Vector2(1.0f, -1.0f),
                    new Vector2(-1.0f, -1.0f)
                }, colour);
            }

            const Vector2& GetWorldVertex(int index, Transform transform) const;

            // Getters
            const ShapeType GetType() const override;

            const std::vector<Vector2*>* GetVertices() const;

            const size_t GetVertexCount() const;
    };
}