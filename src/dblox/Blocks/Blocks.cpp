#include "Shared/Helpers.h"
#include "dblox/Blocks/Blocks.h"

Block* BlockManager::GetBlock( Blocks::Block Block )
{
    if( Block > Blocks::Count )
    {
        __asm nop
    }

    return BlockManager::GetInstance()->m_Blocks[Block];
}

BlockManager::BlockManager( void )
{
    m_Blocks[Blocks::Air] = new AirBlock();
    m_Blocks[Blocks::Dirt] = new DirtBlock();
    m_Blocks[Blocks::Stone] = new StoneBlock();
    m_Blocks[Blocks::Grass] = new GrassBlock();
}

BlockManager::~BlockManager()
{
    for( unsigned int i = 0; i < Blocks::Count; ++i )
    {
        SAFE_DELETE(m_Blocks[i]);
    }
}

BlockManager* BlockManager::Instance = 0;
BlockManager* BlockManager::GetInstance( void )
{
    if( !BlockManager::Instance )
        BlockManager::Instance = new BlockManager();

    return BlockManager::Instance;
}