#ifndef DBLOX_CHUNK_H
#define DBLOX_CHUNK_H

#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

#include "dblox/Blocks/Block.h"

#include <string>

class Chunk
{
	public:
		static const unsigned int Width = 16;
		static const unsigned int Height = 32;

	public:
		Chunk( int X, int Z, unsigned int Seed );
		Chunk( const std::string& Filename );

		~Chunk();

        void Update( float DeltaTime );
		void Draw( void );

		void Rebuild( void );

	protected:
		unsigned int X, Z;
		unsigned char m_Blocks[Chunk::Width][Chunk::Width][Chunk::Height];

		Graphics::VertexBuffer* m_pVBO;
		unsigned int m_VertexCount;

	protected:
		unsigned int CalcNumVisibleFaces( void );
		void GenerateFaceForBlock( unsigned int x, unsigned z,  unsigned int y, Graphics::VertexFormats::VertexXYZNUVW* Vertices, unsigned int Face );
};

#endif
