#pragma once
#include "Contact.hpp"

namespace n2p{
    class CollisionSolver{
        public:
            // Avoid correcting small movements to reduce jitter
            static constexpr float slop = 0.1f;
            static constexpr float correctionPercent = 1.0f;
            static constexpr float impulseSlop = 0.1f;

            static constexpr unsigned int iterations = 15;

            static void CorrectPosition(const Manifold& manifold);
            static void FindTargetVelocities(Manifold& manifold);
            static void SolveNormal(Manifold& manifold);
            static void SolveTangent(Manifold& manifold);
            static void SolveCollision(Manifold& manifold);
    };
}