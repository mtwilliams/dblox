#ifndef DBLOX_RESOURCES_H
#define DBLOX_RESOURCES_H

#include "Graphics/Texture.h"
#include "dblox/ResourceManager.h"

namespace Resources
{
    Resource::Manager<Graphics::Texture*>* TextureManager();

    extern bool Precache( void );
}

#endif