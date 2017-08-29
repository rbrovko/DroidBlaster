//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_ACTIVITYHANDLER_HPP
#define DROIDBLASTER_ACTIVITYHANDLER_HPP

#include "Types.hpp"

class ActivityHandler {
public:
    virtual ~ActivityHandler() {};

    virtual status onActivate() = 0;
    virtual void onDeactivate() = 0;
    virtual status onStep() = 0;

    virtual void onStart() {};
    virtual void onResume() {};
    virtual void onPause() {};
    virtual void onStop() {};
    virtual void onDestroy() {};

    virtual void onSaveInstanceState(void** pData, size_t *pSize) {};
    virtual void onConfigurationChanged() {};
    virtual void onLowMemory() {};

    virtual void onCreateWindow() {};
    virtual void onDestroyWindow() {};
    virtual void onGainFocus() {};
    virtual void onLostFocus() {};
};

#endif //DROIDBLASTER_ACTIVITYHANDLER_HPP
