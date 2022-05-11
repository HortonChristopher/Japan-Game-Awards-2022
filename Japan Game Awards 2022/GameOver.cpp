#include "GameOver.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void GameOver::Initialize()
{
	//// デバッグテキスト用テクスチャ読み込み Import texture for debug text
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}

	//// デバッグテキスト初期化 Debug text initialization
	//debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み Texture loading
	if (!Sprite::LoadTexture(2, L"Resources/Sprite/GameOverBG.png")) {
		assert(0);
		return;
	}

	// 背景スプライト生成 Background sprite generation
	gameOverBG = Sprite::Create(2, { 0.0f,0.0f });
}

void GameOver::Finalize()
{
	delete gameOverBG;
}

void GameOver::Update()
{

}

void GameOver::Draw()
{
	// 背景スプライト描画 Background sprite drawing
	gameOverBG->Draw();
}
