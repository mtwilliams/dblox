#include "Shared/Platform.h"
#include "Shared/Helpers.h"
#include "Graphics/Color.h"

namespace Graphics
{
	Color::Color( unsigned char R, unsigned char G, unsigned char B, unsigned char A )
		: R(R)
		, G(G)
		, B(B)
		, A(A)
	{
	}

	const Color Color::Transparent = Color(255, 255, 255, 0);
	const Color Color::White = Color(255, 255, 255);
	const Color Color::Black = Color(0, 0, 0);
	const Color Color::Red = Color(255, 0, 0);
	const Color Color::Green = Color(0, 255, 0);
	const Color Color::Blue = Color(0, 0, 255);
}
