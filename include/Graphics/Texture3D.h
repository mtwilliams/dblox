#ifndef GRAPHICS_TEXTURE3D_H
#define GRAPHICS_TEXTURE3D_H

#include "Graphics/Exports.h"
#include "Graphics/Format.h"
#include "Graphics/Color.h"
#include "Graphics/Texture.h"

#include <string>

namespace Graphics
{
	struct Texture3DImpl_t;
	class DBLOX_GRAPHICS_EXPORT Texture3D
	{
		public:
			Texture3D( void );
			Texture3D( unsigned int Width, unsigned int Height, unsigned int Depth, Formats::Format Format, bool Mipmap );
			~Texture3D();

			void Bind( TextureTargets::Target Target );
			void Unbind( void );

			static void Unbind( TextureTargets::Target Target );

			TextureTargets::Target BoundTo( void );
			
			unsigned int GetWidth( void );
			unsigned int GetHeight( void );
			unsigned int GetDepth( void );
			
		protected:
			Texture3DImpl_t* Impl;
	};
}

#endif