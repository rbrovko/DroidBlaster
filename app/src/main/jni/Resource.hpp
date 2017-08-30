//
// Created by Brovko Roman on 30.08.17.
//

#ifndef DROIDBLASTER_RESOURCE_HPP
#define DROIDBLASTER_RESOURCE_HPP

#include "Types.hpp"

#include <android_native_app_glue.h>

class Resource {
public:
    Resource(android_app *pApplication, const char *pPath);

    const char* getPath() { return mPath; };

    status open();
    void close();
    status read(void* pBuffer, size_t pCount);

    bool operator==(const Resource& pOther);

private:
    const char* mPath;
    AAssetManager *mAssetManager;
    AAsset *mAsset;
};


#endif //DROIDBLASTER_RESOURCE_HPP
