#include "Math/Vector3.h"

#include <math.h>

namespace Math
{
	const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);

	Vector3::Vector3( const Vector3& Other )
		: X(Other.X)
		, Y(Other.Y)
		, Z(Other.Z)
	{
	}

	Vector3::Vector3( float X, float Y, float Z )
		: X(X)
		, Y(Y)
		, Z(Z)
	{
	}

	float Vector3::DotProduct( const Vector3& RHS ) const
	{
		return X * RHS.X + Y * RHS.Y + Z * RHS.Z;
	}

	Vector3 Vector3::CrossProduct( const Vector3& RHS ) const
	{
		return Vector3(Y * RHS.Z - Z * RHS.Y, X * RHS.X - X * RHS.Z, X * RHS.Y -  Y * RHS.X);
	}

	float Vector3::Magnitude( void ) const
	{
		return sqrt(X * X + Y * Y + Z * Z);
	}

	void Vector3::Normalize( void )
	{
		float Mag = 1.0f / Magnitude();

		X *= Mag;
		Y *= Mag;
		Z *= Mag;
	}

	Vector3 Vector3::Normalized( void ) const
	{
		Vector3 Normalized = Vector3( X, Y, Z );
		Normalized.Normalize();
		return Normalized;
	}

	bool Vector3::operator==( const Vector3& RHS ) const 
	{
		return (abs(X - RHS.X ) <= 0.001f) && (abs(Y - RHS.Y ) <= 0.001f) && (abs(Z - RHS.Z ) <= 0.001f);
	}

	Vector3 Vector3::operator+(const Vector3& RHS) const 
	{
		return Vector3( X + RHS.X,  Y + RHS.Y,  Z + RHS.Z);
	}

	Vector3 Vector3::operator-(const Vector3& RHS) const
	{
		return Vector3( X - RHS.X,  Y - RHS.Y,  Z - RHS.Z);
	}

	Vector3 Vector3::operator-( void ) const
	{
		return Vector3(-X, -Y, -Z);
	}

	Vector3 Vector3::operator/( float Scalar ) const
	{
		float Inv = 1.0f / Scalar;
		return Vector3 (X * Inv , Y * Inv, Z * Inv);
	}

	Vector3 Vector3::operator/(const Vector3& RHS) const
	{
		return Vector3 (X / RHS.X,  Y / RHS.Y,  Z / RHS.Z);
	}

	Vector3 Vector3::operator*(const Vector3& RHS) const
	{
		return Vector3 (X * RHS.X,  Y * RHS.Y,  Z * RHS.Z);
	}

	Vector3 Vector3::operator*( float Scalar ) const
	{
		return Vector3 (X * Scalar,  Y * Scalar,  Z * Scalar);
	}
}
