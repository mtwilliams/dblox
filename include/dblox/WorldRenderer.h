#ifndef DBLOX_WORLD_RENDERER_H
#define DBLOX_WORLD_RENDERER_H

#include "Graphics/Shader.h"
#include "Graphics/TextureArray.h"

#include "dblox/Chunk.h"

#include <list>

class WorldRenderer
{
    public:
        WorldRenderer( void );
        ~WorldRenderer();

        bool Initialize( void );

        void Update( float DeltaTime );
        void Render( void );

        void AddChunkToRenderQueue( Chunk* Chunk );

    protected:
        Graphics::TextureArray* m_pTerrainTexture;
        Graphics::Shader* m_pChunkShader;

        std::list<Chunk*> m_ChunkRenderQueue;
};

#endif
