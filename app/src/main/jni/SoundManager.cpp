//
// Created by Brovko Roman on 04.09.17.
//

#include "SoundManager.hpp"
#include "Log.hpp"
#include "Resource.hpp"

SoundManager::SoundManager(android_app *pApplication) :
        mApplication(pApplication),
        mEngine(NULL), mEngineObj(NULL),
        mOutputMixObj(NULL),
        mBGMPlayerObj(NULL), mBGMPlayer(NULL), mBGMPlayerSeek(NULL),
        mSoundQueues(), mCurrentQueue(0),
        mSounds(), mSoundCount(0),
        mRecorderObj(NULL), mRecorderQueue(NULL),
        mRecorderSound(pApplication, 2 * 44100 * sizeof(int16_t)) {
    Log::info("Creating SoundManager");
}

SoundManager::~SoundManager() {
    Log::info("Destroying SoundManager");
    for (int32_t i = 0; i < mSoundCount; ++i) {
        delete mSounds[i];
    }
    mSoundCount = 0;
}

status SoundManager::start() {
    Log::info("Starting SoundManager");
    SLresult result;
    const SLuint32 engineMixIIDCount = 1;
    const SLInterfaceID engineMixIIDs[] = {SL_IID_ENGINE};
    const SLboolean engineMixReqs[] = {SL_BOOLEAN_TRUE};
    const SLuint32 outputMixIIDCount = 0;
    const SLInterfaceID outputMixIIDs[] = {};
    const SLboolean outputMixReqs[] = {};

    // Creates OpenSL ES engine and dumps its capabilities
    result = slCreateEngine(&mEngineObj, 0, NULL, engineMixIIDCount, engineMixIIDs, engineMixReqs);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mEngineObj)->Realize(mEngineObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE, &mEngine);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    // Creates audio output
    result = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj, outputMixIIDCount, outputMixIIDs, outputMixReqs);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mOutputMixObj)->Realize(mOutputMixObj, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    // Setup sound player
    Log::info("Starting sound player");
    for (int32_t i = 0; i < QUEUE_COUNT; ++i) {
        if (mSoundQueues[i].initialize(mEngine, mOutputMixObj) != STATUS_OK) {
            goto ERROR;
        }
    }

    // TODO: without record
//    if (startSoundRecorder() != STATUS_OK) {
//        goto ERROR;
//    }

    // Loads resources
    for (int32_t i = 0; i < mSoundCount; ++i) {
        if (mSounds[i]->load() != STATUS_OK) {
            goto ERROR;
        }
    }

    // TODO: without record
//    mRecorderSound.load();

    return STATUS_OK;

    ERROR:
    Log::error("Error while starting SoundManager");
    stop();

    return STATUS_KO;
}

void SoundManager::stop() {
    Log::info("Stopping SoundManager");

    // Stops and destroys BGM player
    stopBGM();

    // Destroys sound player
    for (int32_t i = 0; i < QUEUE_COUNT; ++i) {
        mSoundQueues[i].finalize();
    }

    // Destroys sound player
    if (mRecorderObj != NULL) {
        (*mRecorderObj)->Destroy(mRecorderObj);
        mRecorderObj = NULL;
        mRecorder = NULL;
        mRecorderQueue = NULL;
    }

    // Destroys audio output and engine
    if (mOutputMixObj != NULL) {
        (*mOutputMixObj)->Destroy(mOutputMixObj);
        mOutputMixObj = NULL;
    }

    if (mEngineObj != NULL) {
        (*mEngineObj)->Destroy(mEngineObj);
        mEngineObj = NULL;
        mEngine = NULL;
    }

    // Frees sound resources
    for (int32_t i = 0; i < mSoundCount; ++i) {
        mSounds[i]->unload();
    }
}

status SoundManager::playBGM(Resource &pResource) {
    SLresult result;
    Log::info("Opening BGM %s", pResource.getPath());

    // setup BGM audio source
    ResourceDescription descriptor = pResource.description();
    if (descriptor.mDescription < 0) {
        Log::info("Could not open BGM file");
        return STATUS_KO;
    }

    SLDataLocator_AndroidFD dataLocatorIn;
    dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
    dataLocatorIn.fd = descriptor.mDescription;
    dataLocatorIn.offset = descriptor.mStart;
    dataLocatorIn.length = descriptor.mLength;

    SLDataFormat_MIME dataFormat;
    dataFormat.formatType = SL_DATAFORMAT_MIME;
    dataFormat.mimeType = NULL;
    dataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

    SLDataSource dataSource;
    dataSource.pLocator = &dataLocatorIn;
    dataSource.pFormat = &dataFormat;

    SLDataLocator_OutputMix dataLocatorOut;
    dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    dataLocatorOut.outputMix = mOutputMixObj;

    SLDataSink dataSink;
    dataSink.pLocator = &dataLocatorOut;
    dataSink.pFormat = NULL;

    // Creates BGM player and retrieves its interfaces
    const SLuint32 bgmPlayerIIDCount = 2;
    const SLInterfaceID bgmPlayerIIDs[] = {SL_IID_PLAY, SL_IID_SEEK};
    const SLboolean bgmPlayerReqs[] = {SL_BOOLEAN_FALSE, SL_BOOLEAN_TRUE};

    result = (*mEngine)->CreateAudioPlayer(mEngine, &mBGMPlayerObj, &dataSource, &dataSink,
                                           bgmPlayerIIDCount, bgmPlayerIIDs, bgmPlayerReqs);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mBGMPlayerObj)->Realize(mBGMPlayerObj, SL_BOOLEAN_FALSE);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj, SL_IID_PLAY, &mBGMPlayer);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj, SL_IID_SEEK, &mBGMPlayerSeek);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    // Enables looping and starts playing
    result = (*mBGMPlayerSeek)->SetLoop(mBGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mBGMPlayer)->SetPlayState(mBGMPlayer, SL_PLAYSTATE_PLAYING);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    return STATUS_OK;

    ERROR:
    Log::error("Error playing BGM");

    return STATUS_KO;
}

void SoundManager::stopBGM() {
    if (mBGMPlayer != NULL) {
        SLuint32 bgmPlayerState;
        (*mBGMPlayerObj)->GetState(mBGMPlayerObj, &bgmPlayerState);

        if (bgmPlayerState == SL_OBJECT_STATE_REALIZED) {
            (*mBGMPlayer)->SetPlayState(mBGMPlayer, SL_PLAYSTATE_PAUSED);

            (*mBGMPlayerObj)->Destroy(mBGMPlayerObj);
            mBGMPlayerObj = NULL;
            mBGMPlayer = NULL;
            mBGMPlayerSeek = NULL;
        }
    }
}

Sound* SoundManager::registerSound(Resource &pResource) {
    for (int32_t i = 0; i < mSoundCount; ++i) {
        if (strcmp(pResource.getPath(), mSounds[i]->getPath()) == 0) {
            return mSounds[i];
        }
    }

    Sound* sound = new Sound(mApplication, &pResource);
    mSounds[mSoundCount++] = sound;

    return sound;
}

void SoundManager::playSound(Sound *pSound) {
    int32_t currentQueue = ++mCurrentQueue;
    SoundQueue& soundQueue = mSoundQueues[currentQueue % (QUEUE_COUNT - 1)];
    soundQueue.playSound(pSound);
}

status SoundManager::startSoundRecorder() {
    Log::info("Starting sound recorder");
    SLresult result;

    // Setup sound audio source
    SLDataLocator_AndroidSimpleBufferQueue dataLocatorOut;
    dataLocatorOut.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    dataLocatorOut.numBuffers = 1;

    SLDataFormat_PCM dataFormat;
    dataFormat.formatType = SL_DATAFORMAT_PCM;
    dataFormat.numChannels = 1; // mono sound
    dataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
    dataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
    dataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
    dataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

    SLDataSink dataSink;
    dataSink.pLocator = &dataLocatorOut;
    dataSink.pFormat = &dataFormat;

    SLDataLocator_IODevice dataLocatorIn;
    dataLocatorIn.locatorType = SL_DATALOCATOR_IODEVICE;
    dataLocatorIn.deviceType = SL_IODEVICE_AUDIOINPUT;
    dataLocatorIn.deviceID = SL_DEFAULTDEVICEID_AUDIOINPUT;
    dataLocatorIn.device = NULL;

    SLDataSource dataSource;
    dataSource.pLocator = &dataLocatorIn;
    dataSource.pFormat = NULL;

    // Creates the sounds recorder and retrieves its interfaces
    const SLuint32 lSoundRecorderIIDCount = 2;
    const SLInterfaceID lSoundRecorderIIDs[] = {SL_IID_RECORD, SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean lSoundRecorderReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    result = (*mEngine)->CreateAudioRecorder(mEngine,
                                             &mRecorderObj, &dataSource, &dataSink,
                                             lSoundRecorderIIDCount, lSoundRecorderIIDs, lSoundRecorderReqs);
    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mRecorderObj)->Realize(mRecorderObj, SL_BOOLEAN_FALSE);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mRecorderObj)->GetInterface(mRecorderObj, SL_IID_RECORD, &mRecorder);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mRecorderObj)->GetInterface(mRecorderObj, SL_IID_ANDROIDBUFFERQUEUESOURCE, &mRecorderQueue);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    // Registers a record callback
    result = (*mRecorderQueue)->RegisterCallback(mRecorderQueue, callback_recorder, this);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    result = (*mRecorder)->SetCallbackEventsMask(mRecorder, SL_RECORDEVENT_BUFFER_FULL);

    if (result != SL_RESULT_SUCCESS) {
        goto ERROR;
    }

    return STATUS_OK;

    ERROR:
    Log::error("Error while starting recorder");

    return STATUS_KO;
}

void SoundManager::recordSound() {
    SLresult result;
    SLuint32 recorderState;

    (*mRecorderObj)->GetState(mRecorderObj, &recorderState);
    if (recorderState == SL_OBJECT_STATE_REALIZED) {
        // Stops any current recording
        result = (*mRecorder)->SetRecordState(mRecorder, SL_RECORDSTATE_STOPPED);

        if (result != SL_RESULT_SUCCESS) {
            goto ERROR;
        }

        result = (*mRecorderQueue)->Clear(mRecorderQueue);

        if (result != SL_RESULT_SUCCESS) {
            goto ERROR;
        }

        // Provides a buffer for recording
        result = (*mRecorderQueue)->Enqueue(mRecorderQueue,
                                            mRecorderSound.getBuffer(), mRecorderSound.getLength());

        if (result != SL_RESULT_SUCCESS) {
            goto ERROR;
        }

        // Starts recording
        result = (*mRecorder)->SetRecordState(mRecorder, SL_RECORDSTATE_RECORDING);

        if (result != SL_RESULT_SUCCESS) {
            goto ERROR;
        }

    }

    return;

    ERROR:
    Log::error("Error trying to record sound");
}

void SoundManager::playRecordSound() {
    SLuint32 recorderState;

    (*mRecorderObj)->GetState(mRecorderObj, &recorderState);

    if (recorderState == SL_OBJECT_STATE_REALIZED) {
        SoundQueue& soundQueue = mSoundQueues[QUEUE_COUNT - 1];
        soundQueue.playSound(&mRecorderSound);
    }
}

void SoundManager::callback_recorder(SLAndroidSimpleBufferQueueItf pQueue, void *pContext) {
    SLresult result;
    SoundManager& manager = *(SoundManager*)pContext;

    Log::info("Ended recording sound");

    result = (*(manager.mRecorder))->SetRecordState(manager.mRecorder, SL_RECORDSTATE_STOPPED);

    if (result == SL_RESULT_SUCCESS) {
        manager.playRecordSound();
    } else {
        Log::warn("Could not stop record queue");
    }
}