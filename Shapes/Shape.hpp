#pragma once

namespace n2p{
    enum ShapeType {
        circle,
        rect,
        polygon
    };

    class Shape {
        public:
            virtual ~Shape() = default;

            const virtual ShapeType GetType() const = 0;
    };
}