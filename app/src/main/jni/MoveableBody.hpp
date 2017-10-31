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

    b2Body* registerMoveableBody(Location& pLocation, int32_t pSizeX, int32_t pSizeY);

    void initialize();
    void update();

private:
    PhysicsManager& mPhysicsManager;
    InputManager& mInputManager;

    b2Body* mBody;
    b2MouseJoint* mTarget;
};


#endif //DROIDBLASTER_MOVEABLEBODY_HPP

