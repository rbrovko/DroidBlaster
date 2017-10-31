//
// Created by Brovko Roman on 29.08.17.
//

#include "Ship.hpp"
#include "Log.hpp"
#include "Types.hpp"

static const float INITIAL_X = 0.5f;
static const float INITIAL_Y = 0.25f;
static const int32_t DEFAULT_LIVES = 10;

static const int32_t SHIP_DESTROY_FRAME_1 = 8;
static const int32_t SHIP_DESTROY_FRAME_COUNT = 9;
static const float SHIP_DESTROY_ANIM_SPEED = 12.0f;

Ship::Ship(android_app *pApplication, GraphicsManager &pGraphicsManager,
           SoundManager &pSoundManager) :
        mGraphicsManager(pGraphicsManager),
        mSoundManager(pSoundManager),
        mGraphics(NULL), mCollisionSound(NULL),
        mBody(NULL),
        mDestroyed(false), mLives(0) {}

void Ship::registerShip(Sprite *pGraphics, Sound *pCollisionSound, b2Body *pBody) {
    mGraphics = pGraphics;
    mCollisionSound = pCollisionSound;
    mBody = pBody;
}

void Ship::initialize() {
    mDestroyed = false;
    mLives = DEFAULT_LIVES;

    b2Vec2 position(
            mGraphicsManager.getRenderWidth() * INITIAL_X / PHYSICS_SCALE,
            mGraphicsManager.getRenderHeight() * INITIAL_Y / PHYSICS_SCALE
    );

    mBody->SetTransform(position, 0.0f);
    mBody->SetActive(true);
}

void Ship::update() {
    if (mLives >= 0) {
        if (((PhysicsCollision *)mBody->GetUserData())->collide) {
            mSoundManager.playSound(mCollisionSound);
            --mLives;
            if (mLives < 0) {
                Log::info("Ship has been destroyed");
                mGraphics->setAnimation(SHIP_DESTROY_FRAME_1,
                                        SHIP_DESTROY_FRAME_COUNT, SHIP_DESTROY_ANIM_SPEED, false);
                mBody->SetActive(false);
            } else {
                Log::info("Ship collided");
            }
        }
    } else {
        // Destroyed
        if (mGraphics->animationEnded()) {
            mDestroyed = true;
        }
    }
}