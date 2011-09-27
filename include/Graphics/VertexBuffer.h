#ifndef GRAPHICS_VERTEXBUFFER_H
#define GRAPHICS_VERTEXBUFFER_H

#include "Graphics/Exports.h"

#include "Graphics/Primitive.h"
#include "Graphics/IndexBuffer.h"

namespace Graphics
{
	namespace VertexFormats
	{
		enum Format
		{
			XYZ = 0,
			XYZUV,
			XYZCUV,
			XYZNUV
		};

		typedef struct
		{
			float Position[3];
		} VertexXYZ;

		typedef struct
		{
			float Position[3];
			float UV[2];
		} VertexXYZUV;

		typedef struct
		{
			float Position[3];
			unsigned char Color[4];
			float UV[2];
		} VertexXYZCUV;

		typedef struct
		{
			float Position[3];
			float Normal[3];
			float UV[2];
		} VertexXYZNUV;
	}

	struct VertexBufferImpl_t;
	class DBLOX_GRAPHICS_EXPORT VertexBuffer
	{
		public:
			enum Type
			{
				Static = 0,
				Dynamic
			};

			enum Mode
			{
				Writeable = 0,
				Invalid
			};
		public:
			VertexBuffer( unsigned int Size, VertexBuffer::Type Type, VertexBuffer::Mode Mode );
			~VertexBuffer();

			void Bind( void );
			void Unbind( void );

			void Write( void* Data, unsigned int Length );

			void Draw( Primitives::Type Type, VertexFormats::Format Format, unsigned int Count );
			void Draw( Primitives::Type Type, VertexFormats::Format Format, IndexBuffer* Indicies, unsigned int Count );

			unsigned int GetSize( void );

		protected:
			VertexBufferImpl_t* Impl;
	};
}

#endif