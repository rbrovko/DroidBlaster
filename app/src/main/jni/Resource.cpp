//
// Created by Brovko Roman on 30.08.17.
//

#include "Resource.hpp"

#include <sys/stat.h>

Resource::Resource(android_app *pApplication, const char *pPath) :
        mPath(pPath),
        mAssetManager(pApplication->activity->assetManager),
        mAsset(NULL) {}

status Resource::open() {
    mAsset = AAssetManager_open(mAssetManager, mPath, AASSET_MODE_UNKNOWN);

    return (mAsset != NULL) ? STATUS_OK : STATUS_KO;
}

void Resource::close() {
    if (mAsset != NULL) {
        AAsset_close(mAsset);
        mAsset = NULL;
    }
}

status Resource::read(void *pBuffer, size_t pCount) {
    int32_t readCount = AAsset_read(mAsset, pBuffer, pCount);

    return (readCount == pCount) ? STATUS_OK : STATUS_KO;
}

ResourceDescription Resource::description() {
    ResourceDescription lDescription = {-1, 0, 0};
    AAsset *lAsset = AAssetManager_open(mAssetManager, mPath, AASSET_MODE_UNKNOWN);

    if (lAsset != NULL) {
        lDescription.mDescription = AAsset_openFileDescriptor(lAsset, &lDescription.mStart, &lDescription.mLength);
        AAsset_close(lAsset);
    }

    return lDescription;
}

off_t Resource::getLength() {
    return AAsset_getLength(mAsset);
}

bool Resource::operator==(const Resource &pOther) {
    return !strcmp(mPath, pOther.mPath);
}