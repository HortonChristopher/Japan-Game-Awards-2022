#include "CinematicCamera.h"
#include "Camera.h"

CinematicCamera::XMFLOAT3 CinematicCamera::cinematicCamera(XMFLOAT3 startCameraPos, XMFLOAT3 playerPos, const float t)
{
	float x = startCameraPos.x * (1.0f - t);
	x += playerPos.x * t;
	float y = startCameraPos.y * (1.0f - t);
	y += playerPos.y * t;
	float z = startCameraPos.z * (1.0f - t);
	z += playerPos.z * t;

	XMFLOAT3 cameraPos{ x,y,z };

	return cameraPos;
}