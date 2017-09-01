//
// Created by Brovko Roman on 31.08.17.
//

#ifndef DROIDBLASTER_SPRITEBATCH_HPP
#define DROIDBLASTER_SPRITEBATCH_HPP

#include "GraphicsManager.hpp"
#include "Sprite.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

class SpriteBatch : public GraphicsComponent {
public:
    SpriteBatch(TimeManager& pTimeManager, GraphicsManager& pGraphicsManager);
    ~SpriteBatch();

    Sprite* registerSprite(Resource& pTextureResource, int32_t pHeight, int32_t pWidth);

    status load();
    void draw();

private:
    SpriteBatch(const SpriteBatch&);
    void operator=(const SpriteBatch&);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;

    Sprite* mSprites[1024];
    int32_t mSpriteCount;

    Sprite::Vertex mVertices[1024];
    int32_t mVertexCount;

    GLushort mIndexes[1024];
    int32_t mIndexCount;

    GLuint mShaderProgram;
    GLuint aPosition, aTexture;
    GLuint uProjection, uTexture;
};


#endif //DROIDBLASTER_SPRITEBATCH_HPP
