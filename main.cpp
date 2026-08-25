#include <iostream>
#include <memory>
#include <vector>

#include "Physics/Physics.hpp"
#include "Rendering/Renderer.hpp"
#include "Shapes/Shapes.hpp"
#include "SDL2/include/SDL.h"

#include <chrono>

using namespace std::chrono_literals;
using namespace n2p;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

// Returns true if quit event
bool HandleEvents(){
    SDL_Event windowEvent;

    while (SDL_PollEvent(&windowEvent)){
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
    Renderer renderer{800, 600, Camera{Vector2::Zero(), 10.0f}};

    Rigidbody wall(
        0.0f,
        Transform{Vector2(-25.0f, 0.0f), 0.0f},
        std::make_unique<Polygon>(
            std::vector<Vector2*> {
                new Vector2(1.0f, 10.0f),
                new Vector2(1.0f, -10.0f),
                new Vector2(-1.0f,-10.0f),
                new Vector2(-1.0f,10.0f)
            },
            Colour{200,200,200,255}
        ),
        0.0f
    );
    wall.restitution = 1.0f;
    world.AddBody(&wall);

    Rigidbody box1(
        1.0f,
        Transform{Vector2(0.0f, 0.0f), 0.0f},
        std::make_unique<Polygon>(
            std::vector<Vector2*> {
                new Vector2(1.0f, 1.0f),
                new Vector2(1.0f, -1.0f),
                new Vector2(-1.0f,-1.0f),
                new Vector2(-1.0f,1.0f)
            },
            Colour{200,0,0,255}
        ),
        0.0f
    );
    box1.restitution = 1.0f;
    world.AddBody(&box1);

    Rigidbody box2(
        100.0f,
        Transform{Vector2(25.0f, 0.0f), 0.0f},
        std::make_unique<Polygon>(
            std::vector<Vector2*> {
                new Vector2(5.0f, 5.0f),
                new Vector2(5.0f, -5.0f),
                new Vector2(-5.0f,-5.0f),
                new Vector2(-5.0f,5.0f)
            },
            Colour{0,200,0,255}
        ),
        0.0f
    );
    box2.restitution = 1.0f;
    world.AddBody(&box2);

    box2.ApplyImpulse(Vector2(-150.0f, 0.0f));

    while (running){
        auto deltatime = clock::now() - timestart;
        timestart = clock::now();

        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltatime);

        if (HandleEvents()){
            // Quit
            running = false;
        }

        while (lag >= timestep){
            frameCount++;
            frameCount %= 60;

            float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(timestep).count() / 1000.0f;

            world.Step(seconds);

            renderer.Clear();
            renderer.DrawWorld(world);
            renderer.Present();

            lag -= timestep;
        }

        // calculate how close or far we are from the next timestep (for future interpolation)
        // auto alpha = (float) lag.count() / timestep.count();
    }

    renderer.Quit();

    return 0;
}