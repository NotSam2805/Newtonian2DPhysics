#pragma once
#include "Shape.hpp"

namespace n2p{
    class Circle : public Shape{
        private:
            float radius;
        public:
            // Constructor
            explicit Circle(float radius);

            // Getters
            float GetRadius() const;

            const ShapeType GetType() const override;
    };
}