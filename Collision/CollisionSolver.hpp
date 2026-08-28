#pragma once
#include "Contact.hpp"

namespace n2p{
    class CollisionSolver{
        public:
            // Avoid correcting small movements to reduce jitter
            static constexpr float slop = 0.01f;
            static constexpr float correctionPercent = 0.8f;
            static constexpr float impulseSlop = 0.1f;

            static constexpr unsigned int iterations = 10;

            static void CorrectPosition(const Manifold& manifold);
            static void FindTargetVelocities(Manifold& manifold);
            static void SolveNormal(Manifold& manifold);
            static void SolveTangent(Manifold& manifold);
            static void SolveCollision(Manifold& manifold);
    };
}