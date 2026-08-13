#include <iostream>
#include <memory>
#include "Physics/Physics.hpp"
#include "Rendering/Renderer.hpp"
#include "SDL2/include/SDL.h"

#include <chrono>

using namespace std::chrono_literals;
using namespace n2p;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

// Returns true if quit event
bool HandleEvents(){
    SDL_Event windowEvent;

    if (SDL_PollEvent(&windowEvent)){
        if(SDL_QUIT == windowEvent.type){
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[]){
    using clock = std::chrono::high_resolution_clock;

    std::chrono::nanoseconds lag(0ns);

    auto timestart = clock::now();

    bool running = true;

    // Keep track of which frame we are on
    int frameCount = 0;


    // Setup initial physics world
    PhysicsWorld world{};
    Renderer renderer{800, 600};

    // Make a blue square
    Rigidbody square(
        1.0f, Transform(),
        std::make_unique<Rect>(10.0f, 10.0f, n2p::Colour{0,0,255,SDL_ALPHA_OPAQUE})
    );
    
    // Make a red ball
    Rigidbody ball(
        1.0f,
        Transform{Vector2(25.0f,0.0f)}, std::make_unique<Circle>(0.5f, n2p::Colour{255,0,0,SDL_ALPHA_OPAQUE})
    );

    world.AddBody(&square);
    world.AddBody(&ball);

    // Add an upwards force to the square
    square.AddForce(Vector2(0.0f, 0.5f));
    // Add a downwards force to the ball
    ball.AddForce(Vector2(0.0f, -1.0f));

    while (running){
        auto deltatime = clock::now() - timestart;
        timestart = clock::now();

        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltatime);

        if (HandleEvents()){
            // Quit
            running = false;
        }

        if (lag >= timestep){
            frameCount++;
            frameCount %= 60;

            world.Step(std::chrono::duration_cast<std::chrono::milliseconds>(timestep).count() / 1000.0f);

            // Update renderer on every second frame
            if (frameCount % 2 == 0){
                renderer.Clear();
                renderer.DrawWorld(world);
            }

            lag -= timestep;
        }

        // calculate how close or far we are from the next timestep (for future interpolation)
        // auto alpha = (float) lag.count() / timestep.count();
    }

    renderer.Quit();

    return 0;
}