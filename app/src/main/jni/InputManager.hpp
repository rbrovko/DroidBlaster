//
// Created by Brovko Roman on 05.09.17.
//

#ifndef DROIDBLASTER_INPUTMANAGER_HPP
#define DROIDBLASTER_INPUTMANAGER_HPP

#include "GraphicsManager.hpp"
#include "InputHandler.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>

class InputManager : public InputHandler {
public:
    InputManager(android_app *pApplication, GraphicsManager& pGraphicsManager);

    float getDirectionX() { return mDirectionX; };
    float getDirectionY() { return mDirectionY; };
    void setRefPoint(Location *pRefPoint) { mRefPoint = pRefPoint; };

    void start();

protected:
    bool onTouchEvent(AInputEvent *pEvent);
    bool onKeyboardEvent(AInputEvent *pEvent);
    bool onTrackballEvent(AInputEvent *pEvent);
    bool onAccelerometerEvent(ASensorEvent *pEvent);

private:
    android_app *mApplication;
    GraphicsManager& mGraphicsManager;

    // input values
    float mScaleFactor;
    float mDirectionX, mDirectionY;
    // Reference point to evaluate distance
    Location *mRefPoint;
};


#endif //DROIDBLASTER_INPUTMANAGER_HPP
