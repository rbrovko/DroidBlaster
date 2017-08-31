//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_GRAPHICSMANAGER_HPP
#define DROIDBLASTER_GRAPHICSMANAGER_HPP

#include "Types.hpp"
#include "Resource.hpp"

#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

struct GraphicsElement {
    GraphicsElement(int32_t pWidth, int32_t pHeight) :
            location(),
            width(pWidth),
            height(pHeight) {}

    Location location;
    int32_t width;
    int32_t height;
};

struct TextureProperties {
    Resource *textureResource;
    GLuint  texture;
    int32_t width;
    int32_t height;
};

class GraphicsManager {
public:
    GraphicsManager(android_app *pApplication);
    ~GraphicsManager();

    int32_t getRenderWidth() {
        return mRenderWidth;
    }
    int32_t getRenderHeight() {
        return mRenderHeight;
    }

    GraphicsElement* registerElement(int32_t pHeight, int32_t pWidth);

    status start();
    void stop();
    status update();

    TextureProperties* loadTexture(Resource& pResource);

private:
    android_app *mApplication;

    int32_t mRenderWidth;
    int32_t mRenderHeight;

    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;

    GraphicsElement* mElements[1024];
    int32_t mElementCount;

    TextureProperties mTextures[32];
    int32_t mTextureCount;
};

#endif //DROIDBLASTER_GRAPHICSMANAGER_HPP
