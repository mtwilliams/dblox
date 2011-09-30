#ifndef DBLOX_BLOCKS_BLOCKS_H
#define DBLOX_BLOCKS_BLOCKS_H

#include "dblox/Blocks/Block.h"
#include "dblox/Blocks/AirBlock.h"
#include "dblox/Blocks/DirtBlock.h"

namespace Blocks
{
    enum Block
    {
        Air = 0,
        Dirt,
        Count
    };
}

class BlockManager
{
    public:
        static Block* GetBlock( Blocks::Block Block );

    protected:
        Block* m_Blocks[Blocks::Count];

    protected:
        static BlockManager* Instance;

        BlockManager( void );
        ~BlockManager();

        static BlockManager* GetInstance( void );
};

#endif