#include "Shared/Helpers.h"
#include "Shared/Platform.h"
#include "Graphics/IndexBuffer.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>
#endif

namespace Graphics
{
	struct IndexBufferImpl_t
	{
		// OpenGL
		unsigned int ID;

		// Agnostic
		IndexBuffer::Type Type;
		IndexBuffer::Mode Mode;
		unsigned int Size;
	};

	IndexBuffer::IndexBuffer( unsigned int Size, IndexBuffer::Type Type, IndexBuffer::Mode Mode )
	{
		Impl = new IndexBufferImpl_t();
		Impl->Type = Type;
		Impl->Mode = Mode;
		Impl->Size = Size;

		glGenBuffers(1, &Impl->ID);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &Impl->ID);
		SAFE_DELETE(Impl);
	}

	void IndexBuffer::Bind( void )
	{
		glEnableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Impl->ID);
	}

	void IndexBuffer::Unbind( void )
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_INDEX_ARRAY);
	}

	void IndexBuffer::Write( void* Data, unsigned int Length )
	{
		unsigned int GlMode = 0;

		switch( Impl->Mode )
		{
			case IndexBuffer::Writeable: GlMode = GL_DYNAMIC_DRAW_ARB; break;
		}

		glEnableClientState(GL_INDEX_ARRAY);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Impl->ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Length, Data, GlMode);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableClientState(GL_INDEX_ARRAY);
	}

	unsigned int IndexBuffer::GetSize( void )
	{
		return Impl->Size;
	}

	IndexBuffer::Type IndexBuffer::GetType( void )
	{
		return Impl->Type;
	}
}
