#ifndef DBLOX_UTILS_COORDINATES_H
#define DBLOX_UTILS_COORDINATES_H

#include "Utils/Exports.h"

namespace Utils
{
	inline unsigned int MapToPositive( int Value )
    {
	    if( Value >= 0 )
		    return Value * 2;

	    return -Value * 2 - 1;
    }

    inline unsigned int GetCantorsPair( int X, int Z )
    {
	    unsigned int PosX = MapToPositive(X);
	    unsigned int PosZ = MapToPositive(Z);

	    return ((PosX + PosZ) * (PosX + PosZ + 1) / 2) + PosZ;
    }
}

#endif