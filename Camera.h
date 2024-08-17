#pragma once
#include "Main.h"

enum CAMERA_MOVEMENT {
	W = 1,
	S = 2,
	A = 3,
	D = 4
};

const XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
constexpr float ROTATION_GAIN = 0.4f;
constexpr float MOVEMENT_GAIN = 5.0f;

class Camera
{
private:
	float pitch;
	float yaw = XM_PI;
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
	XMFLOAT3 pos = { 0.0f, 0.0f, -10.0f };
	XMFLOAT3 front = { 0.0f, 0.0f, 1.0f };
	XMFLOAT3 left = { -1.0f, 0.0f, 0.0f };
	XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
public:
	Camera(uint16_t w = 0, uint16_t h = 0);
	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetProjectionMatrix() const;
	void Turn(float deltaX, float deltaY);
	void Move(float deltaT, CAMERA_MOVEMENT type);
	void Update();
	void UpdateVectors();
	void Resize(uint16_t w, uint16_t h);
};

