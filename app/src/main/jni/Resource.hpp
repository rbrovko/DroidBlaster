//
// Created by Brovko Roman on 30.08.17.
//

#ifndef DROIDBLASTER_RESOURCE_HPP
#define DROIDBLASTER_RESOURCE_HPP

#include "Types.hpp"

#include <android_native_app_glue.h>
#include <fstream>
#include <string>

struct ResourceDescription {
    int32_t mDescription;
    off_t mStart;
    off_t mLength;
};

class Resource {
public:
    Resource(android_app *pApplication, const char *pPath);

    const char* getPath() { return mPath.c_str(); };

    status open();
    void close();
    status read(void* pBuffer, size_t pCount);

    off_t getLength();

    bool operator==(const Resource& pOther);

private:
    std::string mPath;
    std::fstream mInputStream;
};


#endif //DROIDBLASTER_RESOURCE_HPP
