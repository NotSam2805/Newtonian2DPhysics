#pragma once
#include "../Maths/Maths.hpp"

namespace n2p{
    class Camera{
        private:
            Vector2 position;
            float zoom;
        public:
            Camera();
            ~Camera();

            Vector2 WorldToScreen(Vector2 world);
    };
}