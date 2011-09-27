#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Graphics/Texture3D.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>
#endif

#include <stdio.h>

namespace Graphics
{
	struct Texture3DImpl_t
	{
		// General
		Formats::Format Format;
		unsigned int Width;
		unsigned int Height;
		unsigned int Depth;

		// Agnostic
		TextureTargets::Target Bound;

		// OpenGL
		unsigned int ID;
	};

	// Purpose: Constructor
	Texture3D::Texture3D( void )
	{
		Impl = new Texture3DImpl_t();
		Impl->Width = 0;
		Impl->Height = 0;
		Impl->Depth = 0;
		Impl->ID = 0;
		Impl->Bound = TextureTargets::NotBound;
	}

	Texture3D::~Texture3D()
	{
		if( Impl )
		{
			if( Impl->ID )
			{
				glDeleteTextures(1, &Impl->ID);
			}
		}

		SAFE_DELETE(Impl);
	}

	Texture3D::Texture3D( unsigned int Width, unsigned int Height, unsigned int Depth, Formats::Format Format, bool Mipmap )
	{
		Impl = new Texture3DImpl_t();
		Impl->Format = Format;
		Impl->Width = Width;
		Impl->Height = Height;
		Impl->Depth = Depth;
		Impl->ID = 0;
		Impl->Bound = TextureTargets::NotBound;

		unsigned int GlComponents = 0;
		unsigned int GlPixelFormat = 0;
		unsigned int GlType = 0;
		switch( Format )
		{
			case Formats::D24: GlPixelFormat = GL_DEPTH_COMPONENT; GlComponents = GL_DEPTH_COMPONENT; GlType = GL_UNSIGNED_BYTE; break;
			case Formats::G8: GlPixelFormat = GL_LUMINANCE; GlComponents = 1; GlType = GL_UNSIGNED_BYTE; break;
			case Formats::R8G8B8: GlPixelFormat = GL_RGB; GlComponents = 3; GlType = GL_UNSIGNED_BYTE; break;
			case Formats::R8G8B8A8: GlPixelFormat = GL_RGBA; GlComponents = 4; GlType = GL_UNSIGNED_BYTE; break;

			case Formats::R16G16F: GlPixelFormat = GL_RG; GlComponents = GL_RG16F; GlType = GL_FLOAT; break;
			case Formats::R32F: GlPixelFormat = GL_RED; GlComponents = GL_R32F; GlType = GL_FLOAT; break;

			default: Impl->Format = Formats::R8G8B8A8; GlPixelFormat = GL_RGBA; GlComponents = 1; GlType = GL_UNSIGNED_BYTE; break;
		}

		glGenTextures(1, &Impl->ID);
		glBindTexture(GL_TEXTURE_3D, Impl->ID);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, Mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, Mipmap ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		
		glTexImage3D(GL_TEXTURE_3D, 0, GlComponents, Width, Height, Depth, 0, GlPixelFormat, GlType, 0);

		if( Mipmap )
		{
			// ATI bug.
			glEnable(GL_TEXTURE_3D);
			glGenerateMipmap(GL_TEXTURE_3D);
		}

		// Some graphics drivers/cards require textures to be 'flushed'.
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture3D::Bind( TextureTargets::Target Target )
	{
		Impl->Bound = Target;

		switch(Target)
		{
			case TextureTargets::Texture0: glActiveTexture(GL_TEXTURE0); break;
			case TextureTargets::Texture1: glActiveTexture(GL_TEXTURE1); break;
			case TextureTargets::Texture2: glActiveTexture(GL_TEXTURE2); break;
			case TextureTargets::Texture3: glActiveTexture(GL_TEXTURE3); break;
			case TextureTargets::Texture4: glActiveTexture(GL_TEXTURE4); break;
			case TextureTargets::Texture5: glActiveTexture(GL_TEXTURE5); break;
			case TextureTargets::Texture6: glActiveTexture(GL_TEXTURE6); break;
			case TextureTargets::Texture7: glActiveTexture(GL_TEXTURE7); break;
		}

		glEnable(GL_TEXTURE_3D);
		glBindTexture(GL_TEXTURE_3D, Impl->ID);
	}

	void Texture3D::Unbind( void )
	{
		Texture3D::Unbind(Impl->Bound);
		Impl->Bound = TextureTargets::NotBound;
	}

	void Texture3D::Unbind( TextureTargets::Target Target )
	{
		switch( Target )
		{
			case TextureTargets::Texture0: glActiveTexture(GL_TEXTURE0); break;
			case TextureTargets::Texture1: glActiveTexture(GL_TEXTURE1); break;
			case TextureTargets::Texture2: glActiveTexture(GL_TEXTURE2); break;
			case TextureTargets::Texture3: glActiveTexture(GL_TEXTURE3); break;
			case TextureTargets::Texture4: glActiveTexture(GL_TEXTURE4); break;
			case TextureTargets::Texture5: glActiveTexture(GL_TEXTURE5); break;
			case TextureTargets::Texture6: glActiveTexture(GL_TEXTURE6); break;
			case TextureTargets::Texture7: glActiveTexture(GL_TEXTURE7); break;
		}

		glBindTexture(GL_TEXTURE_3D, 0);
	}

	TextureTargets::Target Texture3D::BoundTo( void )
	{
		return Impl->Bound;
	}
			
	unsigned int Texture3D::GetWidth( void )
	{
		return Impl->Width;
	}

	unsigned int Texture3D::GetHeight( void )
	{
		return Impl->Height;
	}
	
	unsigned int Texture3D::GetDepth( void )
	{
		return Impl->Depth;
	}
}
