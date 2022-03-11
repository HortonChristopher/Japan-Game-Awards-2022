#include "Player.h"
#include "Input.h"
#include "Controller.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成 Instantiate a 3D object
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化 Initialize
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット Set model
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

	// コライダーの追加 Add Collider
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする Make the coordinates floating from your feet the center of the sphere by the radius
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0, 0 }), radius));

	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	// A,Dで旋回 Turn with A, D
	if (input->PushKey(DIK_A)) {
		rotation.y -= 2.0f;
	}
	else if (input->PushKey(DIK_D)) {
		rotation.y += 2.0f;
	}

	// 移動ベクトルをY軸回りの角度で回転 Rotate he movement vector at an angle around the Y axis
	XMVECTOR move = { 0, 0, 0.1f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	// 向いている方向に移動 Move in the direction you are facing
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

	// 行列の更新など Matrix update, etc
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	// デバッグテキスト表示 Debug text display
	DebugText::GetInstance()->Print("Collision detected.", 0.0f, 0.0f, 12.0f);

	// 衝突店にパーティクルを発生させる Generate particles at the collision point
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