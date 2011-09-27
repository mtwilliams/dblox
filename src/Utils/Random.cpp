#include "Shared/Helpers.h"
#include "Utils/Random.h"
#include "Utils/MersenneTwister.h"

#include <time.h>

namespace Utils
{
	struct RandomImpl_t
	{
		MTRand_int32* IRand;
		MTRand* DRand;
	};

	Random* Random::Instance = 0;
	Random* Random::GetInstance()
	{
		if( !Random::Instance )
			Random::Instance = new Random();

		return Random::Instance;
	}

	unsigned int Random::Integer( void )
	{
		return (*(Random::GetInstance()->Impl->IRand))();
	}

	double Random::Double( void )
	{
		return (*(Random::GetInstance()->Impl->DRand))();
	}

	Random::Random( void )
		: Impl(0)
	{
		Impl = new RandomImpl_t();
		Impl->IRand = new MTRand_int32(time(0));
		Impl->DRand = new MTRand();
	}

	Random::~Random()
	{
		SAFE_DELETE(Impl->DRand);
		SAFE_DELETE(Impl->IRand);
		SAFE_DELETE(Impl);
	}
}
