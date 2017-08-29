//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_PHYSICSMANAGER_HPP
#define DROIDBLASTER_PHYSICSMANAGER_HPP

#include "GraphicsManager.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

struct PhysicsBody {
    PhysicsBody(Location *pLocation, int32_t pWidth, int32_t pHeight):
            location(pLocation),
            width(pWidth), height(pHeight),
            velocityX(0.0f), velocityY(0.0.f) {}

    Location *location;
    int32_t width;
    int32_t height;
    float velocityX;
    float velocityY;
};

class PhysicsManager {
public:
    PhysicsManager(TimeManager& pTimeManager, GraphicsManager& pGraphicsManager);
    ~PhysicsManager();

    PhysicsBody* loadBody(Location& pLocation, int32_t pWidth, int32_t pHeight);

    void update();

private:
    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;
    PhysicsBody* mPhysicsBodies[1024];
    int32_t mPhysicsBodyCount;
};


#endif //DROIDBLASTER_PHYSICSMANAGER_HPP
