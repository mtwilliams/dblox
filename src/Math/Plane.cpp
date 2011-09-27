#include "Math/Plane.h"

namespace Math
{
	Plane::Plane( const Plane& Other )
		: Normal(Other.Normal)
		, Intercept(Other.Intercept)
	{
	}

	Plane::Plane( const Vector3& Normal, float Intercept )
		: Normal(Normal)
		, Intercept(Intercept)
	{
	}

	Plane::Classification Plane::Classify( const Vector3& Point )
	{
		float Distance = Normal.DotProduct(Point) - Intercept;

		if( Distance >= 0.0f )
			return Plane::Behind;

		return Plane::Infront;
	}
}
