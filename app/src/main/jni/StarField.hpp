//
// Created by Brovko Roman on 04.09.17.
//

#ifndef DROIDBLASTER_STARFIELD_HPP
#define DROIDBLASTER_STARFIELD_HPP

#include "GraphicsManager.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

#include <GLES2/gl2.h>

class StarField : public GraphicsComponent {
public:
    StarField(android_app *pApplication, TimeManager& pTimeManager,
              GraphicsManager& pGraphicsManager, int32_t pStarCount, Resource& pTextureResource);

    status load();
    void draw();

private:
    struct Vertex {
        GLfloat x, y, z;
    };

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;

    int32_t mStarCount;
    Resource& mTextureResource;

    GLuint mVertexBuffer, mTexture, mShaderProgram;
    GLuint aPosition, uProjection;
    GLuint uTime, uHeight, uTexture;
};


#endif //DROIDBLASTER_STARFIELD_HPP
