#include "Rect.hpp"

namespace n2p{
    // Constructor
    Rect::Rect(float width, float height, Colour colour) : width(width), height(height), colour(colour) {};

    // Getters
    float Rect::GetHeight() const { return height; }
    
    float Rect::GetWidth() const { return width; }

    const ShapeType Rect::GetType() const { return ShapeType::rect; }
}