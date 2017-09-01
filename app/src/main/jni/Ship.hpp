//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_SHIP_HPP
#define DROIDBLASTER_SHIP_HPP

#include "GraphicsManager.hpp"
#include "Sprite.hpp"

class Ship {
public:
    Ship(android_app *pApplication, GraphicsManager& pGraphicsManager);

    void registerShip(Sprite *pGraphics);

    void initialize();

private:
    GraphicsManager& mGraphicsManager;
    Sprite* mGraphics;
};


#endif //DROIDBLASTER_SHIP_HPP
