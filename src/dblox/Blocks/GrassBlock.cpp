#include "dblox/Blocks/GrassBlock.h"

int GrassBlock::OnEvent( const BlockEvent_t& Event )
{
    return 0;
}

int GrassBlock::GetTextureOnFace( unsigned int Face )
{
    if( Face == BLOCK_FACE_TOP )
        return 1;

    if( Face == BLOCK_FACE_BOTTOM )
        return 2;

    return 4;
}

int GrassBlock::GetFlags( void )
{
    return BLOCK_FLAG_NONE;
}