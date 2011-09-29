#include "Shared/Helpers.h"
#include "Shared/Platform.h"
#include "Graphics/VertexBuffer.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>
#endif

#include <stddef.h>

namespace Graphics
{
	struct VertexBufferImpl_t
	{
		// OpenGL
		unsigned int ID;

		// Agnostic
		VertexBuffer::Type Type;
		VertexBuffer::Mode Mode;
		unsigned int Size;
	};

	VertexBuffer::VertexBuffer( unsigned int Size, VertexBuffer::Type Type, VertexBuffer::Mode Mode )
	{
		Impl = new VertexBufferImpl_t();
		Impl->Type = Type;
		Impl->Mode = Mode;
		Impl->Size = Size;

		glGenBuffers(1, &Impl->ID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &Impl->ID);
		SAFE_DELETE(Impl);
	}

	void VertexBuffer::Bind( void )
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, Impl->ID);
	}

	void VertexBuffer::Unbind( void )
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void VertexBuffer::Write( void* Data, unsigned int Length )
	{
		unsigned int GlMode = 0;

		switch( Impl->Mode )
		{
			case VertexBuffer::Writeable: GlMode = GL_DYNAMIC_DRAW_ARB; break;
		}

		glBindBuffer(GL_ARRAY_BUFFER, Impl->ID);
		glBufferData(GL_ARRAY_BUFFER, Length, Data, GlMode);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	unsigned int TypeToGL( Primitives::Type Type )
	{
		unsigned int GlType = 0;
		switch( Type )
		{
			case Primitives::Lines: GlType = GL_LINES; break;
			case Primitives::LineStrip: GlType = GL_LINE_STRIP; break;
			case Primitives::Triangles: GlType = GL_TRIANGLES; break;
			case Primitives::TriangleStrip: GlType = GL_TRIANGLE_STRIP; break;
			case Primitives::TriangleFan: GlType = GL_TRIANGLE_FAN; break;
		}

		return GlType;
	}

	unsigned int TypeToGL( IndexBuffer::Type Type )
	{
		unsigned int GlIndexType = 0;
		switch( Type )
		{
			case IndexBuffer::Byte: GlIndexType = GL_UNSIGNED_BYTE; break;
			case IndexBuffer::Short: GlIndexType = GL_UNSIGNED_SHORT; break;
			case IndexBuffer::Int32: GlIndexType = GL_UNSIGNED_INT; break;
		}

		return GlIndexType;
	}

	void SetupPointers( unsigned int VBO, VertexFormats::Format Format )
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		switch( Format )
		{
			case VertexFormats::XYZ:
			{
				glVertexPointer(3, GL_FLOAT, 0, 0);
			} break;

			case VertexFormats::XYZUV:
			{
				glVertexPointer(3, GL_FLOAT, sizeof(VertexFormats::VertexXYZUV), (void*)offsetof(VertexFormats::VertexXYZUV, Position));
				
				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(VertexFormats::VertexXYZUV), (void*)offsetof(VertexFormats::VertexXYZUV, UV));
			} break;

			case VertexFormats::XYZCUV:
			{
				glVertexPointer(3, GL_FLOAT, sizeof(VertexFormats::VertexXYZCUV), (void*)offsetof(VertexFormats::VertexXYZCUV, Position));
				
				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(VertexFormats::VertexXYZCUV), (void*)offsetof(VertexFormats::VertexXYZCUV, UV));

				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(VertexFormats::VertexXYZCUV), (void*)offsetof(VertexFormats::VertexXYZCUV, Color));
			} break;

			case VertexFormats::XYZNUV:
			{
				glVertexPointer(3, GL_FLOAT, sizeof(VertexFormats::VertexXYZNUV), (void*)offsetof(VertexFormats::VertexXYZNUV, Position));
				
				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(VertexFormats::VertexXYZNUV), (void*)offsetof(VertexFormats::VertexXYZNUV, UV));

				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, sizeof(VertexFormats::VertexXYZNUV), (void*)offsetof(VertexFormats::VertexXYZNUV, Normal));
			} break;

            case VertexFormats::XYZNUVW:
            {
                glVertexPointer(3, GL_FLOAT, sizeof(VertexFormats::VertexXYZNUVW), (void*)offsetof(VertexFormats::VertexXYZNUVW, Position));
				
				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(3, GL_FLOAT, sizeof(VertexFormats::VertexXYZNUVW), (void*)offsetof(VertexFormats::VertexXYZNUVW, UVW));

				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, sizeof(VertexFormats::VertexXYZNUVW), (void*)offsetof(VertexFormats::VertexXYZNUVW, Normal));
            } break;
		}
	}

	void VertexBuffer::Draw( Primitives::Type Type, VertexFormats::Format Format, unsigned int Count )
	{
		SetupPointers(Impl->ID, Format);
		glDrawArrays(TypeToGL(Type), 0, Count);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void VertexBuffer::Draw( Primitives::Type Type, VertexFormats::Format Format, IndexBuffer* Indicies, unsigned int Count )
	{
		SetupPointers(Impl->ID, Format);

		Indicies->Bind();
		glDrawElements(TypeToGL(Type), Count, TypeToGL(Indicies->GetType()), 0);
		Indicies->Unbind();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	unsigned int VertexBuffer::GetSize( void )
	{
		return Impl->Size;
	}
}
