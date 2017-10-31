//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_SHIP_HPP
#define DROIDBLASTER_SHIP_HPP

#include "GraphicsManager.hpp"
#include "PhysicsManager.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"
#include "Sound.hpp"

class Ship {
public:
    Ship(android_app *pApplication, GraphicsManager& pGraphicsManager, SoundManager& pSoundManager);

    void registerShip(Sprite* pGraphics, Sound* pCollisionSound, b2Body* pBody);

    void initialize();
    void update();
    bool  isDestroyed() { return mDestroyed;}

private:
    GraphicsManager& mGraphicsManager;
    SoundManager& mSoundManager;

    Sprite* mGraphics;
    Sound* mCollisionSound;
    b2Body *mBody;

    bool mDestroyed;
    int32_t mLives;
};

#endif //DROIDBLASTER_SHIP_HPP
