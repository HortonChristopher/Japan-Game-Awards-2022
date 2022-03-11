#include "Player.h"
#include "Input.h"
#include "Controller.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐� Instantiate a 3D object
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������ Initialize
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g Set model
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	// �R���C�_�[�̒ǉ� Add Collider
	float radius = 0.6f;
	// ���a�������������畂�������W�����̒��S�ɂ��� Make the coordinates floating from your feet the center of the sphere by the radius
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0, 0 }), radius));

	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	// A,D�Ő��� Turn with A, D
	if (input->PushKey(DIK_A)) {
		rotation.y -= 2.0f;
	}
	else if (input->PushKey(DIK_D)) {
		rotation.y += 2.0f;
	}

	// �ړ��x�N�g����Y�����̊p�x�ŉ�] Rotate he movement vector at an angle around the Y axis
	XMVECTOR move = { 0, 0, 0.1f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	// �����Ă�������Ɉړ� Move in the direction you are facing
	if (input->PushKey(DIK_S)) {
		position.x -= move.m128_f32[0];
		position.y -= move.m128_f32[1];
		position.z -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W)) {
		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
	}

	// �s��̍X�V�Ȃ� Matrix update, etc
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	// �f�o�b�O�e�L�X�g�\�� Debug text display
	DebugText::GetInstance()->Print("Collision detected.", 0.0f, 0.0f, 12.0f);

	// �Փ˓X�Ƀp�[�e�B�N���𔭐������� Generate particles at the collision point
	/*for (int i = 0; i < 1; ++i)
	{
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		ParticleManager::GetInstance()->Add(10, XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
	}*/
}