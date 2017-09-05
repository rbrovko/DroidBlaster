//
// Created by Brovko Roman on 04.09.17.
//

#ifndef DROIDBLASTER_SOUND_HPP
#define DROIDBLASTER_SOUND_HPP

#include "Resource.hpp"
#include "Types.hpp"

class Sound {
public:
    Sound(android_app* pApplication, Resource* pResource);
    Sound(android_app* pApplication, int32_t pLength);

    const char* getPath();
    uint8_t* getBuffer() { return mBuffer; }
    off_t getLength() { return mLength; }

    status load();
    status unload();

private:
    friend class SoundManager;

    Resource* mResource;
    uint8_t *mBuffer;
    off_t mLength;
};


#endif //DROIDBLASTER_SOUND_HPP
