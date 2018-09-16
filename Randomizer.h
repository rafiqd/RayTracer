//
// Created by Rafiq on 2018-09-09.
//

#ifndef RENDERER_RANDOMIZER_H
#define RENDERER_RANDOMIZER_H

#include <random>
#include <ctime>


class Randomizer {
public:
    Randomizer(int min, int max, int seed = int(std::time(nullptr)));
    float getRandFloat();

private:
    int seed;
    std::default_random_engine gen;
    std::uniform_real_distribution<float> randFloat;

};

#endif //RENDERER_RANDOMIZER_H
