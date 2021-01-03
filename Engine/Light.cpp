// Light class
// Holds data that represents a single light source
#include "pch.h"
#include "light.h"

# define M_PI 3.14159265358979323846

Light::Light()
{
	m_ambientColour	=	DirectX::SimpleMath::Vector4(0.0f,0.0f,0.0f, 0.0f);
	m_diffuseColour	=	DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	m_position	=		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_lookAt	=		DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
};

Light::~Light()
{
}

void Light::setAmbientColour(float red, float green, float blue, float alpha)
{
	m_ambientColour = DirectX::SimpleMath::Vector4(red, green, blue, alpha);
}

void Light::setDiffuseColour(float red, float green, float blue, float alpha)
{
	m_diffuseColour = DirectX::SimpleMath::Vector4(red, green, blue, alpha);
}

void Light::setPosition(float x, float y, float z)
{
	m_position = XMVectorSet(x, y, z, 1.0f);
}

DirectX::SimpleMath::Vector4 Light::getAmbientColour()
{
	return m_ambientColour;
}

DirectX::SimpleMath::Vector4 Light::getDiffuseColour()
{
	return m_diffuseColour;
}

DirectX::SimpleMath::Vector3 Light::getPosition()
{
	return m_position;
}

void Light::setLookAt(float x, float y, float z)
{
	m_lookAt = DirectX::SimpleMath::Vector4(x, y, z, 1.0f);
}

void Light::generateViewMatrix()
{
	DirectX::SimpleMath::Vector3 up;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the view matrix from the three vectors.
	m_viewMatrix = XMMatrixLookAtLH(m_position, m_lookAt, up);
	return;
}

void Light::generateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)M_PI / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	 m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	 return;
}

DirectX::SimpleMath::Matrix Light::getViewMatrix()
{
	return m_viewMatrix;
}

DirectX::SimpleMath::Matrix Light::getProjectionMatrix()
{
	return m_projectionMatrix;
}