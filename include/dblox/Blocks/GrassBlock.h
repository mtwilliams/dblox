#ifndef DBLOX_BLOCKS_GRASS_BLOCK_H
#define DBLOX_BLOCKS_GRASS_BLOCK_H

#include "dblox/Blocks/Block.h"

class GrassBlock : public Block
{
    public:
        int OnEvent( const BlockEvent_t& Event );
        int GetTextureOnFace( unsigned int Face );
        int GetFlags( void );
};

#endif