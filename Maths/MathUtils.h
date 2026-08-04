namespace n2p{
    float Clamp(float value, float min, float max){
        value = (value < min) ? min : value;
        return (value > max) ? max : value;
    }

    float Lerp(float a, float b, float t){
        return a + ((b-a) * t);
    }
}
