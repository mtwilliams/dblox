#ifndef GRAPHICS_RENDERTARGET_H
#define GRAPHICS_RENDERTARGET_H

#include "Graphics/Exports.h"

#include "Graphics/Format.h"
#include "Graphics/Texture.h"

namespace Graphics
{
	struct RenderTargetImpl_t;
	class DBLOX_GRAPHICS_EXPORT RenderTarget
	{
		public:
			RenderTarget( unsigned int Width, unsigned int Height, bool Depth );
			~RenderTarget();

			void Bind( TextureTargets::Target = TextureTargets::ColorBuffer0 );
			void Unbind( void );
			
			unsigned int GetWidth( void );
			unsigned int GetHeight( void );

			Texture* GetColorBuffer( void );
			Texture* GetDepthBuffer( void );

		protected:
			RenderTargetImpl_t* Impl;
	};
}

#endif