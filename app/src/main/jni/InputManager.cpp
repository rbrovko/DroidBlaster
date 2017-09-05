//
// Created by Brovko Roman on 05.09.17.
//

#include "InputManager.hpp"
#include "Log.hpp"

#include <android_native_app_glue.h>
#include <cmath>

InputManager::InputManager(android_app *pApplication, GraphicsManager &pGraphicsManager) :
        mApplication(pApplication), mGraphicsManager(pGraphicsManager),
        mDirectionX(0.0f), mDirectionY(0.0f),
        mRefPoint(NULL) {

}

void InputManager::start() {
    Log::info("Starting InputManager");
    mDirectionX = 0.0f;
    mDirectionY = 0.0f;
    mScaleFactor = float(mGraphicsManager.getRenderWidth()) / float(mGraphicsManager.getRenderHeight());
}

bool InputManager::onTouchEvent(AInputEvent *pEvent) {
    static const float TOUCH_MAX_RANGE = 65.0f; // in game units

    if (mRefPoint != NULL) {
        if (AMotionEvent_getAction(pEvent) == AMOTION_EVENT_ACTION_MOVE) {
            float x = AMotionEvent_getX(pEvent, 0) * mScaleFactor;
            float y = (float(mGraphicsManager.getScreenHeight())
                       - AMotionEvent_getY(pEvent, 0)) * mScaleFactor;
            /*
             * Needs a conversion to proper coordinate
             * (origin at bottom/left) only moveY need it
             */
            float moveX = x - mRefPoint->x;
            float moveY = y - mRefPoint->y;
            float moveRange = sqrt((moveX * moveX) + (moveY * moveY));

            if (moveRange > TOUCH_MAX_RANGE) {
                float cropFactor = TOUCH_MAX_RANGE / moveRange;
                moveX *= cropFactor;
                moveY *= cropFactor;
            }

            mDirectionX = moveX / TOUCH_MAX_RANGE;
            mDirectionY = moveY / TOUCH_MAX_RANGE;
        } else {
            mDirectionX = 0.0f;
            mDirectionY = 0.0f;
        }
    }

    return true;
}
