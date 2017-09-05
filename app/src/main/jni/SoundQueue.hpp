//
// Created by Brovko Roman on 04.09.17.
//

#ifndef DROIDBLASTER_SOUNDQUEUE_HPP
#define DROIDBLASTER_SOUNDQUEUE_HPP

#include "Sound.hpp"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundQueue {
public:
    SoundQueue();

    status initialize(SLEngineItf pEngine, SLObjectItf pOutputMixObj);
    void finalize();
    void playSound(Sound* pSound);

private:
    SLObjectItf mPlayerObj;
    SLPlayItf mPlayer;
    SLBufferQueueItf mPlayerQueue;
};

#endif //DROIDBLASTER_SOUNDQUEUE_HPP
