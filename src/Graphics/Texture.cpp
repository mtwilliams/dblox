#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Graphics/Texture.h"

#include <GL/glew.h>

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
	#include <gl/GL.h>

	// initialization of 'variable' is skipped by 'instruction'
	#pragma warning( disable : 4533 )

	// 'function': was declared deprecated
	#pragma warning( disable : 4996 )
#endif

#include "FreeImage.h"

#include <stdio.h>

namespace Graphics
{
	struct TextureImpl_t
	{
		// General
		Formats::Format Format;
		unsigned int Width;
		unsigned int Height;

		// Agnostic
		TextureTargets::Target Bound;

		// OpenGL
		unsigned int ID;
	};

	// Purpose: Constructor
	Texture::Texture( void )
	{
		Impl = new TextureImpl_t();
		Impl->Width = 0;
		Impl->Height = 0;
		Impl->ID = 0;
		Impl->Bound = TextureTargets::NotBound;
	}

	Texture::~Texture()
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

	// Purpose: Constructor
	// Mipmap: If true, generates mipmaps for the image and sets the appropriate filters.
	Texture::Texture( unsigned int Width, unsigned int Height, Formats::Format Format, bool Mipmap )
	{
		Impl = new TextureImpl_t();
		Impl->Format = Format;
		Impl->Width = Width;
		Impl->Height = Height;
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

			default: Impl->Format = Formats::R8G8B8A8; GlPixelFormat = GL_RGBA; GlComponents = 4; GlType = GL_UNSIGNED_BYTE; break;
		}

		glGenTextures(1, &Impl->ID);
		glBindTexture(GL_TEXTURE_2D, Impl->ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mipmap ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GlComponents, Width, Height, 0, GlPixelFormat, GlType, 0);

		if( Mipmap )
		{
			// ATI bug.
			glEnable(GL_TEXTURE_2D);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Some graphics drivers/cards require textures to be 'flushed'.
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool Texture::LoadFromFile( const std::string& Filename, bool Mipmap, Texture** Result )
	{
		// Detect, load and convert an image.
		FREE_IMAGE_FORMAT Format = FreeImage_GetFileType(Filename.c_str(), 0);
		FIBITMAP* Image = FreeImage_Load(Format, Filename.c_str());

		if( !Image )
			return false;

		FIBITMAP* Image32 = FreeImage_ConvertTo32Bits(Image);

		if( !Image32 )
		{
			FreeImage_Unload(Image);
			return false;
		}

		FreeImage_Unload(Image);

		// Initialize.
		Texture* Texture = new Graphics::Texture();
		Texture->Impl->Format = Graphics::Formats::R8G8B8A8;
		Texture->Impl->Width = FreeImage_GetWidth(Image32);
		Texture->Impl->Height = FreeImage_GetHeight(Image32);
		Texture->Impl->Bound = TextureTargets::NotBound;

		// Generate texture (on OpenGL).
		glGenTextures(1, &Texture->Impl->ID);
		glBindTexture(GL_TEXTURE_2D, Texture->Impl->ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mipmap ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Convert to OpenGL data.
		unsigned char* Converted = new unsigned char[Texture->Impl->Width * Texture->Impl->Height * 4];
		unsigned char* Pixels = (unsigned char*)FreeImage_GetBits(Image32);

		for( unsigned int i = 0; i < Texture->Impl->Width * Texture->Impl->Height; ++i )
		{
			Converted[i * 4 + 0] = Pixels[i * 4 + 2];
			Converted[i * 4 + 1] = Pixels[i * 4 + 1];
			Converted[i * 4 + 2] = Pixels[i * 4 + 0];
			Converted[i * 4 + 3] = Pixels[i * 4 + 3];
		}

		glTexImage2D(GL_TEXTURE_2D, 0, 4, Texture->Impl->Width, Texture->Impl->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Converted);

		if( Mipmap )
		{
			// ATI bug.
			glEnable(GL_TEXTURE_2D);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Some graphics drivers/cards require textures to be 'flushed'.
		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] Converted;
		*Result = Texture;

		return true;
	}

	void Texture::Bind( TextureTargets::Target Target )
	{
		Impl->Bound = Target;

		switch(Target)
		{
			case TextureTargets::DepthBuffer: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			
			case TextureTargets::ColorBuffer0: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer1: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer2: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer3: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer4: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer5: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer6: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;
			case TextureTargets::ColorBuffer7: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT7_EXT, GL_TEXTURE_2D, Impl->ID, 0); return; break;

			case TextureTargets::Texture0: glActiveTexture(GL_TEXTURE0); break;
			case TextureTargets::Texture1: glActiveTexture(GL_TEXTURE1); break;
			case TextureTargets::Texture2: glActiveTexture(GL_TEXTURE2); break;
			case TextureTargets::Texture3: glActiveTexture(GL_TEXTURE3); break;
			case TextureTargets::Texture4: glActiveTexture(GL_TEXTURE4); break;
			case TextureTargets::Texture5: glActiveTexture(GL_TEXTURE5); break;
			case TextureTargets::Texture6: glActiveTexture(GL_TEXTURE6); break;
			case TextureTargets::Texture7: glActiveTexture(GL_TEXTURE7); break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Impl->ID);
	}

	void Texture::Unbind( void )
	{
		Texture::Unbind(Impl->Bound);
		Impl->Bound = TextureTargets::NotBound;
	}

	void Texture::Unbind( TextureTargets::Target Target )
	{
		switch(Target)
		{
			case TextureTargets::DepthBuffer: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			
			case TextureTargets::ColorBuffer0: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer1: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer2: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer3: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer4: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer5: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer6: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			case TextureTargets::ColorBuffer7: glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT7_EXT, GL_TEXTURE_2D, 0, 0); return; break;
			
			case TextureTargets::Texture0: glActiveTexture(GL_TEXTURE0); break;
			case TextureTargets::Texture1: glActiveTexture(GL_TEXTURE1); break;
			case TextureTargets::Texture2: glActiveTexture(GL_TEXTURE2); break;
			case TextureTargets::Texture3: glActiveTexture(GL_TEXTURE3); break;
			case TextureTargets::Texture4: glActiveTexture(GL_TEXTURE4); break;
			case TextureTargets::Texture5: glActiveTexture(GL_TEXTURE5); break;
			case TextureTargets::Texture6: glActiveTexture(GL_TEXTURE6); break;
			case TextureTargets::Texture7: glActiveTexture(GL_TEXTURE7); break;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	TextureTargets::Target Texture::BoundTo( void )
	{
		return Impl->Bound;
	}
			
	unsigned int Texture::GetWidth( void )
	{
		return Impl->Width;
	}

	unsigned int Texture::GetHeight( void )
	{
		return Impl->Height;
	}

	Graphics::Color Texture::GetPixelColorAt( unsigned int X, unsigned int Y )
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Impl->ID);

		unsigned char* Data = new unsigned char[Impl->Width * Impl->Height * 4];
		unsigned int Offset = ((Impl->Height - Y) * Impl->Width + X) * 4;
		Graphics::Color Color = Graphics::Color(0, 0, 0, 0);

		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);

		Color.R = Data[Offset + 0];
		Color.G = Data[Offset + 1];
		Color.B = Data[Offset + 2];
		Color.A = Data[Offset + 3];

		delete[] Data;

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		return Color;
	}
}
