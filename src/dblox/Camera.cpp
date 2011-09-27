#include "dblox/Camera.h"

Camera::Camera( const Math::Vector3& Position, const Math::Vector3& Rotation, const float NearPlane, const float FarPlane, const float FoV, const float AspectRatio )
	: m_Position(Position)
	, m_Rotation(Rotation)
	, m_NearPlane(NearPlane)
	, m_FarPlane(FarPlane)
	, m_FoV(FoV)
	, m_AspectRatio(AspectRatio)
{
	Update();
}

Camera::~Camera()
{
}

void Camera::Move( const Math::Vector3& Position )
{
	m_Position = m_Position + Position;
}

void Camera::Rotate( const Math::Vector3& Rotation )
{
	m_Rotation = m_Rotation + Rotation;
}

void Camera::SetPosition( const Math::Vector3& Position )
{
	m_Position = Position;
}

Math::Vector3 Camera::GetPosition( void ) const
{
	return m_Position;
}

void Camera::SetRotation( const Math::Vector3& Rotation )
{
	m_Rotation = Rotation;
}

Math::Vector3 Camera::GetRotation( void ) const
{
	return m_Rotation;
}

Math::Matrix4 Camera::GetProjectionMatrix( void ) const
{
	return m_ProjMatrix;
}

Math::Matrix4 Camera::GetViewMatrix( void ) const
{
	return m_ViewMatrix;
}

void Camera::Update( void )
{
	m_ProjMatrix = Math::Matrix4::Identity;
	m_ProjMatrix.Perspective(m_FoV, m_AspectRatio, m_NearPlane, m_FarPlane);

	m_ViewMatrix = Math::Matrix4::Identity;
	m_ViewMatrix.Rotate(m_Rotation);
	m_ViewMatrix.Translate(m_Position);
}

void Camera::Apply( void )
{
	m_ProjMatrix.Load(Math::MatrixType::Projection);
	m_ViewMatrix.Load(Math::MatrixType::ModelView);
}
