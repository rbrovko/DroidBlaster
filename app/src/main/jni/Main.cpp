//
// Created by Brovko Roman on 29.08.17.
//

#include "EventLoop.hpp"
#include "Log.hpp"

void android_main(android_app *pApplication) {
    EventLoop(pApplication).run();
}
