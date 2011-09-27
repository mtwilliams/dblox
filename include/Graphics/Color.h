#ifndef GRAPHICS_COLOR_H
#define GRAPHICS_COLOR_H

#include "Graphics/Exports.h"

namespace Graphics
{
	class DBLOX_GRAPHICS_EXPORT Color
	{
		public:
			Color( unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255 );

		public:
			static const Color Transparent;
			static const Color White;
			static const Color Black;
			static const Color Red;
			static const Color Green;
			static const Color Blue;

		public:
			unsigned char R, G, B, A;
	};
}

#endif