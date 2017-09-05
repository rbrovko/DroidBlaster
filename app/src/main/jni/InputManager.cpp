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

bool InputManager::onKeyboardEvent(AInputEvent *pEvent) {
    static const float ORTHOGONAL_MOVE = 1.0f;

    if (AKeyEvent_getAction(pEvent) == AKEY_EVENT_ACTION_DOWN) {
        switch (AKeyEvent_getKeyCode(pEvent)) {
            case AKEYCODE_DPAD_LEFT:
                mDirectionX = -ORTHOGONAL_MOVE;
                return true;

            case AKEYCODE_DPAD_RIGHT:
                mDirectionX = ORTHOGONAL_MOVE;
                return true;

            case AKEYCODE_DPAD_DOWN:
                mDirectionY = -ORTHOGONAL_MOVE;
                return true;

            case AKEYCODE_DPAD_UP:
                mDirectionY = ORTHOGONAL_MOVE;
                return true;
        }
    } else {
        switch (AKeyEvent_getKeyCode(pEvent)) {
            case AKEYCODE_DPAD_LEFT:
            case AKEYCODE_DPAD_RIGHT:
                mDirectionX = 0.0f;
                return true;

            case AKEYCODE_DPAD_DOWN:
            case AKEYCODE_DPAD_UP:
                mDirectionY = 0.0f;
                return true;
        }
    }

    return false;
}

bool InputManager::onTrackballEvent(AInputEvent *pEvent) {
    static const float ORTHOGONAL_MOVE = 1.0f;
    static const float DIAGONAL_MOVE = 0.707f;
    static const float THRESHOLD = (1 / 100.0f);

    if (AMotionEvent_getAction(pEvent) == AMOTION_EVENT_ACTION_MOVE) {
        float directionX = AMotionEvent_getX(pEvent, 0);
        float directionY = AMotionEvent_getY(pEvent, 0);
        float horizontal, vertical;

        if (directionX < -THRESHOLD) {
            if (directionY < -THRESHOLD) {
                horizontal = -DIAGONAL_MOVE;
                vertical = DIAGONAL_MOVE;
            } else if (directionY > THRESHOLD) {
                horizontal = -DIAGONAL_MOVE;
                vertical = -DIAGONAL_MOVE;
            } else {
                horizontal = -ORTHOGONAL_MOVE;
                vertical = 0.0f;
            }
        } else if (directionX > THRESHOLD) {
            if (directionY < -THRESHOLD) {
                horizontal = DIAGONAL_MOVE;
                vertical = DIAGONAL_MOVE;
            } else if (directionY > THRESHOLD) {
                horizontal = DIAGONAL_MOVE;
                vertical = -DIAGONAL_MOVE;
            } else {
                horizontal = ORTHOGONAL_MOVE;
                vertical = 0.0f;
            }
        } else if (directionY < -THRESHOLD) {
            horizontal = 0.0f;
            vertical = ORTHOGONAL_MOVE;
        } else if (directionY > THRESHOLD) {
            horizontal = 0.0f;
            vertical = -ORTHOGONAL_MOVE;
        }

        // Ends movement if there is a counter movement
        if ((horizontal < 0.0f) && (mDirectionX > 0.0f)) {
            mDirectionX = 0.0f;
        } else if ((horizontal > 0.0f) && (mDirectionX < 0.0f)) {
            mDirectionX = 0.0f;
        } else {
            mDirectionX = horizontal;
        }

        if ((vertical < 0.0f) && (mDirectionY > 0.0f)) {
            mDirectionY = 0.0f;
        } else if ((vertical > 0.0f) && (mDirectionY < 0.0f)) {
            mDirectionY = 0.0f;
        } else {
            mDirectionY = vertical;
        }
    } else {
        mDirectionX = 0.0f;
        mDirectionY = 0.0f;
    }

    return true;
}

bool InputManager::onAccelerometerEvent(ASensorEvent *pEvent) {

}