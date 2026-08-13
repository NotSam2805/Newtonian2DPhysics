#pragma once
#include "Shape.hpp"
#include "../Maths/Vector2.hpp"
#include "Colour.hpp"

namespace n2p{
    class Rect : public Shape{
        private:
            float width;
            float height;
        public:
            Colour colour;
            
            // Constructor
            explicit Rect(float width, float height, Colour colour);

            // Getters
            float GetWidth() const;

            float GetHeight() const;

            const ShapeType GetType() const override;
    };
}