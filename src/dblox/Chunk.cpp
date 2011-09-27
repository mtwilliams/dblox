#include "Shared/Helpers.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Utils/Random.h"

#include "dblox/Textures.h"
#include "dblox/Blocks.h"

#include "dblox/Chunk.h"

#include <math.h>

using namespace Graphics::VertexFormats;

#define BLOCK_FACE_LEFT   1
#define BLOCK_FACE_RIGHT  2
#define BLOCK_FACE_FRONT  3
#define BLOCK_FACE_BACK   4
#define BLOCK_FACE_TOP    5
#define BLOCK_FACE_BOTTOM 6

#define BLOCK_AT( x, z, y ) \
	(m_Blocks[x][z][y])

Chunk::Chunk( int X, int Y, unsigned int Seed )
	: X(X)
	, Y(Y)
	, m_pVBO(0)
{
	for( unsigned int x = 0; x < Chunk::Width; ++x ) {
	for( unsigned int z = 0; z < Chunk::Width; ++z ) {
	for( unsigned int y = 0; y < Chunk::Height; ++y ) {
		if( y < Chunk::Height / 2 - 1 )
		{
			if( x == 0 )
			{
				BLOCK_AT(x, z, y) = 2;
			}
			else if( x == Chunk::Width - 1 )
			{
				BLOCK_AT(x, z, y) = 4;
			}
			else
			{
				BLOCK_AT(x, z, y) = (Utils::Random::Integer() % 32) > 0 ? 0 : 1;
			}
		}
		else if( y == Chunk::Height / 2 - 1 )
		{
			BLOCK_AT(x, z, y) = 18;
		}
		else
		{
			BLOCK_AT(x, z, y) = 0;
		}
	}}}

	Rebuild();
}

Chunk::Chunk( const std::string& Filename )
{
}

Chunk::~Chunk()
{
	SAFE_DELETE(m_pVBO);
}

void Chunk::Draw( void )
{
	Textures::GetTexture("Terrain")->Bind(Graphics::TextureTargets::Texture0);
	m_pVBO->Draw(Graphics::Primitives::Triangles, XYZNUV, m_VertexCount);
}

void Chunk::Update( float DeltaTime )
{
}

void Chunk::Rebuild( void )
{
	unsigned int Visible = CalcNumVisibleFaces();

	m_VertexCount = 0;
	VertexXYZNUV* Vertices = new VertexXYZNUV[Visible * 6];

	for( unsigned int x = 0; x < Chunk::Width; ++x ) {
	for( unsigned int z = 0; z < Chunk::Width; ++z ) {
	for( unsigned int y = 0; y < Chunk::Height; ++y ) {

		if( !Blocks::IsBlockTranslucent(BLOCK_AT(x, z, y)) )
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
				GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_FRONT);
			}
			else  if( z == Chunk::Width - 1 )
			{
				GenerateFaceForBlock(x, z, y, Vertices, BLOCK_FACE_BACK);
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

		if( x > 0 && !Blocks::IsBlockTranslucent(BLOCK_AT(x - 1, z, y)) )
		{
			GenerateFaceForBlock(x - 1, z, y, Vertices, BLOCK_FACE_RIGHT);
		}
		
		if( x < Chunk::Width - 1 && !Blocks::IsBlockTranslucent(BLOCK_AT(x + 1, z, y)) )
		{
			GenerateFaceForBlock(x + 1, z, y, Vertices, BLOCK_FACE_LEFT);
		}

		if( z > 0 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z - 1, y)) )
		{
			GenerateFaceForBlock(x, z - 1, y, Vertices, BLOCK_FACE_BACK);
		}

		if( z < Chunk::Width - 1 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z + 1, y)) )
		{
			GenerateFaceForBlock(x, z + 1, y, Vertices, BLOCK_FACE_FRONT);
		}

		if( y > 0 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z, y - 1)) )
		{
			GenerateFaceForBlock(x, z, y - 1, Vertices, BLOCK_FACE_TOP);
		}

		if( y < Chunk::Width - 1 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z, y + 1)) )
		{
			GenerateFaceForBlock(x, z, y + 1, Vertices, BLOCK_FACE_BOTTOM);
		}
	}}}

	SAFE_DELETE(m_pVBO);

	m_pVBO = new Graphics::VertexBuffer(Visible * 6, Graphics::VertexBuffer::Dynamic, Graphics::VertexBuffer::Writeable);
	m_pVBO->Write(Vertices, m_VertexCount * sizeof(VertexXYZNUV));

	delete[] Vertices;
}

unsigned int Chunk::CalcNumVisibleFaces( void )
{
	unsigned int Visible = 0;

	for( unsigned int x = 0; x < Chunk::Width; ++x ) {
	for( unsigned int z = 0; z < Chunk::Width; ++z ) {
	for( unsigned int y = 0; y < Chunk::Height; ++y ) {

		if( !Blocks::IsBlockTranslucent(BLOCK_AT(x, z, y)) )
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

		if( x > 0 && !Blocks::IsBlockTranslucent(BLOCK_AT(x - 1, z, y)) )
		{
			Visible++;
		}

		if( x < Chunk::Width - 1 && !Blocks::IsBlockTranslucent(BLOCK_AT(x + 1, z, y)) )
		{
			Visible++;
		}

		if( z > 0 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z - 1, y)) )
		{
			Visible++;
		}

		if( z < Chunk::Width - 1 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z + 1, y)) )
		{
			Visible++;
		}

		if( y > 0 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z, y - 1)) )
		{
			Visible++;
		}

		if( y < Chunk::Width - 1 && !Blocks::IsBlockTranslucent(BLOCK_AT(x, z, y + 1)) )
		{
			Visible++;
		}

	}}}

	return Visible;
}

void Chunk::GenerateFaceForBlock( unsigned int x, unsigned int z, unsigned y, VertexXYZNUV* Vertices, unsigned int Face )
{
	Graphics::Texture* Terrain = Textures::GetTexture("Terrain");

	unsigned int Block = BLOCK_AT(x, z, y);
	float U1 = (Block % 16) * 16;
	float V1 = (Block / 16) * 16;
	float U2 = (Block % 16) * 16 + 16;
	float V2 = (Block / 16) * 16 + 16;

	U1 /= Terrain->GetWidth();
	U2 /= Terrain->GetWidth();
	V1 /= Terrain->GetHeight();
	V2 /= Terrain->GetHeight();

	V1 = 1.0f - V1;
	V2 = 1.0f - V2;

	// The Z axis is flipped in OpenGL vs. Chunks
	z = Chunk::Width - z;
	y = Chunk::Height - y;

	switch( Face )
	{
		case BLOCK_FACE_LEFT:
		{
			// X, Y, Z and U, V
			VertexXYZNUV Vertex;

			Vertex.Normal[0] = -1.0f;
			Vertex.Normal[1] = 0.0f;
			Vertex.Normal[2] = 0.0f;

			// Top-Left
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Left
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Left
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;
		} break;

		case BLOCK_FACE_RIGHT:
		{
			// X, Y, Z and U, V
			VertexXYZNUV Vertex;

			Vertex.Normal[0] = 1.0f;
			Vertex.Normal[1] = 0.0f;
			Vertex.Normal[2] = 0.0f;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Right
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;
		} break;

		case BLOCK_FACE_FRONT:
		{
			// X, Y, Z and U, V
			VertexXYZNUV Vertex;

			Vertex.Normal[0] = 0.0f;
			Vertex.Normal[1] = 0.0f;
			Vertex.Normal[2] = 1.0f;

			// Top-Left
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Left
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Right
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Left
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;
		} break;

		case BLOCK_FACE_BACK:
		{
			// X, Y, Z and U, V
			VertexXYZNUV Vertex;

			Vertex.Normal[0] = 0.0f;
			Vertex.Normal[1] = 0.0f;
			Vertex.Normal[2] = -1.0f;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;
		} break;

		case BLOCK_FACE_TOP:
		{
			// X, Y, Z and U, V
			VertexXYZNUV Vertex;

			Vertex.Normal[0] = 0.0f;
			Vertex.Normal[1] = 1.0f;
			Vertex.Normal[2] = 0.0f;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y - 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;
		} break;

		case BLOCK_FACE_BOTTOM:
		{
			// X, Y, Z and U, V
			VertexXYZNUV Vertex;

			Vertex.Normal[0] = 0.0f;
			Vertex.Normal[1] = 1.0f;
			Vertex.Normal[2] = 0.0f;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Bottom-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z - 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V2;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Right
			Vertex.Position[0] = x - 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U2;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;

			// Top-Left
			Vertex.Position[0] = x + 0.5f;
			Vertex.Position[1] = y + 0.5f;
			Vertex.Position[2] = z + 0.5f;

			Vertex.UV[0] = U1;
			Vertex.UV[1] = V1;

			Vertices[m_VertexCount] = Vertex;
			m_VertexCount++;
		} break;
	}
}
