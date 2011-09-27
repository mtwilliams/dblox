#ifndef DBLOX_UTILS_RANDOM_H
#define DBLOX_UTILS_RANDOM_H

#include "Utils/Exports.h"

namespace Utils
{
	struct RandomImpl_t;
	class DBLOX_UTILS_EXPORT Random
	{
		public:
			static Random* GetInstance();

			static unsigned int Integer( void );
			static double Double( void );

		private:
			static Random* Instance;
			RandomImpl_t* Impl;

			Random( void );
			~Random();
	};
}

#endif