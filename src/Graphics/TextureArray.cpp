#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Graphics/TextureArray.h"

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
	struct TextureArrayImpl_t
    {
        // General
		Formats::Format Format;
		unsigned int Width;
		unsigned int Height;
        unsigned int Layers;

        // Agnostic
        TextureTargets::Target Bound;

		// OpenGL
		unsigned int ID;
    };

    TextureArray::TextureArray( void )
        : Impl(0)
    {
        Impl = new TextureArrayImpl_t();
        Impl->Width = 0;
		Impl->Height = 0;
        Impl->Layers = 0;
		Impl->ID = 0;
        Impl->Bound = TextureTargets::NotBound;
    }

    TextureArray::~TextureArray()
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

    bool TextureArray::LoadFromFile( const std::string& Filename, unsigned int Width, unsigned int Height, bool Mipmap, TextureArray** Result )
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
		TextureArray* Texture = new Graphics::TextureArray();
		Texture->Impl->Format = Graphics::Formats::R8G8B8A8;
		Texture->Impl->Width = Width;
		Texture->Impl->Height = Height;
        Texture->Impl->Layers = FreeImage_GetHeight(Image32) / Height;
        Texture->Impl->Bound = TextureTargets::NotBound;

        // Generate texture (on OpenGL).
		glGenTextures(1, &Texture->Impl->ID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, Texture->Impl->ID);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, Mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, Mipmap ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Convert to OpenGL data.
		unsigned char* Converted = new unsigned char[FreeImage_GetWidth(Image32) * FreeImage_GetHeight(Image32) * 4];
		unsigned char* Pixels = (unsigned char*)FreeImage_GetBits(Image32);

		for( unsigned int i = 0; i < FreeImage_GetWidth(Image32) * FreeImage_GetHeight(Image32); ++i )
		{
			Converted[i * 4 + 0] = Pixels[i * 4 + 2];
			Converted[i * 4 + 1] = Pixels[i * 4 + 1];
			Converted[i * 4 + 2] = Pixels[i * 4 + 0];
			Converted[i * 4 + 3] = Pixels[i * 4 + 3];
		}

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, Texture->Impl->Width, Texture->Impl->Height, Texture->Impl->Layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, Converted);

		if( Mipmap )
		{
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		}

		// Some graphics drivers/cards require textures to be 'flushed'.
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		delete[] Converted;
		*Result = Texture;

		return true;
    }

    void TextureArray::Bind( TextureTargets::Target Target )
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

		glBindTexture(GL_TEXTURE_2D_ARRAY, Impl->ID);
    }

	void TextureArray::Unbind( void )
    {
        switch(Impl->Bound)
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

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        Impl->Bound = TextureTargets::NotBound;
    }

    unsigned int TextureArray::GetWidth( void )
    {
        return Impl->Width;
    }

    unsigned int TextureArray::GetHeight( void )
    {
        return Impl->Height;
    }

    unsigned int TextureArray::GetLayers( void )
    {
        return Impl->Layers;
    }
}