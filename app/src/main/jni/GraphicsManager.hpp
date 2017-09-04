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

class GraphicsComponent {
public:
    virtual status load() = 0;
    virtual void draw() = 0;
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
    GLfloat* getProjectionMatrix() {
        return mProjectionMatrix[0];
    }

    void registerComponent(GraphicsComponent *pComponent);

    status start();
    void stop();
    status update();

    TextureProperties* loadTexture(Resource& pResource);
    GLuint loadShader(const char* pVertexShader, const char* pFragmentShader);

private:
    struct RenderVertex {
        GLfloat x, y, u, v;
    };

    android_app *mApplication;

    int32_t mRenderWidth;
    int32_t mRenderHeight;

    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;

    GLfloat mProjectionMatrix[4][4];

    TextureProperties mTextures[32];
    int32_t mTextureCount;

    GLuint mShaders[32];
    int32_t mShaderCount;

    GraphicsComponent* mComponents[32];
    int32_t mComponentCount;
};

#endif //DROIDBLASTER_GRAPHICSMANAGER_HPP
