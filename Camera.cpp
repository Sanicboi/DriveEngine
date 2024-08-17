#include "Camera.h"



Camera::Camera(uint16_t w, uint16_t h) : pitch(0.0f), yaw(90.0f)
{
	if (w > 0 && h > 0) {
		XMStoreFloat4x4(
			&proj,
			XMMatrixPerspectiveFovLH(
				XMConvertToRadians(70.0f),
				(float)w / (float)h,
				0.01f, 100.0f
			)
		);
		Update();
	}

}




XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return view;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return proj;
}

void Camera::Turn(float deltaX, float deltaY)
{
	pitch -= deltaY * ROTATION_GAIN;
	yaw -= deltaX * ROTATION_GAIN;
	UpdateVectors();
}

void Camera::Move(float deltaT, CAMERA_MOVEMENT type)
{
	XMFLOAT3 move;
	if (type == W)
		XMStoreFloat3(
			&move,
			XMLoadFloat3(&front)
		);
	else if (type == S)
		XMStoreFloat3(
			&move,
			XMLoadFloat3(&front) * -1
		);
	else if (type == A)
		XMStoreFloat3(
			&move,
			XMLoadFloat3(&left)
		);
	else if (type == D)
		XMStoreFloat3(
			&move,
			XMLoadFloat3(&left) * -1
		);


	XMStoreFloat3(
		&move,
		XMLoadFloat3(&move) * MOVEMENT_GAIN * deltaT
	);
	XMStoreFloat3(
		&pos,
		XMLoadFloat3(&pos) + XMLoadFloat3(&move)
	);
}

void Camera::Update()
{

	constexpr float limit = 89.99f;
	pitch = std::max(-limit, pitch);
	pitch = std::min(+limit, pitch);

	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}
	else if (yaw < -360.0f)
	{
		yaw += 360.0f;
	}

	UpdateVectors();

	XMVECTOR lookAt = XMLoadFloat3(&pos) + XMLoadFloat3(&front);

	XMStoreFloat4x4(
		&view,
		XMMatrixLookAtLH(XMLoadFloat3(&pos), lookAt, XMLoadFloat3(&up))
	);
}


void Camera::Resize(uint16_t w, uint16_t h) {
	XMStoreFloat4x4(
		&proj,
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(70.0f),
			(float)w / (float)h,
			0.01f, 100.0f
		)
	);
}


void Camera::UpdateVectors() {


	const float p = XMConvertToRadians(pitch);
	const float ya = XMConvertToRadians(yaw);

	float x = cosf(p) * cosf(ya);
	float y = sinf(p);
	float z = cosf(p) * sinf(ya);
	front = XMFLOAT3(x, y, z);
	XMStoreFloat3(
		&front,
		XMVector3Normalize(
			XMLoadFloat3(&front)
		)
	);


	XMFLOAT3 worldUp = { 0.0f, 1.0f, 0.0f };
	XMStoreFloat3(
		&left,
		XMVector3Normalize(
			XMVector3Cross(
				XMLoadFloat3(&front), XMLoadFloat3(&worldUp)
			)
		)
	);


	XMStoreFloat3(
		&up,
		XMVector3Normalize(
			XMVector3Cross(
				XMLoadFloat3(&left), XMLoadFloat3(&front)
			)
		)
	);

}