//
// Created by Brovko Roman on 29.08.17.
//

#include "GraphicsManager.hpp"
#include "Log.hpp"

GraphicsManager::GraphicsManager(android_app *pApplication) :
        mApplication(pApplication),
        mRenderWidth(0), mRenderHeight(0),
        mDisplay(EGL_NO_DISPLAY),
        mSurface(EGL_NO_SURFACE),
        mContext(EGL_NO_CONTEXT),
        mElements(), mElementCount(0) {
    Log::info("Creating GraphicsManager");
}

GraphicsManager::~GraphicsManager() {
    Log::info("Destroying GraphicsManager");
    for (int32_t i = 0; i < mElementCount; ++i) {
        delete mElements[i];
    }
}

GraphicsElement* GraphicsManager::registerElement(int32_t pHeight, int32_t pWidth) {
    mElements[mElementCount] = new GraphicsElement(pHeight, pWidth);

    return mElements[mElementCount++];
}

status GraphicsManager::start() {
    Log::info("Starting GraphicsManager");

    EGLint format, numConfigs, errorResult;
    GLenum status;
    EGLConfig config;

    // Defines display requirements. 16-bits mode here (Red - 5 bits, Green - 6 bits, Blue - 5 bits)
    const EGLint DISPLAY_ATTRIBS[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 5,
            EGL_GREEN_SIZE, 6,
            EGL_RED_SIZE, 5,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };
    // Request an OpenGL ES 2 context
    const EGLint CONTEXT_ATTRIBS[] = {
            EGL_CONTEXT_CLIENT_TYPE, 2,
            EGL_NONE
    };

    // Retrieves a display connection and initializes it
    Log::info("Connecting to the display");
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mDisplay == EGL_NO_DISPLAY) {
        goto ERROR;
    }
    if (!eglInitialize(mDisplay, NULL, NULL)) {
        goto ERROR;
    }

    // Selects the first OpenGL configuration found
    Log::info("Selecting a display config");
    if (!eglChooseConfig(mDisplay, DISPLAY_ATTRIBS, &config, 1, &numConfigs) ||
            (numConfigs <= 0)) {
        goto ERROR;
    }

    // Reconfigures the Android window with the EGL format
    Log::info("Configuration window format");
    if (!eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format)) {
        goto ERROR;
    }
    if (ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0, format) < 0) {
        goto ERROR;
    }

    // Creates the display surface
    Log::info("Initializing the display");
    mSurface = eglCreateWindowSurface(mDisplay, config, mApplication->window, NULL);
    if (mSurface == EGL_NO_SURFACE) {
        goto ERROR;
    }
    mContext = eglCreateContext(mDisplay, config, NULL, CONTEXT_ATTRIBS);
    if (mContext == EGL_NO_CONTEXT) {
        goto ERROR;
    }

    // Activates the display surface
    Log::info("Activating the display");
    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext) ||
            !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mRenderWidth) ||
            !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mRenderHeight) ||
            (mRenderWidth <= 0) || (mRenderHeight <= 0)) {
        goto ERROR;
    }

    // Z-Buffer is useless as we are ordering draw calls ourselves
    glViewport(0, 0, mRenderWidth, mRenderHeight);
    glDisable(GL_DEPTH_TEST);

    // Displays information about OpenGL
    Log::info("Starting GraphicsManager");
    Log::info("Version   : %s", glGetString(GL_VERSION));
    Log::info("Vendor    : %s", glGetString(GL_VENDOR));
    Log::info("Renderer  : %s", glGetString(GL_RENDERER));
    Log::info("Offscreen : %d x %d", mRenderWidth, mRenderHeight);

    return STATUS_OK;

    ERROR:
    Log::error("Error while starting GraphicsManager");
    stop();

    return STATUS_KO;
}

void GraphicsManager::stop() {
    Log::info("Stopping GraphicsManager");

    // Destroys OpenGL context
    if (mDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (mContext != EGL_NO_CONTEXT) {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }

        if (mSurface != EGL_NO_SURFACE) {
            eglDestroySurface(mDisplay, mSurface);
            mSurface = EGL_NO_SURFACE;
        }

        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

status GraphicsManager::update() {
    // Locks the window buffer and draws on it
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(mApplication->window, &windowBuffer, NULL) < 0) {
        Log::error("Error while starting GraphicsManager");

        return STATUS_KO;
    }

    // Clears the window
    memset(windowBuffer.bits, 0, windowBuffer.stride * windowBuffer.height * sizeof(uint32_t *));

    // Renders graphic elements
    int32_t maxX = windowBuffer.width - 1;
    int32_t maxY = windowBuffer.height - 1;
    for (int32_t i = 0; i < mElementCount; ++i) {
        GraphicsElement *element = mElements[i];

        // Computes coordinates
        int32_t leftX = element->location.x - element->width / 2;
        int32_t rightX = element->location.x + element->width / 2;
        int32_t leftY = windowBuffer.height - element->location.y - element->height / 2;
        int32_t rightY = windowBuffer.height - element->location.y + element->height / 2;

        // Clips coordinates
        if (rightX < 0 || leftX > maxX ||
                rightY < 0 || leftY > maxY) {
            continue;
        }

        if (leftX < 0) {
            leftX = 0;
        } else if (rightX > maxX) {
            rightX = maxX;
        }

        if (leftY < 0) {
            leftY = 0;
        } else if (rightY > maxY) {
            rightY = maxY;
        }

        // Draws a rectangle
        uint32_t *line = (uint32_t *)(windowBuffer.bits) + (windowBuffer.stride * leftY);
        for (int iY = leftY; iY <= rightY; iY++) {
            for (int iX = leftX; iX <= rightX; iX++) {
                line[iX] = 0X000000FF; // red color
            }
            line = line + windowBuffer.stride;
        }
    }

    // Finshed drawing
    ANativeWindow_unlockAndPost(mApplication->window);

    return STATUS_OK;
}