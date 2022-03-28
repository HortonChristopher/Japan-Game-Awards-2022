#pragma once

#include "Sprite.h"
#include "DirectXCommon.h"

/// <summary>
/// �^�C�g���V�[�� Title Scene
/// </summary>
class TitleScene
{
private: // �ÓI�����o�ϐ� Static member variables
	static const int debugTextTexNumber = 0;

public: // �����o�֐� Member function
	void Initialize();

	void Finalize();

	void Update();

	void Draw();

private: // �����o�ϐ� Member variables
	DirectXCommon* dxCommon = nullptr;

	Sprite* titleBG = nullptr;
	/*DebugText debugText;*/
};

