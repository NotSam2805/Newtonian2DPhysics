#pragma once
#include "../SDL2/include/SDL.h"
#include "Colour.hpp"
#include "../Physics/Physics.hpp"
#include "../Shapes/Shapes.hpp"

namespace n2p{
    class Renderer{
        private:
            SDL_Window *window;
            SDL_Renderer *renderer;

            Colour backgroundColour;

        public:
            Renderer();
            ~Renderer();

            void Clear();

            void DrawWorld(const PhysicsWorld& world);

            void DrawBody(const Rigidbody& body);

            void DrawCircle(const Circle& circle);

            void DrawRect(const Rect& rect);

            void DrawPolygon(const Polygon& poly);

            void Present();
    };
}