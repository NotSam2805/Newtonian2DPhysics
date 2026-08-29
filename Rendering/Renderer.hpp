#pragma once
#include "../SDL2/include/SDL.h"
#include "../Physics/Physics.hpp"
#include "../Shapes/Shapes.hpp"
#include "Camera.hpp"

namespace n2p{
    class Renderer{
        private:
            SDL_Window *window;
            SDL_Renderer *renderer;

            Colour backgroundColour;

            Camera *camera;
            
            // Draw functions
            void DrawBody(const Rigidbody& body);

            void DrawCircle(const Rigidbody& circle);

            void DrawRect(const Rigidbody& rect);

            void DrawPolygon(const Rigidbody& poly);

        public:
            // Constructor
            Renderer(int windowWidth, int windowHeight, Camera camera = {Vector2::Zero(), 1.0f}, Colour backgroundColour = Colour{33,33,33,255});
            // Destructor
            ~Renderer() = default;

            // Clear to background colour
            void Clear();

            // Draw all bodies in world
            void DrawWorld(const PhysicsWorld& world);

            // Show rendered image
            void Present();

            // Returns if the position is on the screen
            bool IsInScreenBounds(const Vector2& screenPosition);

            // Returns the screen position for any world position
            Vector2 WorldToScreen(const Vector2& world);

            void Quit();
    };
}