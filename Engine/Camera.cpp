#include "pch.h"
#include "Camera.h"

# define M_PI 3.14159265358979323846

using namespace DirectX::SimpleMath;

Camera::Camera()
{
	m_orientation.x = 0.0f;		//rotation around x - pitch
	m_orientation.y = 0.0f;		//rotation around y - yaw
	m_orientation.z = 0.0f;		//rotation around z - roll

	m_position.x = 0.0f;		//camera position in space.
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_lookat.x = 0.0f;		//Look target point
	m_lookat.y = 0.0f;
	m_lookat.z = 0.0f;

	m_forward.x = 0.0f;		//forward/look direction
	m_forward.y = 0.0f;
	m_forward.z = 0.0f;

	m_right.x = 0.0f;
	m_right.y = 0.0f;
	m_right.z = 0.0f;

	m_up.x = 0.0f;
	m_up.y = 0.0f;
	m_up.z = 0.0f;

	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	Update();
}


Camera::~Camera()
{
}

void Camera::Update()
{
	//rotation in yaw
	float theta = m_orientation.y * 3.1415f / 180.0f;
	float phi = m_orientation.x * 3.1415f / 180.0f;

	m_forward.x = sin(theta) * sin(phi);
	m_forward.z = cos(theta) * sin(phi);
	m_forward.y = cos(phi);

	m_forward.Normalize();

	//create right vector from look Direction
	m_forward.Cross(Vector3::UnitY, m_right);

	//create up vector from look Direction
	m_forward.Cross(Vector3::UnitX, m_up);

	//update lookat point
	m_lookat = m_position + m_forward;

	//apply camera vectors and create camera matrix
	m_cameraMatrix = (Matrix::CreateLookAt(m_position, m_lookat, Vector3::UnitY));

}

Matrix Camera::getCameraMatrix()
{
	return m_cameraMatrix;
}

void Camera::setPosition(Vector3 newPosition)
{
	m_position = newPosition;
}

Vector3 Camera::getPosition()
{
	return m_position;
}

void Camera::setForward(Vector3 newForward)
{
	m_forward = newForward;
}

Vector3 Camera::getForward()
{
	return m_forward;
}

void Camera::setRotation(Vector3 newRotation)
{
	m_orientation = newRotation;
}

Vector3 Camera::getRotation()
{
	return m_orientation;
}

float Camera::getMoveSpeed()
{
	return m_movespeed;
}

float Camera::getRotationSpeed()
{
	return m_camRotRate;
}

Vector3 Camera::getRight()
{
	return m_right;
}

Vector3 Camera::getUp()
{
	return m_up;
}

void Camera::SetLookAtPos(Vector3 lookAtPos)
{
	if (
		lookAtPos.x == this->m_position.x &&
		lookAtPos.y == this->m_position.y &&
		lookAtPos.z == this->m_position.z
		)	return;

	lookAtPos.x = this->m_position.x - lookAtPos.x;
	lookAtPos.y = this->m_position.y - lookAtPos.y;
	lookAtPos.z = this->m_position.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += M_PI;

	this->setRotation(Vector3(pitch, yaw, 0.0f));
}