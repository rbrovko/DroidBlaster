//
// Created by Brovko Roman on 29.08.17.
//

#include "DroidBlaster.hpp"
#include "Log.hpp"
#include "Sound.hpp"

#include <unistd.h>

static const int32_t SHIP_SIZE = 64;
static const int32_t SHIP_FRAME_1 = 0;
static const int32_t SHIP_FRAME_COUNT = 8;
static const float SHIP_ANIM_SPEED = 8.0f;

static const int32_t ASTEROID_COUNT = 16;
static const int32_t ASTEROID_SIZE = 64;
static const int32_t ASTEROID_FRAME_1 = 0;
static const int32_t ASTEROID_FRAME_COUNT = 16;
static const float ASTEROID_MIN_ANIM_SPEED = 8.0f;
static const float ASTEROID_ANIM_SPEED_RANGE = 16.0f;

static const int32_t STAR_COUNT = 50;

DroidBlaster::DroidBlaster(android_app *pApplication):
        mTimeManager(),
        mGraphicsManager(pApplication),
        mPhysicsManager(mTimeManager, mGraphicsManager),
        mSoundManager(pApplication),
        mInputManager(pApplication, mGraphicsManager),
        mEventLoop(pApplication, *this, mInputManager),

        mAsteroidTexture(pApplication, "droidblaster/asteroid.png"),
        mShipTexture(pApplication, "droidblaster/ship.png"),
        mStarTexture(pApplication, "droidblaster/star.png"),
        mBGM(pApplication, "droidblaster/bgm.mp3"),
        mCollisionSound(pApplication, "droidblaster/collision.pcm"),

        mShip(pApplication, mGraphicsManager, mSoundManager),
        mAsteroids(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
        mStarField(pApplication, mTimeManager, mGraphicsManager, STAR_COUNT, mStarTexture),
        mSpriteBatch(mTimeManager, mGraphicsManager),

        mMoveableBody(pApplication, mInputManager, mPhysicsManager) {

    Log::info("Creating DroidBlaster");

    Sprite *shipGraphics = mSpriteBatch.registerSprite(mShipTexture, SHIP_SIZE, SHIP_SIZE);
    shipGraphics->setAnimation(SHIP_FRAME_1, SHIP_FRAME_COUNT, SHIP_ANIM_SPEED, true);
    Sound *collisionSound = mSoundManager.registerSound(mCollisionSound);
    mMoveableBody.registerMoveableBody(shipGraphics->location, SHIP_SIZE, SHIP_SIZE);
    mShip.registerShip(shipGraphics, collisionSound);

    // Creates asteroids
    for (int32_t i = 0; i < ASTEROID_COUNT; ++i) {
        Sprite *asteroidGraphics = mSpriteBatch.registerSprite(mAsteroidTexture, ASTEROID_SIZE, ASTEROID_SIZE);
        float animSpeed = ASTEROID_MIN_ANIM_SPEED + RAND(ASTEROID_ANIM_SPEED_RANGE);
        asteroidGraphics->setAnimation(ASTEROID_FRAME_1, ASTEROID_FRAME_COUNT, animSpeed, true);
        mAsteroids.registerAsteroid(asteroidGraphics->location, ASTEROID_SIZE, ASTEROID_SIZE);
    }
}

void DroidBlaster::run() {
    mEventLoop.run();
}

status DroidBlaster::onActivate() {
    Log::info("Activating DroidBlaster");

    // Starts managers
    if (mGraphicsManager.start() != STATUS_OK) {
        return STATUS_KO;
    }
    if (mSoundManager.start() != STATUS_OK) {
        return STATUS_KO;
    }
    mInputManager.start();

    // Plays music and a sound at startup
    mSoundManager.playBGM(mBGM);
    // TODO: without record
//    mSoundManager.recordSound();

    // Initializes game objects
    mAsteroids.initialize();
    mShip.initialize();
    mMoveableBody.initialize();

    mTimeManager.reset();

    return STATUS_OK;
}

void DroidBlaster::onDeactivate() {
    Log::info("Deactivating DroidBlaster");
    mGraphicsManager.stop();
    mSoundManager.stop();
}

status DroidBlaster::onStep() {
    mTimeManager.update();
    mPhysicsManager.update();

    // updates models
    mAsteroids.update();
    mMoveableBody.update();

    return mGraphicsManager.update();
}

void DroidBlaster::onStart() {
    Log::info("onStart");
}

void DroidBlaster::onResume() {
    Log::info("onResume");
}

void DroidBlaster::onPause() {
    Log::info("onPause");
}

void DroidBlaster::onStop() {
    Log::info("onStop");
}

void DroidBlaster::onDestroy() {
    Log::info("onDestroy");
}

void DroidBlaster::onSaveInstanceState(void **pData, size_t *pSize) {
    Log::info("onSaveInstanceState");
}

void DroidBlaster::onConfigurationChanged() {
    Log::info("onConfigurationChanged");
}

void DroidBlaster::onLowMemory() {
    Log::info("onLowMemory");
}

void DroidBlaster::onCreateWindow() {
    Log::info("onCreateWindow");
}

void DroidBlaster::onDestroyWindow() {
    Log::info("onDestroyWindow");
}

void DroidBlaster::onGainFocus() {
    Log::info("onGainFocus");
}

void DroidBlaster::onLostFocus() {
    Log::info("onLostFocus");
}