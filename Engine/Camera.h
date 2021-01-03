#pragma once
using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera();
	~Camera();

	void		Update();
	Matrix		getCameraMatrix();
	void		setPosition(Vector3 newPosition);
	Vector3		getPosition();
	void		setForward(Vector3 newForward);
	Vector3		getForward();
	void		setRotation(Vector3 newRotation);
	Vector3		getRotation();
	float		getMoveSpeed();
	float		getRotationSpeed();
	void 		SetLookAtPos(Vector3 lootAtPos);
	Vector3		getRight();
	Vector3		getUp();

private:
	Matrix	m_cameraMatrix;
	Vector3	m_lookat;
	Vector3	m_position;
	Vector3	m_forward;
	Vector3	m_right;
	Vector3	m_up;
	Vector3	m_orientation;

	float	m_movespeed ;
	float	m_camRotRate;

};

