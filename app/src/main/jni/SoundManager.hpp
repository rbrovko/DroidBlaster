//
// Created by Brovko Roman on 04.09.17.
//

#ifndef DROIDBLASTER_SOUNDMANAGER_HPP
#define DROIDBLASTER_SOUNDMANAGER_HPP

#include "Types.hpp"

#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>

class SoundManager {
public:
    SoundManager(android_app* pApplication);

    status start();
    void stop();

private:
    android_app* mApplication;

    SLObjectItf mEngineObj;
    SLEngineItf mEngine;
    SLObjectItf mOutputMixObj;
};


#endif //DROIDBLASTER_SOUNDMANAGER_HPP
