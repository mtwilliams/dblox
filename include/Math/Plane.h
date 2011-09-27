#ifndef DBLOX_MATH_PLANE_H
#define DBLOX_MATH_PLANE_H

#include "Math/Exports.h"
#include "Math/Vector3.h"

namespace Math
{
	class DBLOX_MATH_EXPORT Plane
	{
		public:
			enum Classification
			{
				Behind,
				Infront
			};

		public:
			Plane( const Plane& Other );
			Plane( const Vector3& Normal, float Intercept );

			Plane::Classification Classify( const Vector3& Point );
			
		public:
			Vector3 Normal;
			float Intercept;
	};
}

#endif
