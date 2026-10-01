#pragma once
#include "Shape.hpp"
#include "Colour.hpp"

namespace n2p{
    class Circle : public Shape{
        private:
            float radius;
        public:
            Colour colour;

            // Constructor
            explicit Circle(float radius, Colour colour);

            // Getters
            float GetRadius() const;

            const ShapeType GetType() const override;
    };
}