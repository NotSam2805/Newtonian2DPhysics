#include "Rect.hpp"

namespace n2p{
    // Constructor
    Rect::Rect(float width, float height) : width(width), height(height) {};

    // Getters
    float Rect::GetHeight() const { return height; }
    
    float Rect::GetWidth() const { return width; }

    const ShapeType Rect::GetType() const { return ShapeType::rect; }
}