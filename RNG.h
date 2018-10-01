//
// Created by Rafiq on 2018-09-30.
//

#ifndef RENDERER_RNG_H
#define RENDERER_RNG_H

#include <random>

static const float FloatOneMinusEpsilon = 0x1.fffffep-1f;

class RNG {
public:
    std::mt19937 generator;
    RNG() = default;
    explicit RNG(int seed){
        generator = std::mt19937(seed);
    }

    inline float UniformFloat(float min, float max){
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

    inline float UniformInt(int min, int max){
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

};

extern thread_local RNG gen;

#endif //RENDERER_RNG_H
