#include "Camera.h"

Camera::Camera(float x, float y, float z, UINT w, UINT h, float f, float zNear, float zFar)
{
	position.x = x;
	position.y = y;
	position.z = z;
	position.w = 1.0f;
	width = w;
	height = h;
	zN = zNear;
	zF = zFar;
	fov = f;
	XMStoreFloat4x4(&projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(fov),(float)w/(float)h, zNear, zFar)));
	XMStoreFloat4x4(&view, XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat4(&position), XMLoadFloat4(&target), XMLoadFloat4(&up)) /** XMMatrixRotationRollPitchYaw(pitch, yaw, roll)*/));
}

void Camera::Move(float x, float y, float z)
{
	MoveX(x);
	MoveY(y);
	MoveZ(z);
}

void Camera::MoveX(float x)
{
	position.x += x;
}

void Camera::MoveY(float y)
{
	position.y += y;
}

void Camera::MoveZ(float z)
{
	position.z += z;
}

void Camera::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return view;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return projection;
}

void Camera::Turn(float deltaX, float deltaY, float deltaZ)
{
	pitch += sensitivity * deltaY;
	yaw += sensitivity * deltaX;
	roll += sensitivity * deltaZ;
}

void Camera::UpdateMatrices()
{
	XMStoreFloat4x4(&projection, XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), (float)width / (float)height, zN, zF)));
	XMStoreFloat4x4(&view, XMMatrixTranspose(XMMatrixLookAtLH(XMLoadFloat4(&position), XMLoadFloat4(&target), XMLoadFloat4(&up)) /** XMMatrixRotationRollPitchYaw(pitch, yaw, roll)*/));
}

void Camera::Resize(UINT w, UINT h)
{
	width = w;
	height = h;
	UpdateMatrices();
}
