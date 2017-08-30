//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_TYPES_HPP
#define DROIDBLASTER_TYPES_HPP

#include <cstdlib>

typedef int32_t status;

const status STATUS_OK = 0;
const status STATUS_KO = -1;
const status STATUS_EXIT = -2;

struct Location {
    Location(): x(0.0f), y(0.0f) {};

    float x;
    float y;
};

#define RAND(pMax) (float(pMax) * float(rand()) / float(RAND_MAX))

#endif //DROIDBLASTER_TYPES_HPP
