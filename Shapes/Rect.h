#pragma once
#include "Shape.h"
#include "../Maths/Vector2.h"

namespace n2p{
    class Rect : public Shape{
        private:
            float width;
            float height;
        public:
            // Constructor
            explicit Rect(float width, float height);

            // Getters
            float GetWidth() const;

            float GetHeight() const;

            const ShapeType GetType() const override;
    };
}