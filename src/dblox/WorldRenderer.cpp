#include "dblox/WorldRenderer.h"

WorldRenderer::WorldRenderer( void )
    : m_pTerrainTexture(0)
    , m_pChunkShader(0)
{
}

WorldRenderer::~WorldRenderer()
{
}

bool WorldRenderer::Initialize( void )
{
    if( !Graphics::TextureArray::LoadFromFile("Textures/Terrain.png", 16, 16, false, &m_pTerrainTexture) )
        return false;

    if( !Graphics::Shader::CreateFromFX("Effects/Chunk.fx", &m_pChunkShader) )
        return false;

    m_pChunkShader->Bind();
    m_pChunkShader->SetUniformSampler("Textures", Graphics::TextureTargets::Texture0);
    m_pChunkShader->Unbind();

    return true;
}

void WorldRenderer::Update( float DeltaTime )
{
}

void WorldRenderer::Render( void )
{
    // Draw chunks.
    m_pTerrainTexture->Bind(Graphics::TextureTargets::Texture0);
    m_pChunkShader->Bind();

    // TODO: Order chunks for translucent blocks (like water)
    for( std::list<Chunk*>::iterator Iter = m_ChunkRenderQueue.begin(); Iter != m_ChunkRenderQueue.end(); ++Iter )
    {
        (*Iter)->Draw();
    }

    m_ChunkRenderQueue.clear();

    m_pChunkShader->Unbind();
    m_pTerrainTexture->Unbind();
}

void WorldRenderer::AddChunkToRenderQueue( Chunk* Chunk )
{
    m_ChunkRenderQueue.push_back(Chunk);
}