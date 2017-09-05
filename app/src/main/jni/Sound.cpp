//
// Created by Brovko Roman on 04.09.17.
//

#include "Sound.hpp"
#import "Log.hpp"

#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"

Sound::Sound(android_app *pApplication, Resource *pResource) :
        mResource(pResource),
        mBuffer(NULL), mLength(0) {

}

const char* Sound::getPath() {
    return mResource->getPath();
}

status Sound::load() {
    Log::info("Loading sound %s", mResource->getPath());
    status result;

    if (mResource->open() != STATUS_OK) {
        goto ERROR;
    }

    // Reads sound file
    mLength = mResource->getLength();
    mBuffer = new uint8_t[mLength];
    result = mResource->read(mBuffer, mLength);
    mResource->close();

    return STATUS_OK;

    ERROR:
    Log::error("Error while reading PCM sound");

    return STATUS_KO;
}

status Sound::unload() {
    delete[] mBuffer;
    mBuffer = NULL;
    mLength = 0;

    return STATUS_OK;
}
