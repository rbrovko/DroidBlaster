//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_EVENTLOOP_HPP
#define DROIDBLASTER_EVENTLOOP_HPP

#include <android_native_app_glue.h>

class EventLoop {
public:
    EventLoop(android_app *pApplication);
    void run();

private:
    android_app *mApplication;
};

#endif //DROIDBLASTER_EVENTLOOP_HPP
