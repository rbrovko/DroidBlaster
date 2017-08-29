//
// Created by Brovko Roman on 29.08.17.
//

#include "PhysicsManager.hpp"
#include "Log.hpp"

PhysicsManager::PhysicsManager(TimeManager &pTimeManager, GraphicsManager &pGraphicsManager) :
        mTimeManager(pTimeManager),
        mGraphicsManager(pGraphicsManager),
        mPhysicsBodies(),
        mPhysicsBodyCount(0) {
    Log::info("Creating PhysicsManager");
}

PhysicsManager::~PhysicsManager() {
    Log::info("Destroying PhysicsManager");
    for (int32_t i = 0; i < mPhysicsBodyCount; ++i) {
        delete mPhysicsBodies[i];
    }
}

PhysicsBody* PhysicsManager::loadBody(Location &pLocation, int32_t pWidth, int32_t pHeight) {
    PhysicsBody *body = new PhysicsBody(&pLocation, pWidth, pHeight);
    mPhysicsBodies[mPhysicsBodyCount++] = body;

    return body;
}

void PhysicsManager::update() {
    float timeStep = mTimeManager.elapsed();
    for (int32_t i = 0; i < mPhysicsBodyCount; ++i) {
        PhysicsBody *body = mPhysicsBodies[i];
        body->location->x += (timeStep * body->velocityX);
        body->location->y += (timeStep * body->velocityY);
    }
}