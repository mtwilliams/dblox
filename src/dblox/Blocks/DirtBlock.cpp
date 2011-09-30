#include "dblox/Blocks/DirtBlock.h"

int DirtBlock::OnEvent( const BlockEvent_t& Event )
{
    return 0;
}

int DirtBlock::GetTextureOnFace( unsigned int Face )
{
    if( Face == BLOCK_FACE_TOP )
        return 1;

    return 2;
}

int DirtBlock::GetFlags( void )
{
    return BLOCK_FLAG_NONE;
}