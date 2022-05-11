#include "GameOver.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void GameOver::Initialize()
{
	//// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ��� Import texture for debug text
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}

	//// �f�o�b�O�e�L�X�g������ Debug text initialization
	//debugText.Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ��� Texture loading
	if (!Sprite::LoadTexture(2, L"Resources/Sprite/GameOverBG.png")) {
		assert(0);
		return;
	}

	// �w�i�X�v���C�g���� Background sprite generation
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
	// �w�i�X�v���C�g�`�� Background sprite drawing
	gameOverBG->Draw();
}
