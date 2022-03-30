#include "GameClear.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void GameClear::Initialize()
{
	//// デバッグテキスト用テクスチャ読み込み Import texture for debug text
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}

	//// デバッグテキスト初期化 Debug text initialization
	//debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み Texture loading
	if (!Sprite::LoadTexture(1, L"Resources/GameClear.png")) {
		assert(0);
		return;
	}

	// 背景スプライト生成 Background sprite generation
	gameClearBG = Sprite::Create(1, { 0.0f,0.0f });
}

void GameClear::Finalize()
{
	delete gameClearBG;
}

void GameClear::Update()
{

}

void GameClear::Draw()
{
	// 背景スプライト描画 Background sprite drawing
	gameClearBG->Draw();
}
