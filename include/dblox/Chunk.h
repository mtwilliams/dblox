#ifndef DBLOX_CHUNK_H
#define DBLOX_CHUNK_H

#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

#include "dblox/Blocks/Blocks.h"

#include <string>

#define CHUNK_BLOCK_AT( x, z, y ) \
    (Blocks[x][z][y])

class World;
class Chunk
{
	public:
		static const unsigned int Width = 16;
		static const unsigned int Height = 32;

	public:
		Chunk( int X, int Z, World* World );
		Chunk( const std::string& Filename );

		~Chunk();

        void Update( float DeltaTime );
		void Draw( void );

		void Rebuild( void );

        int GetX( void );
        int GetZ( void );

        Blocks::Block Blocks[Chunk::Width][Chunk::Width][Chunk::Height];

	protected:
		int X, Z;
        World* m_pWorld;

		Graphics::VertexBuffer* m_pVBO;
		unsigned int m_VertexCount;

	protected:
		unsigned int CalcNumVisibleFaces( void );
		void GenerateFaceForBlock( unsigned int x, unsigned z,  unsigned int y, Graphics::VertexFormats::VertexXYZNUVW* Vertices, unsigned int Face );
};

#endif
