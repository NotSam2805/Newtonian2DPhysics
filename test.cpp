#include "Physics/Physics.h"
#include <iostream>

int main(){
    n2p::PhysicsWorld world;
    n2p::Rigidbody body;

    world.AddBody(&body);
    body.AddForce(n2p::Vector2(1.0f, 1.0f));

    // Should show (0,0)
    std::cout << body.GetPosition() << "\n";

    // Step forward 1 second
    world.Step(1.0f);

    // Should show (1,1)
    std::cout << body.GetPosition() << "\n";

    return 0;
}