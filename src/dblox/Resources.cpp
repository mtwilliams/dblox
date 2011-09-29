#include "Shared/Helpers.h"
#include "dblox/Resources.h"

#include <stdio.h>

namespace Resources
{
    Graphics::Texture* TextureManager_Create( const std::string& Filename )
    {
        Graphics::Texture* Result;

        if( !Graphics::Texture::LoadFromFile(Filename, true, &Result) )
            return 0;

        return Result;
    }

    void TextureManager_Destroy( Graphics::Texture* Texture )
    {
        SAFE_DELETE(Texture);
    }

    Resource::Manager<Graphics::Texture*>* TextureManager()
    {
        static Resource::Manager<Graphics::Texture*>* Mngr = new Resource::Manager<Graphics::Texture*>(TextureManager_Create, TextureManager_Destroy);
        return Mngr;
    }

    typedef struct
    {
        const char* Filename;
    } TexturePrecache_t;

    static const TexturePrecache_t PrecacheTextures[] = {
        { "Textures/Logo.png" },
        { 0, }
    };

    bool Precache( void )
    {
        Resource::Manager<Graphics::Texture*>* TextureMngr = Resources::TextureManager();

        for( unsigned int i = 0; ; ++i )
        {
            if( !PrecacheTextures[i].Filename )
                break;

            Resource::Handle<Graphics::Texture*>* Handle = TextureMngr->Load(PrecacheTextures[i].Filename);
            Graphics::Texture* Texture = (*Handle);

            printf("Info: Precaching '%s'\n", PrecacheTextures[i].Filename);

            if( !Texture )
            {
                printf("Error (Precache): Failed to precache '%s'\n", PrecacheTextures[i].Filename);
                return false;
            }
        }

        return true;
    }
}
