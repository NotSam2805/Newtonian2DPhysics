#pragma once
#include "Contact.hpp"
#include <vector>

namespace n2p{
    class CollisionSolver{
        public:
            // Avoid correcting small movements to reduce jitter
            static constexpr float slop = 0.01f;
            static constexpr float correctionPercent = 0.8f;

            static constexpr unsigned int iterations = 20;

            static void CorrectPosition(const Manifold& manifold);
            static void FindTargetVelocities(Manifold& manifold);
            static void SolveNormal(Manifold& manifold);
            static void SolveTangent(Manifold& manifold);
            static void SolveCollisions(std::vector<Manifold> manifolds);
    };
}