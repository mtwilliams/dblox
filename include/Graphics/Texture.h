#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include "Graphics/Exports.h"
#include "Graphics/Format.h"
#include "Graphics/Color.h"

#include <string>

namespace Graphics
{
	namespace TextureTargets
	{
		typedef enum
		{
			DepthBuffer,
			ColorBuffer0,
			ColorBuffer1,
			ColorBuffer2,
			ColorBuffer3,
			ColorBuffer4,
			ColorBuffer5,
			ColorBuffer6,
			ColorBuffer7,
			Texture0,
			Texture1,
			Texture2,
			Texture3,
			Texture4,
			Texture5,
			Texture6,
			Texture7,
			NotBound
		} Target;
	}

	struct TextureImpl_t;
	class DBLOX_GRAPHICS_EXPORT Texture
	{
		public:
			Texture( void );
			Texture( unsigned int Width, unsigned int Height, Formats::Format Format, bool Mipmap );
			~Texture();

			static bool LoadFromFile( const std::string& Filename, bool Mipmap, Texture** Result );

			void Bind( TextureTargets::Target Target );
			void Unbind( void );

			static void Unbind( TextureTargets::Target Target );

			TextureTargets::Target BoundTo( void );
			
			unsigned int GetWidth( void );
			unsigned int GetHeight( void );

			Graphics::Color GetPixelColorAt( unsigned int X, unsigned int Y );

		protected:
			TextureImpl_t* Impl;
	};
}

#endif