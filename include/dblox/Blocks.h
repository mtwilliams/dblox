#ifndef DBLOX_BLOCKS_H
#define DBLOX_BLOCKS_H

#include <bitset>

namespace Blocks
{
	static const unsigned int Count = 255;

	extern bool Initialize( void );
	extern bool IsBlockTranslucent( unsigned int Block );
}

#endif
