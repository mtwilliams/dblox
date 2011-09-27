#include "Math/Vector2.h"

#include <math.h>

namespace Math
{
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);

	Vector2::Vector2( const Vector2& Other )
		: X(Other.X)
		, Y(Other.Y)
	{
	}

	Vector2::Vector2( float X, float Y )
		: X(X)
		, Y(Y)
	{
	}

	float Vector2::DotProduct( const Vector2& RHS ) const
	{
		return X * RHS.X + Y * RHS.Y;
	}

	float Vector2::Magnitude( void ) const
	{
		return sqrt(X * X + Y * Y);
	}

	void Vector2::Normalize( void )
	{
		float Mag = 1.0 / Magnitude();

		X *= Mag;
		Y *= Mag;
	}

	Vector2 Vector2::Normalized( void ) const
	{
		Vector2 Normalized = Vector2( X, Y );
		Normalized.Normalize();
		return Normalized;
	}

	bool Vector2::operator==( const Vector2& RHS ) const 
	{
		return (abs(X - RHS.X ) <= 0.001f) && (abs(Y - RHS.Y ) <= 0.001f);
	}

	Vector2 Vector2::operator+(const Vector2& RHS) const 
	{
		return Vector2( X + RHS.X,  Y + RHS.Y);
	}

	Vector2 Vector2::operator-(const Vector2& RHS) const
	{
		return Vector2( X - RHS.X,  Y - RHS.Y);
	}

	Vector2 Vector2::operator-( void ) const
	{
		return Vector2(-X, -Y);
	}

	Vector2 Vector2::operator/( float Scalar ) const
	{
		float Inv = 1.0 / Scalar;
		return Vector2 (X * Inv , Y * Inv);
	}

	Vector2 Vector2::operator/(const Vector2& RHS) const
	{
		return Vector2 (X / RHS.X,  Y / RHS.Y);
	}

	Vector2 Vector2::operator*(const Vector2& RHS) const
	{
		return Vector2 (X * RHS.X,  Y * RHS.Y);
	}

	Vector2 Vector2::operator*( float Scalar ) const
	{
		return Vector2 (X * Scalar,  Y * Scalar);
	}
}
