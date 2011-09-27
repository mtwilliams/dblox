#ifndef GRAPHICS_INDEXBUFFER_H
#define GRAPHICS_INDEXBUFFER_H

#include "Graphics/Exports.h"

namespace Graphics
{
	struct IndexBufferImpl_t;

	class DBLOX_GRAPHICS_EXPORT IndexBuffer
	{
		public:
			enum Type
			{
				Byte = 0,
				Short,
				Int32
			};

			enum Mode
			{
				Writeable = 0,
				Invalid
			};
		public:
			IndexBuffer( unsigned int Size, IndexBuffer::Type Type, IndexBuffer::Mode Mode );
			~IndexBuffer();

			void Bind( void );
			void Unbind( void );

			void Write( void* Data, unsigned int Length );

			unsigned int GetSize( void );
			IndexBuffer::Type GetType( void );

		protected:
			IndexBufferImpl_t* Impl;
	};
}

#endif