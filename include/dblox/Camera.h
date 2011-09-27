#ifndef DBLOX_CAMERA_H
#define DBLOX_CAMERA_H

#include "Math/Vector3.h"
#include "Math/Matrix4.h"

class Camera
{
	public:
		Camera( const Math::Vector3& Position, const Math::Vector3& Rotation, const float NearPlane, const float FarPlane, const float FoV, const float AspectRatio );
		~Camera();

		void Move( const Math::Vector3& Position );
		void Rotate( const Math::Vector3& Rotation );

		void SetPosition( const Math::Vector3& Position );
		Math::Vector3 GetPosition( void ) const;

		void SetRotation( const Math::Vector3& Rotation );
		Math::Vector3 GetRotation( void ) const;

		Math::Matrix4 GetProjectionMatrix( void ) const;
		Math::Matrix4 GetViewMatrix( void ) const;

		void Update( void );
		void Apply( void );

	protected:
		Math::Vector3 m_Position;
		Math::Vector3 m_Rotation;

		const float m_NearPlane;
		const float m_FarPlane;
		const float m_FoV;
		const float m_AspectRatio;

		Math::Matrix4 m_ProjMatrix;
		Math::Matrix4 m_ViewMatrix;
};

#endif
