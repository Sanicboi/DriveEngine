#pragma once
#include "Main.h"

class Camera
{
private:
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4 position;
	XMFLOAT4 target = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 up = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	float sensitivity = 0.1f;
	float speed = 5.0f;
	UINT width, height;
	float zN, zF;
	float fov;
public:
	Camera(float x = 0.0f, float y = 0.0f, float z = 3.0f, UINT w = 800, UINT h = 600, float f = 45.0f, float zNear = 0.1f, float zFar = 1000.0f);
	void Move(float x, float y, float z);
	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);
	void SetPosition(float x, float y, float z);
	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetProjectionMatrix() const;
	void Turn(float deltaX, float deltaY, float deltaZ);
	void UpdateMatrices();
	void Resize(UINT w, UINT h);
};

