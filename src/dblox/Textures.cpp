#include "dblox/Textures.h"

namespace Textures
{
	std::map<std::string, Graphics::Texture*>& GetTextures()
	{
		static std::map<std::string, Graphics::Texture*> Textures;
		return Textures;
	}

	Graphics::Texture* GetTexture( const std::string& Texture )
	{
		std::map<std::string, Graphics::Texture*>::const_iterator iter = GetTextures().find(Texture);

		if( iter == GetTextures().end() )
			return 0;

		return iter->second;
	}

	bool Initialize( void )
	{
		Graphics::Texture* Texture;

		if( !Graphics::Texture::LoadFromFile("terrain.png", false, &Texture) )
			return false;

		GetTextures().insert(std::make_pair("Terrain", Texture));

		return true;
	}
}
