#include "MathsUtils.hpp"

namespace n2p{
    inline float Clamp(float value, float min, float max){
        value = (value < min) ? min : value;
        return (value > max) ? max : value;
    }

    inline float Lerp(float a, float b, float t){
        return a + ((b-a) * t);
    }
}