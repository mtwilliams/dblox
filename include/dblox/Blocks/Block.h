#ifndef DBLOX_BLOCKS_BLOCK_H
#define DBLOX_BLOCKS_BLOCK_H

#include "dblox/Blocks/Event.h"

#define BLOCK_FACE_LEFT   1
#define BLOCK_FACE_RIGHT  2
#define BLOCK_FACE_FRONT  3
#define BLOCK_FACE_BACK   4
#define BLOCK_FACE_TOP    5
#define BLOCK_FACE_BOTTOM 6

#define BLOCK_FLAG_NONE        0
#define BLOCK_FLAG_NODRAW      1 // Don't draw
#define BLOCK_FLAG_TRANSLUCENT 2 // Can see through
#define BLOCK_FLAG_PHANTOM     4 // Doesn't collide

class Block
{
    public:
        virtual ~Block()
        {
        }

        virtual int OnEvent( const BlockEvent_t& Event ) = 0;
        virtual int GetTextureOnFace( unsigned int Face ) = 0;
        virtual int GetFlags( void ) = 0;
};

#endif