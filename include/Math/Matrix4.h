#ifndef DBLOX_MATH_MATRIX4_H
#define DBLOX_MATH_MATRIX4_H

#include "Math/Exports.h"
#include "Math/Vector3.h"

namespace Math
{
	namespace MatrixType
	{
		enum Type
		{
			ModelView,
			Projection,
		};
	}

	class DBLOX_MATH_EXPORT Matrix4
	{
		public:
			Matrix4();

			Vector3 operator*( const Vector3& Vector );
			Matrix4 operator*( const Matrix4& Other );

			operator float*() const;

			void Translate( const Vector3& Vector );
			void Scale( const Vector3& Vector );
			void Rotate( const Vector3& Vector );

			void Ortho( float Left, float Right, float Bottom, float Top, float Near = -1.0f, float Far = 1.0f );
			void Perspective( float FoV, float AspectRatio, float Near, float Far );
			void LookAt( const Vector3& Eye, const Vector3& Target, const Vector3& Up );

			bool Invert( void );

			void Load( MatrixType::Type Type ) const;
			static void Matrix4::LoadIdentity( MatrixType::Type Type );
			float* AsArray( void ) const;

		protected:
			float Matrix[16];

		protected:
			Matrix4( float* Matrix );

		public:
			static const Matrix4 Identity;
	};
}

#endif