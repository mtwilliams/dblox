#ifndef DBLOX_TEXTURES_H
#define DBLOX_TEXTURES_H

#include "Graphics/Texture.h"

#include <map>
#include <string>

namespace Textures
{
	extern bool Initialize( void );
	extern Graphics::Texture* GetTexture( const std::string& Texture );
}

#endif
