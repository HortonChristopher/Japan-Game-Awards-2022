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
	//// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ��� Import texture for debug text
	//if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
	//	assert(0);
	//	return;
	//}

	//// �f�o�b�O�e�L�X�g������ Debug text initialization
	//debugText.Initialize(debugTextTexNumber);

	// �e�N�X�`���ǂݍ��� Texture loading
	if (!Sprite::LoadTexture(4, L"Resources/Sprite/Title.png")) {
		assert(0);
		return;
	}

	//�R���g���[���[������
	InitInput();

	// �w�i�X�v���C�g���� Background sprite generation
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
	// �w�i�X�v���C�g�`�� Background sprite drawing
	titleBG->Draw();
}
