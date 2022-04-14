#pragma once

#include <DirectXMath.h>

class CinematicCamera
{
protected: // エイリアス
// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	XMFLOAT3 cinematicCamera(XMFLOAT3 startCameraPos, XMFLOAT3 playerPos, const float t);
};