#include "TitleScene.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Controller.h"

extern int sceneNo;
extern int sceneChange;

void TitleScene::Initialize()
{
	//// デバッグテキスト用テクスチャ読み込み Import texture for debug text
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}

	//// デバッグテキスト初期化 Debug text initialization
	//debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み Texture loading
	if (!Sprite::LoadTexture(4, L"Resources/Sprite/Title.png")) {
		assert(0);
		return;
	}

	//コントローラー初期化
	InitInput();

	// 背景スプライト生成 Background sprite generation
	titleBG = Sprite::Create(4, { 0.0f,0.0f });
}

void TitleScene::Finalize()
{
	delete titleBG;
}

void TitleScene::Update()
{

}

void TitleScene::Draw()
{
	// 背景スプライト描画 Background sprite drawing
	titleBG->Draw();
}
