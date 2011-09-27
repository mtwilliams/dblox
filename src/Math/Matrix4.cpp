#include "Shared/Platform.h"
#include "Math/Matrix4.h"

#include <math.h>
#include <string.h>

#ifdef API_OPENGL
	#include <GL/glew.h>

	#ifdef PLATFORM_WINDOWS
		#include <windows.h>
		#include <gl/GL.h>
	#endif
#endif

namespace Math
{
	const Matrix4 Matrix4::Identity;

	Matrix4::Matrix4()
	{
		// Load an identity matrix.
		memset((void*)&Matrix, 0, sizeof(float) * 16);

		Matrix[0] = 1;
		Matrix[5] = 1;
		Matrix[10] = 1;
		Matrix[15] = 1;
	}

	Matrix4::Matrix4( float* Matrix )
	{
		// Copy the matrix over.
		for( int i = 0; i < 16; ++i )
			this->Matrix[i] = Matrix[i];
	}

	Vector3 Matrix4::operator*( const Vector3& Vector )
	{
		float X = Vector.X * Matrix[0] + Vector.Y * Matrix[1] + Vector.Z * Matrix[2] + Matrix[3];
		float Y = Vector.Y * Matrix[4] + Vector.Y * Matrix[5] + Vector.Z * Matrix[6] + Matrix[7];
		float Z = Vector.Z * Matrix[8] + Vector.Y * Matrix[9] + Vector.Z * Matrix[10] + Matrix[11];

		return Vector3(X, Y, Z);
	}

	Matrix4 Matrix4::operator*( const Matrix4& Other )
	{
		Matrix4 Result;

		Result.Matrix[0] = Matrix[0] * Other.Matrix[0] + Matrix[1] * Other.Matrix[4] + Matrix[2] * Other.Matrix[8] + Matrix[3] * Other.Matrix[12];
		Result.Matrix[1] = Matrix[0] * Other.Matrix[1] + Matrix[1] * Other.Matrix[5] + Matrix[2] * Other.Matrix[9] + Matrix[3] * Other.Matrix[13];
		Result.Matrix[2] = Matrix[0] * Other.Matrix[2] + Matrix[1] * Other.Matrix[6] + Matrix[2] * Other.Matrix[10] + Matrix[3] * Other.Matrix[14];
		Result.Matrix[3] = Matrix[0] * Other.Matrix[3] + Matrix[1] * Other.Matrix[7] + Matrix[2] * Other.Matrix[11] + Matrix[3] * Other.Matrix[15];
		Result.Matrix[4] = Matrix[4] * Other.Matrix[0] + Matrix[5] * Other.Matrix[4] + Matrix[6] * Other.Matrix[8] + Matrix[7] * Other.Matrix[12];
		Result.Matrix[5] = Matrix[4] * Other.Matrix[1] + Matrix[5] * Other.Matrix[5] + Matrix[6] * Other.Matrix[9] + Matrix[7] * Other.Matrix[13];
		Result.Matrix[6] = Matrix[4] * Other.Matrix[2] + Matrix[5] * Other.Matrix[6] + Matrix[6] * Other.Matrix[10] + Matrix[7] * Other.Matrix[14];
		Result.Matrix[7] = Matrix[4] * Other.Matrix[3] + Matrix[5] * Other.Matrix[7] + Matrix[6] * Other.Matrix[11] + Matrix[7] * Other.Matrix[15];
		Result.Matrix[8] = Matrix[8] * Other.Matrix[0] + Matrix[9] * Other.Matrix[4] + Matrix[10] * Other.Matrix[8] + Matrix[11] * Other.Matrix[12];
		Result.Matrix[9] = Matrix[8] * Other.Matrix[1] + Matrix[9] * Other.Matrix[5] + Matrix[10] * Other.Matrix[9] + Matrix[11] * Other.Matrix[13];
		Result.Matrix[10] = Matrix[8] * Other.Matrix[2] + Matrix[9] * Other.Matrix[6] + Matrix[10] * Other.Matrix[10] + Matrix[11] * Other.Matrix[14];
		Result.Matrix[11] = Matrix[8] * Other.Matrix[3] + Matrix[9] * Other.Matrix[7] + Matrix[10] * Other.Matrix[11] + Matrix[11] * Other.Matrix[15];
		Result.Matrix[12] = Matrix[12] * Other.Matrix[0] + Matrix[13] * Other.Matrix[4] + Matrix[14] * Other.Matrix[8] + Matrix[15] * Other.Matrix[12];
		Result.Matrix[13] = Matrix[12] * Other.Matrix[1] + Matrix[13] * Other.Matrix[5] + Matrix[14] * Other.Matrix[9] + Matrix[15] * Other.Matrix[13];
		Result.Matrix[14] = Matrix[12] * Other.Matrix[2] + Matrix[13] * Other.Matrix[6] + Matrix[14] * Other.Matrix[10] + Matrix[15] * Other.Matrix[14];
		Result.Matrix[15] = Matrix[12] * Other.Matrix[3] + Matrix[13] * Other.Matrix[7] + Matrix[14] * Other.Matrix[11] + Matrix[15] * Other.Matrix[15];
		
		return Result;
	}

	Matrix4::operator float*() const
	{
		return (float*)&Matrix;
	}

	void Matrix4::Translate( const Vector3& Vector )
	{
		float TranslationMatrix[] = {
			1.0f, 0.0f, 0.0f, Vector.X,
			0.0f, 1.0f, 0.0f, Vector.Y,
			0.0f, 0.0f, 1.0f, Vector.Z,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		*this = (*this) * Matrix4(TranslationMatrix);
	}

	void Matrix4::Scale( const Vector3& Vector )
	{
		float ScaleMatrix[] = {
			Vector.X, 0.0f, 0.0f, 0.0f,
			0.0f, Vector.Y, 0.0f, 0.0f,
			0.0f, 0.0f, Vector.Z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		*this = (*this) * Matrix4(ScaleMatrix);
	}

	void Matrix4::Rotate( const Vector3& Vector )
	{
		Vector3 Radians = Vector3(Vector.X * 0.0174532925f, Vector.Y * 0.0174532925f, Vector.Z  * 0.0174532925f);

		float RotationMatrixX[] = {
			1, 0, 0, 0,
			0, cos(Radians.X), -sin(Radians.X), 0,
			0, sin(Radians.X), cos(Radians.X), 0,
			0, 0, 0, 1
		};

		float RotationMatrixY[] = {
			cos(Radians.Y), 0, sin(Radians.Y), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sin(Radians.Y), 0, cos(Radians.Y), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		float RotationMatrixZ[] = {
			cos(Radians.Z), sin(Radians.Z), 0.0f, 0.0f,
			-sin(Radians.Z), cos(Radians.Z), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		*this = (*this) * Matrix4(RotationMatrixX);
		*this = (*this) * Matrix4(RotationMatrixY);
		*this = (*this) * Matrix4(RotationMatrixZ);
	}

	void Matrix4::Ortho( float Left, float Right, float Bottom, float Top, float Near, float Far )
	{
		float OrthogonalMatrix[] = {
			2.0f / (Right - Left), 0.0f, 0.0f, -(Right + Left) / (Right - Left),
			0.0f, 2.0f / (Top - Bottom), 0.0f, -(Top + Bottom) / (Top - Bottom),
			0.0f, 0.0f, -2.0f / (Far - Near), -(Far + Near) / (Far - Near),
			0.0f, 0.0f, 0.0f, 1.0f
		};

		*this = (*this) * Matrix4(OrthogonalMatrix);
	}

	void Matrix4::Perspective( float FoV, float AspectRatio, float Near, float Far )
	{
		float F = 1.0f / tan(FoV / 2.0f);
		float PerspectiveMatrix[] = {
			F / AspectRatio, 0.0f, 0.0f, 0.0f,
			0.0f, F, 0.0f, 0.0f,
			0.0f, 0.0f, (Far + Near) / (Near - Far), (2.0f * Far * Near) / (Near - Far),
			0.0f, 0.0f, -1.0f, 0.0f
		};

		*this = (*this) * Matrix4(PerspectiveMatrix);
	}

	void Matrix4::LookAt( const Vector3& Eye, const Vector3& Target, const Vector3& Up )
	{
		Vector3 F = (Target - Eye).Normalized();
		Vector3 Up1 = Up.Normalized();
		Vector3 S = F.CrossProduct(Up1);
		Vector3 U = S.CrossProduct(F);
		
		float LookAtMatrix[] = {
			S.X, S.Y, S.Z, 0.0f,
			U.X, U.Y, U.Z, 0.0f,
			-F.X, -F.Y, -F.Z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		*this = (*this) * Matrix4(LookAtMatrix);

		Translate(-Eye);
	}

	bool Matrix4::Invert( void )
	{
		float Inverse[16], Det;

		Inverse[0] = Matrix[5] * Matrix[10] * Matrix[15] - Matrix[5] * Matrix[11] * Matrix[14] - Matrix[9] * Matrix[6] * Matrix[15]
		+ Matrix[9] * Matrix[7] * Matrix[14] + Matrix[13] * Matrix[6] * Matrix[11] - Matrix[13] * Matrix[7] * Matrix[10];
		Inverse[4] =  -Matrix[4] * Matrix[10] * Matrix[15] + Matrix[4] * Matrix[11] * Matrix[14] + Matrix[8] * Matrix[6] * Matrix[15]
		- Matrix[8] * Matrix[7] * Matrix[14] - Matrix[12] * Matrix[6] * Matrix[11] + Matrix[12] * Matrix[7] * Matrix[10];
		Inverse[8] =   Matrix[4] * Matrix[9] * Matrix[15] - Matrix[4] * Matrix[11] * Matrix[13] - Matrix[8] * Matrix[5] * Matrix[15]
		+ Matrix[8] * Matrix[7] * Matrix[13] + Matrix[12] * Matrix[5] * Matrix[11] - Matrix[12] * Matrix[7] * Matrix[9];
		Inverse[12] = -Matrix[4] * Matrix[9] * Matrix[14] + Matrix[4] * Matrix[10] * Matrix[13] + Matrix[8] * Matrix[5] * Matrix[14]
		- Matrix[8] * Matrix[6] * Matrix[13] - Matrix[12] * Matrix[5] * Matrix[10] + Matrix[12] * Matrix[6] * Matrix[9];
		Inverse[1] =  -Matrix[1] * Matrix[10] * Matrix[15] + Matrix[1] * Matrix[11] * Matrix[14] + Matrix[9] * Matrix[2] * Matrix[15]
		- Matrix[9] * Matrix[3] * Matrix[14] - Matrix[13] * Matrix[2] * Matrix[11] + Matrix[13] * Matrix[3] * Matrix[10];
		Inverse[5] =   Matrix[0] * Matrix[10] * Matrix[15] - Matrix[0] * Matrix[11] * Matrix[14] - Matrix[8] * Matrix[2] * Matrix[15]
		+ Matrix[8] * Matrix[3] * Matrix[14] + Matrix[12] * Matrix[2] * Matrix[11] - Matrix[12] * Matrix[3] * Matrix[10];
		Inverse[9] =  -Matrix[0] * Matrix[9] * Matrix[15] + Matrix[0] * Matrix[11] * Matrix[13] + Matrix[8] * Matrix[1] * Matrix[15]
		- Matrix[8] * Matrix[3] * Matrix[13] - Matrix[12] * Matrix[1] * Matrix[11] + Matrix[12] * Matrix[3] * Matrix[9];
		Inverse[13] =  Matrix[0] * Matrix[9] * Matrix[14] - Matrix[0] * Matrix[10] * Matrix[13] - Matrix[8] * Matrix[1] * Matrix[14]
		+ Matrix[8] * Matrix[2] * Matrix[13] + Matrix[12] * Matrix[1] * Matrix[10] - Matrix[12] * Matrix[2] * Matrix[9];
		Inverse[2] =   Matrix[1] * Matrix[6] * Matrix[15] - Matrix[1] * Matrix[7] * Matrix[14] - Matrix[5] * Matrix[2] * Matrix[15]
		+ Matrix[5] * Matrix[3] * Matrix[14] + Matrix[13] * Matrix[2] * Matrix[7] - Matrix[13] * Matrix[3] * Matrix[6];
		Inverse[6] =  -Matrix[0] * Matrix[6] * Matrix[15] + Matrix[0] * Matrix[7] * Matrix[14] + Matrix[4] * Matrix[2] * Matrix[15]
		- Matrix[4] * Matrix[3] * Matrix[14] - Matrix[12] * Matrix[2] * Matrix[7] + Matrix[12] * Matrix[3] * Matrix[6];
		Inverse[10] =  Matrix[0] * Matrix[5] * Matrix[15] - Matrix[0] * Matrix[7] * Matrix[13] - Matrix[4] * Matrix[1] * Matrix[15]
		+ Matrix[4] * Matrix[3] * Matrix[13] + Matrix[12] * Matrix[1] * Matrix[7] - Matrix[12] * Matrix[3] * Matrix[5];
		Inverse[14] = -Matrix[0] * Matrix[5] * Matrix[14] + Matrix[0] * Matrix[6] * Matrix[13] + Matrix[4] * Matrix[1] * Matrix[14]
		- Matrix[4] * Matrix[2] * Matrix[13] - Matrix[12] * Matrix[1] * Matrix[6] + Matrix[12] * Matrix[2] * Matrix[5];
		Inverse[3] =  -Matrix[1] * Matrix[6] * Matrix[11] + Matrix[1] * Matrix[7] * Matrix[10] + Matrix[5] * Matrix[2] * Matrix[11]
		- Matrix[5] * Matrix[3] * Matrix[10] - Matrix[9] * Matrix[2] * Matrix[7] + Matrix[9] * Matrix[3] * Matrix[6];
		Inverse[7] =   Matrix[0] * Matrix[6] * Matrix[11] - Matrix[0] * Matrix[7] * Matrix[10] - Matrix[4] * Matrix[2] * Matrix[11]
		+ Matrix[4] * Matrix[3] * Matrix[10] + Matrix[8] * Matrix[2] * Matrix[7] - Matrix[8] * Matrix[3] * Matrix[6];
		Inverse[11] = -Matrix[0] * Matrix[5] * Matrix[11] + Matrix[0] * Matrix[7] * Matrix[9] + Matrix[4] * Matrix[1] * Matrix[11]
		- Matrix[4] * Matrix[3] * Matrix[9] - Matrix[8] * Matrix[1] * Matrix[7] + Matrix[8] * Matrix[3] * Matrix[5];
		Inverse[15] =  Matrix[0] * Matrix[5] * Matrix[10] - Matrix[0] * Matrix[6] * Matrix[9] - Matrix[4] * Matrix[1] * Matrix[10]
		+ Matrix[4] * Matrix[2] * Matrix[9] + Matrix[8] * Matrix[1] * Matrix[6] - Matrix[8] * Matrix[2] * Matrix[5];

		Det = Matrix[0]*Inverse[0] + Matrix[1]*Inverse[4] + Matrix[2]*Inverse[8] + Matrix[3]*Inverse[12];

		if( Det == 0.0f )
			return false;

		Det = 1.0f / Det;

		for( unsigned int i = 0; i < 16; ++i )
		{
			Matrix[i] = Inverse[i] * Det;
		}

		return true;
	}

#ifdef API_OPENGL
	void Matrix4::Load( MatrixType::Type Type ) const
	{
		glMatrixMode(GL_MODELVIEW + Type);
		glLoadIdentity();
		glLoadTransposeMatrixf((*this));
	}

	void Matrix4::LoadIdentity( MatrixType::Type Type )
	{
		glMatrixMode(GL_MODELVIEW + Type);
		glLoadIdentity();
	}
#endif

	float* Matrix4::AsArray( void ) const
	{
		return (float*)&Matrix;
	}
}
