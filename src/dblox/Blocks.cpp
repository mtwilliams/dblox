#include "dblox/Blocks.h"

namespace Blocks
{
	std::bitset<Blocks::Count>& GetTranslucentBlocks( void )
	{
		static std::bitset<Blocks::Count> TranslucentBlocks;
		return TranslucentBlocks;
	}

	bool Initialize( void )
	{
		std::bitset<Blocks::Count>& TranslucentBlocks = GetTranslucentBlocks();

		// Air
		TranslucentBlocks[0] = 1;

		return true;
	}

	bool IsBlockTranslucent( unsigned int Block )
	{
		return GetTranslucentBlocks()[Block];
	}
}