//
// Created by Rafiq on 2018-09-09.
//

#include "Randomizer.h"

Randomizer::Randomizer(int min, int max, int s): seed(s), gen(s), randFloat(min, max){}

float Randomizer::getRandFloat() {
    return drand48();//randFloat(gen);
}
#include "Randomizer.h"
