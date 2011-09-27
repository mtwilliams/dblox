#ifndef GRAPHICS_SPRITEBATCH_H
#define GRAPHICS_SPRITEBATCH_H

#include "Graphics/Exports.h"
#include "Math/Vector2.h"
#include "Graphics/Color.h"
#include "Graphics/Texture.h"
#include "Graphics/Primitive.h"
#include "Graphics/VertexBuffer.h"

using Math::Vector2;
using Graphics::Color;

namespace Graphics
{
	struct SpriteBatchImpl_t;
	class SpriteBatch
	{
		public:
			SpriteBatch( void );
			~SpriteBatch();

			void Begin( unsigned int Width, unsigned int Height );
			void Flush( void );
			void End( void );

			void AddQuad( Texture* Texture, const Vector2& Min, const Vector2& Max, float Depth, const Vector2& U, const Vector2& V, const Color& Color = Color::White );
			void AddVertex( const Vector2& Position, float Depth, float U, float V, const Color& Color = Color::White );

		protected:
			SpriteBatchImpl_t* Impl;
	};
}

#endif