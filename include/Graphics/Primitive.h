#ifndef GRAPHICS_PRIMITIVE_H
#define GRAPHICS_PRIMITIVE_H

#include "Graphics/Exports.h"

namespace Graphics
{
	namespace Primitives
	{
		enum Type
		{
			Lines = 0,
			LineStrip,
			Triangles,
			TriangleStrip,
			TriangleFan
		};
	}
}

#endif