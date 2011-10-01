#include "dblox/Blocks/StoneBlock.h"

int StoneBlock::OnEvent( const BlockEvent_t& Event )
{
    return 0;
}

int StoneBlock::GetTextureOnFace( unsigned int Face )
{
    return 6;
}

int StoneBlock::GetFlags( void )
{
    return BLOCK_FLAG_NONE;
}