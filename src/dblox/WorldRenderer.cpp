#include "dblox/WorldRenderer.h"

WorldRenderer::WorldRenderer( void )
    : m_pTerrainTexture(0)
    , m_pChunkShader(0)
    , m_pTestChunk(0)
{
    m_pTestChunk = new Chunk(0, 0, 0);
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
    // TODO: Handle order / transparency
    m_pTerrainTexture->Bind(Graphics::TextureTargets::Texture0);
    m_pChunkShader->Bind();

    m_pTestChunk->Draw();

    m_pChunkShader->Unbind();
    m_pTerrainTexture->Unbind();
}