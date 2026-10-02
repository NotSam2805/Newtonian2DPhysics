#include <n2p/N2P.hpp>

int main() {
    n2p::PhysicsWorld world{};
    world.Step(0.016f);
}
