#include "Shared/Helpers.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "dblox/Blocks/Blocks.h"
#include "dblox/World.h"
#include "dblox/Chunk.h"

#include <math.h>

using namespace Graphics::VertexFormats;

#define BLOCK_TEXTURES 32

Chunk::Chunk( int X, int Z, World* World )
    : X(X)
    , Z(Z)
    , m_pWorld(World)
    , m_pVBO(0)
{
}

Chunk::Chunk( const std::string& Filename )
{
}

Chunk::~Chunk()
{
    SAFE_DELETE(m_pVBO);
}

void Chunk::Update( float DeltaTime )
{
}

void Chunk::Draw( void )
{
    if( m_pVBO )
        m_pVBO->Draw(Graphics::Primitives::Triangles, XYZNUVW, m_VertexCount);
}

void Chunk::Rebuild( void )
{
    unsigned int Visible = CalcNumVisibleFaces();

    m_VertexCount = 0;
    VertexXYZNUVW* Vertices = new VertexXYZNUVW[Visible * 6];

    for( unsigned int x = 0; x < Chunk::Width; ++x ) {
    for( unsigned int z = 0; z < Chunk::Width; ++z ) {
    for( unsigned int y = 0; y < Chunk::Height; ++y ) {

        if( !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            if( x == 0 )
            {
                GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_LEFT);
                //GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_RIGHT);
            }
            else if( x == Chunk::Width - 1 )
            {
                GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_RIGHT);
                //GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_LEFT);
            }

            if( z == 0 )
            {
                GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_BACK);
            }
            else  if( z == Chunk::Width - 1 )
            {
                GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_FRONT);
            }

            if( y == 0 )
            {
                GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_BOTTOM);
            }
            else if( y == Chunk::Height - 1 )
            {
                GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_TOP);
            }

            continue;
        }

        if( x > 0 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x - 1, z, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            GenerateFaceForBlock(x - 1, z, y, Vertices, BLOCK_FACE_RIGHT);
        }

        if( x < Chunk::Width - 1 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x + 1, z, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            GenerateFaceForBlock(x + 1, z, y, Vertices, BLOCK_FACE_LEFT);
        }

        if( z > 0 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z - 1, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            GenerateFaceForBlock(x, z - 1, y, Vertices, BLOCK_FACE_FRONT);
        }

        if( z < Chunk::Width - 1 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z + 1, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            GenerateFaceForBlock(x, z + 1, y, Vertices, BLOCK_FACE_BACK);
        }

        if( y > 0 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y - 1))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            GenerateFaceForBlock(x, z, y - 1, Vertices, BLOCK_FACE_TOP);
        }

        if( y < Chunk::Width - 1 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y + 1))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            GenerateFaceForBlock(x, z, y + 1, Vertices, BLOCK_FACE_BOTTOM);
        }
    }}}

    SAFE_DELETE(m_pVBO);

    m_pVBO = new Graphics::VertexBuffer(Visible * 6, Graphics::VertexBuffer::Dynamic, Graphics::VertexBuffer::Writeable);
    m_pVBO->Write(Vertices, m_VertexCount * sizeof(VertexXYZNUVW));

    delete[] Vertices;
}

int Chunk::GetX( void )
{
    return X;
}

int Chunk::GetZ( void )
{
    return Z;
}

unsigned int Chunk::CalcNumVisibleFaces( void )
{
    unsigned int Visible = 0;

    for( unsigned int x = 0; x < Chunk::Width; ++x ) {
    for( unsigned int z = 0; z < Chunk::Width; ++z ) {
    for( unsigned int y = 0; y < Chunk::Height; ++y ) {

        if( !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            if( x == 0 )
            {
                Visible++;
            }
            else if( x == Chunk::Width - 1 )
            {
                Visible++;
            }

            if( z == 0 )
            {
                Visible++;
            }
            else  if( z == Chunk::Width - 1 )
            {
                Visible++;
            }

            if( y == 0 )
            {
                Visible++;
            }
            else if( y == Chunk::Height - 1 )
            {
                Visible++;
            }

            continue;
        }

        if( x > 0 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x - 1, z, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            Visible++;
        }

        if( x < Chunk::Width - 1 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x + 1, z, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            Visible++;
        }

        if( z > 0 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z - 1, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            Visible++;
        }

        if( z < Chunk::Width - 1 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z + 1, y))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            Visible++;
        }

        if( y > 0 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y - 1))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            Visible++;
        }

        if( y < Chunk::Width - 1 && !(BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y + 1))->GetFlags() & BLOCK_FLAG_TRANSLUCENT) )
        {
            Visible++;
        }
    }}}

    return Visible;
}

void Chunk::GenerateFaceForBlock( unsigned int x, unsigned int z, unsigned y, VertexXYZNUVW* Vertices, unsigned int Face )
{
    float U1 = 0.0f;
    float V1 = 0.0f;
    float U2 = 1.0f;
    float V2 = 1.0f;
    float W = BLOCK_TEXTURES - BlockManager::GetBlock((Blocks::Block)CHUNK_BLOCK_AT(x, z, y))->GetTextureOnFace(Face);

    // The Y axis is flipped in OpenGL vs. Chunks
    y = Chunk::Height - y;

    x += X * Chunk::Width;
    z += Z * Chunk::Width;

    switch( Face )
    {
    case BLOCK_FACE_LEFT:
        {
            // X, Y, Z and U, V
            VertexXYZNUVW Vertex;
            Vertex.UVW[2] = W;

            Vertex.Normal[0] = -1.0f;
            Vertex.Normal[1] = 0.0f;
            Vertex.Normal[2] = 0.0f;

            // Top-Left
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Left
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Left
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;
        } break;

    case BLOCK_FACE_RIGHT:
        {
            // X, Y, Z and U, V
            VertexXYZNUVW Vertex;
            Vertex.UVW[2] = W;

            Vertex.Normal[0] = 1.0f;
            Vertex.Normal[1] = 0.0f;
            Vertex.Normal[2] = 0.0f;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Right
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;
        } break;

    case BLOCK_FACE_FRONT:
        {
            // X, Y, Z and U, V
            VertexXYZNUVW Vertex;
            Vertex.UVW[2] = W;

            Vertex.Normal[0] = 0.0f;
            Vertex.Normal[1] = 0.0f;
            Vertex.Normal[2] = 1.0f;

            // Top-Left
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Left
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Right
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Left
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;
        } break;

    case BLOCK_FACE_BACK:
        {
            // X, Y, Z and U, V
            VertexXYZNUVW Vertex;
            Vertex.UVW[2] = W;

            Vertex.Normal[0] = 0.0f;
            Vertex.Normal[1] = 0.0f;
            Vertex.Normal[2] = -1.0f;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;
        } break;

    case BLOCK_FACE_TOP:
        {
            // X, Y, Z and U, V
            VertexXYZNUVW Vertex;
            Vertex.UVW[2] = W;

            Vertex.Normal[0] = 0.0f;
            Vertex.Normal[1] = 1.0f;
            Vertex.Normal[2] = 0.0f;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y - 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;
        } break;

    case BLOCK_FACE_BOTTOM:
        {
            // X, Y, Z and U, V
            VertexXYZNUVW Vertex;
            Vertex.UVW[2] = W;

            Vertex.Normal[0] = 0.0f;
            Vertex.Normal[1] = 1.0f;
            Vertex.Normal[2] = 0.0f;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Bottom-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z - 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V2;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Right
            Vertex.Position[0] = x - 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U2;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;

            // Top-Left
            Vertex.Position[0] = x + 0.5f;
            Vertex.Position[1] = y + 0.5f;
            Vertex.Position[2] = z + 0.5f;

            Vertex.UVW[0] = U1;
            Vertex.UVW[1] = V1;

            Vertices[m_VertexCount] = Vertex;
            m_VertexCount++;
        } break;
    }
}
