//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_SHIP_HPP
#define DROIDBLASTER_SHIP_HPP

#include "GraphicsManager.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"
#include "Sound.hpp"

class Ship {
public:
    Ship(android_app *pApplication, GraphicsManager& pGraphicsManager, SoundManager& pSoundManager);

    void registerShip(Sprite* pGraphics, Sound* pCollisionSound);

    void initialize();

private:
    GraphicsManager& mGraphicsManager;
    SoundManager& mSoundManager;

    Sprite* mGraphics;
    Sound* mCollisionSound;
};

#endif //DROIDBLASTER_SHIP_HPP
