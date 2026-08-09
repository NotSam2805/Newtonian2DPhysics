#pragma once
#include "Shape.h"

namespace n2p{
    class Circle : public Shape{
        private:
            float radius;
        public:
            explicit Circle(float radius);

            float GetRadius() const;

            ShapeType GetType() const override;
    };
}