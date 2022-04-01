#pragma once

#include "Sprite.h"
#include "DirectXCommon.h"

/// <summary>
/// タイトルシーン Title Scene
/// </summary>
class GameOver
{
private: // 静的メンバ変数 Static member variables
	static const int debugTextTexNumber = 0;

public: // メンバ関数 Member function
	void Initialize();

	void Finalize();

	void Update();

	void Draw();

private: // メンバ変数 Member variables
	DirectXCommon* dxCommon = nullptr;

	Sprite* gameOverBG = nullptr;
	/*DebugText debugText;*/
};
