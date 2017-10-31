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

#include <vector>
#include <map>

class GraphicsComponent {
public:
    virtual status load() = 0;
    virtual void draw() = 0;
};

struct TextureProperties {
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
    int32_t getScreenWidth() {
        return mScreenWidth;
    }
    int32_t getScreenHeight() {
        return mScreenHeight;
    };

    GLfloat* getProjectionMatrix() {
        return mProjectionMatrix[0];
    }

    void registerComponent(GraphicsComponent *pComponent);

    status start();
    void stop();
    status update();

    TextureProperties* loadTexture(Resource& pResource);
    GLuint loadShader(const char* pVertexShader, const char* pFragmentShader);
    GLuint loadVertexBuffer(const void* pVertexBuffer, int32_t pVertexBufferSize);

private:
    status initializeRenderBuffer();

    struct RenderVertex {
        GLfloat x, y, u, v;
    };

    android_app *mApplication;

    int32_t mRenderWidth, mRenderHeight;
    int32_t mScreenWidth, mScreenHeight;

    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;

    GLfloat mProjectionMatrix[4][4];

    // Graphics resources
    std::map<Resource*, TextureProperties> mTextures;
    std::vector<GLuint> mShaders;
    std::vector<GLuint> mVertexBuffers;
    std::vector<GraphicsComponent*> mComponents;

    // Rendering resources
    GLint mScreenFrameBuffer;
    GLuint mRenderFrameBuffer, mRenderVertexBuffer;
    GLuint mRenderTexture, mRenderShaderProgram;
    GLuint aPosition, aTexture;
    GLuint uProjection, uTexture;
};

#endif //DROIDBLASTER_GRAPHICSMANAGER_HPP
