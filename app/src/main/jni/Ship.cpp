//
// Created by Brovko Roman on 29.08.17.
//

#include "Ship.hpp"
#include "Log.hpp"
#include "Types.hpp"

static const float INITIAL_X = 0.5f;
static const float INITIAL_Y = 0.25f;

Ship::Ship(android_app *pApplication, GraphicsManager &pGraphicsManager,
           SoundManager &pSoundManager) :
        mGraphicsManager(pGraphicsManager),
        mSoundManager(pSoundManager),
        mGraphics(NULL), mCollisionSound(NULL) {}

void Ship::registerShip(Sprite *pGraphics, Sound *pCollisionSound) {
    mGraphics = pGraphics;
    mCollisionSound = pCollisionSound;
}

void Ship::initialize() {
    mGraphics->location.x = INITIAL_X * mGraphicsManager.getRenderWidth();
    mGraphics->location.y = INITIAL_Y * mGraphicsManager.getRenderHeight();
    mSoundManager.playSound(mCollisionSound);
}