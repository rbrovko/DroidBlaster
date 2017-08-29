//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_TIMEMANAGER_HPP
#define DROIDBLASTER_TIMEMANAGER_HPP

#include "Types.hpp"
#include <ctime>

class TimeManager {
public:
    TimeManager();

    void reset();
    void update();

    double now();
    float elapsed() { return mElapsed; };
    float elapsedTotal() { return mElapsedTotal; };

private:
    double mFirstTime;
    double mLastTime;
    float mElapsed;
    float mElapsedTotal;
};


#endif //DROIDBLASTER_TIMEMANAGER_HPP
