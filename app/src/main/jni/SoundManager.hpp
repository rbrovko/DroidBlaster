//
// Created by Brovko Roman on 04.09.17.
//

#ifndef DROIDBLASTER_SOUNDMANAGER_HPP
#define DROIDBLASTER_SOUNDMANAGER_HPP

#include "Types.hpp"
#include "Resource.hpp"
#include "Sound.hpp"
#include "SoundQueue.hpp"

#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundManager {
public:
    SoundManager(android_app* pApplication);
    ~SoundManager();

    status start();
    void stop();

    status playBGM(Resource& pResource);
    void stopBGM();

    Sound* registerSound(Resource& pResource);
    void playSound(Sound *pSound);

    void recordSound();
    void playRecordSound();

private:
    android_app* mApplication;

    SLObjectItf mEngineObj;
    SLEngineItf mEngine;
    SLObjectItf mOutputMixObj;

    SLObjectItf mBGMPlayerObj;
    SLPlayItf mBGMPlayer;
    SLSeekItf mBGMPlayerSeek;

    static const int32_t QUEUE_COUNT = 5;
    SoundQueue mSoundQueues[QUEUE_COUNT];
    int32_t mCurrentQueue;

    // sound
    Sound* mSounds[32];
    int32_t mSoundCount;

    // sound record
    SLObjectItf mRecorderObj;
    SLRecordItf mRecorder;
    SLAndroidSimpleBufferQueueItf mRecorderQueue;
    // record buffer
    Sound mRecorderSound;

    status startSoundRecorder();
    static void callback_recorder(SLAndroidSimpleBufferQueueItf pQueue, void* pContext);
};


#endif //DROIDBLASTER_SOUNDMANAGER_HPP
