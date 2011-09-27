#ifndef DBLOX_MATH_VECTOR3_H
#define DBLOX_MATH_VECTOR3_H

#include "Math/Exports.h"

namespace Math
{
	class DBLOX_MATH_EXPORT Vector3
	{
		public:
			Vector3( const Vector3& Other );
			Vector3( float X, float Y, float Z );

			float DotProduct( const Vector3& RHS ) const;
			Vector3 CrossProduct( const Vector3& RHS ) const;

			float Magnitude( void ) const;
			void Normalize( void );
			Vector3 Normalized( void ) const;

			bool operator==( const Vector3& RHS ) const;
			Vector3 operator+(const Vector3& RHS) const;
			Vector3 operator-(const Vector3& RHS) const;
			Vector3 operator-( void ) const;
			Vector3 operator/( float Scalar ) const;
			Vector3 operator/(const Vector3& RHS) const;
			Vector3 operator*(const Vector3& RHS) const;
			Vector3 operator*( float Scalar ) const;

			float X, Y, Z;

		public:
			static const Vector3 Zero;
	};
}

#endif