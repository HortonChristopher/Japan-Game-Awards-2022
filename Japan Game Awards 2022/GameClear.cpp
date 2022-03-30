#include "GameClear.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void GameClear::Initialize()
{
	//// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ��� Import texture for debug text
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}

	//// �f�o�b�O�e�L�X�g������ Debug text initialization
	//debugText.Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ��� Texture loading
	if (!Sprite::LoadTexture(1, L"Resources/GameClear.png")) {
		assert(0);
		return;
	}

	// �w�i�X�v���C�g���� Background sprite generation
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
	// �w�i�X�v���C�g�`�� Background sprite drawing
	gameClearBG->Draw();
}
