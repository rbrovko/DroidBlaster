//
// Created by Brovko Roman on 04.09.17.
//

#ifndef DROIDBLASTER_SOUNDMANAGER_HPP
#define DROIDBLASTER_SOUNDMANAGER_HPP

#include "Types.hpp"
#include "Resource.hpp"

#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundManager {
public:
    SoundManager(android_app* pApplication);

    status start();
    void stop();

    status playBGM(Resource& pResource);
    void stopBGM();

private:
    android_app* mApplication;

    SLObjectItf mEngineObj;
    SLEngineItf mEngine;
    SLObjectItf mOutputMixObj;

    SLObjectItf mBGMPlayerObj;
    SLPlayItf mBGMPlayer;
    SLSeekItf mBGMPlayerSeek;
};


#endif //DROIDBLASTER_SOUNDMANAGER_HPP
