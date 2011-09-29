#ifndef GRAPHICS_TEXTURE_ARRAY_H
#define GRAPHICS_TEXTURE_ARRAY_H

#include "Graphics/Exports.h"
#include "Graphics/Format.h"
#include "Graphics/Color.h"
#include "Graphics/Texture.h"

#include <string>

namespace Graphics
{
	struct TextureArrayImpl_t;
	class DBLOX_GRAPHICS_EXPORT TextureArray
	{
		public:
			TextureArray( void );
			~TextureArray();

            static bool LoadFromFile( const std::string& Filename, unsigned int Width, unsigned int Height, bool Mipmap, TextureArray** Result );

            void Bind( TextureTargets::Target Target );
			void Unbind( void );

			unsigned int GetWidth( void );
			unsigned int GetHeight( void );
			unsigned int GetLayers( void );
			
		protected:
			TextureArrayImpl_t* Impl;
	};
}

#endif