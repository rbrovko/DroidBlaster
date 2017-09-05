//
// Created by Brovko Roman on 05.09.17.
//

#ifndef DROIDBLASTER_MOVEABLEBODY_HPP
#define DROIDBLASTER_MOVEABLEBODY_HPP

#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "Types.hpp"

class MoveableBody {
public:
    MoveableBody(android_app *pApplication,
                 InputManager& pInputManager, PhysicsManager& pPhysicsManager);

    PhysicsBody* registerMoveableBody(Location& pLocation, int32_t pSizeX, int32_t pSizeY);

    void initialize();
    void update();

private:
    PhysicsManager& mPhysicsManager;
    InputManager& mInputManager;

    PhysicsBody* mBody;
};


#endif //DROIDBLASTER_MOVEABLEBODY_HPP

