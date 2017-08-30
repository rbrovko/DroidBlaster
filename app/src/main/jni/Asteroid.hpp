//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_ASTEROID_HPP
#define DROIDBLASTER_ASTEROID_HPP

#include "GraphicsManager.hpp"
#include "PhysicsManager.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

class Asteroid {
public:
    Asteroid(android_app *pApplication,
             TimeManager& pTimeManager,
             GraphicsManager& pGraphicsManager,
             PhysicsManager& pPhysicsManager);

    void registerAsteroid(Location& pLocation, int32_t pSizeX, int32_t pSizeY);

    void initialize();
    void update();

private:
    void spawn(PhysicsBody *pBody);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsManager& mPhysicsManager;

    PhysicsBody* mBodies[1024];
    int32_t mBodyCount;
    float mMinBound;
    float mUpperBound;
    float mLowerBound;
    float mLeftBound;
    float mRightBound;
};


#endif //DROIDBLASTER_ASTEROID_HPP
