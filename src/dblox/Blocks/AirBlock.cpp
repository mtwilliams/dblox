#include "dblox/Blocks/AirBlock.h"

int AirBlock::OnEvent( const BlockEvent_t& Event )
{
    // TODO: Implement player breath.
    /*

    if( Event.Type == BLOCK_EVENT_COLLISION && Event.Data.Collision.Player )
    {
        Event.Data.Collision.Player->IncreaseStat(PLAYER_STAT_BREATH, 1);
    }

    */

    return 0;
}

int AirBlock::GetTextureOnFace( unsigned int Face )
{
    return 0;
}

int AirBlock::GetFlags( void )
{
    return BLOCK_FLAG_NODRAW | BLOCK_FLAG_TRANSLUCENT | BLOCK_FLAG_PHANTOM;
}