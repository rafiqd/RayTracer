//
// Created by Rafiq on 2018-09-09.
//

#ifndef RENDERER_DISTRIBUTIONS_H
#define RENDERER_DISTRIBUTIONS_H

#include "Randomizer.h"

class Distributions {
public:
    static Randomizer zero_to_one;
    static Randomizer none_to_pone;
private:
    Distributions() = default;
};


#endif //RENDERER_DISTRIBUTIONS_H
