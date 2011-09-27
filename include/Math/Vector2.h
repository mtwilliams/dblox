#ifndef DBLOX_MATH_VECTOR2_H
#define DBLOX_MATH_VECTOR2_H

#include "Math/Exports.h"

namespace Math
{
	class DBLOX_MATH_EXPORT Vector2
	{
		public:
			Vector2( const Vector2& Other );
			Vector2( float X, float Y );

			float DotProduct( const Vector2& RHS ) const;

			float Magnitude( void ) const;
			void Normalize( void );
			Vector2 Normalized( void ) const;

			bool operator==( const Vector2& RHS ) const;
			Vector2 operator+(const Vector2& RHS) const;
			Vector2 operator-(const Vector2& RHS) const;
			Vector2 operator-( void ) const;
			Vector2 operator/( float Scalar ) const;
			Vector2 operator/(const Vector2& RHS) const;
			Vector2 operator*(const Vector2& RHS) const;
			Vector2 operator*( float Scalar ) const;

			float X, Y;

		public:
			static const Vector2 Zero;
};
}

#endif