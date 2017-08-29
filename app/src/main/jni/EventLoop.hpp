//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_EVENTLOOP_HPP
#define DROIDBLASTER_EVENTLOOP_HPP

#include <android_native_app_glue.h>

#include "ActivityHandler.hpp"

class EventLoop {
public:
    EventLoop(android_app *pApplication, ActivityHandler &pActivityHandler);
    void run();

private:
    void activate();
    void deactivate();

    void processAppEvent(int32_t pCommand);

    static void callback_appEvent(android_app *pApplication, int32_t pCommand);

    android_app *mApplication;
    bool mEnabled;
    bool mQuit;

    ActivityHandler &mActivityHandler;
};

#endif //DROIDBLASTER_EVENTLOOP_HPP
