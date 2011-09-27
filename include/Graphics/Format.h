#ifndef GRAPHICS_FORMAT_H
#define GRAPHICS_FORMAT_H

#include "Graphics/Exports.h"

namespace Graphics
{
	namespace Formats
	{
		typedef enum
		{
			D24,
			G8,
			R8G8B8,
			R8G8B8A8,
			R16G16F,
			R32F,
			Invalid
		} Format;
	}
}

#endif