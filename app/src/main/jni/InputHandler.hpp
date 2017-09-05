//
// Created by Brovko Roman on 05.09.17.
//

#ifndef DROIDBLASTER_INPUTHANDLER_HPP
#define DROIDBLASTER_INPUTHANDLER_HPP

#include <android/input.h>

class InputHandler {
public:
    virtual ~InputHandler() {};

    virtual bool onTouchEvent(AInputEvent *pEvent) = 0;
    virtual bool onKeyboardEvent(AInputEvent *pEvent) = 0;
    virtual bool onTrackballEvent(AInputEvent *pEvent) = 0;
};


#endif //DROIDBLASTER_INPUTHANDLER_HPP
