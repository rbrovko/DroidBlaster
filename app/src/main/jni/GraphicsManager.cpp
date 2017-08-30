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
            EGL_CONTEXT_CLIENT_VERSION, 2,
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
    static float clearColor = 0.0f;
    clearColor += 0.001f;
    glClearColor(clearColor, clearColor, clearColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Shows the result to the user
    if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE) {
        Log::error("Error %d swapping buffers", eglGetError());
        return STATUS_KO;
    }

    return STATUS_OK;
}