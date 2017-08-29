//
// Created by Brovko Roman on 29.08.17.
//

#include "DroidBlaster.hpp"
#include "EventLoop.hpp"
#include "Log.hpp"

void android_main(android_app *pApplication) {
    DroidBlaster(pApplication).run();
}
