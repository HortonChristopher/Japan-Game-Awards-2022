#include "Player.h"
#include "Input.h"
#include "Controller.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "ParticleManager.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;

extern int cameraMove;
extern int prevCameraMove;
extern int playFlag;
extern int isStageGoing;

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

	//コントローラー初期化
	InitInput();

	// コライダーの追加 Add Collider
	float radius = 0.6f;
	// 半径分だけ足元から浮いた座標を球の中心にする Make the coordinates floating from your feet the center of the sphere by the radius
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0, 0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	UpdateInput();

#pragma region プレイヤーの移動処理
	if (isStageGoing == 0)
	{
		//正面から見たときの移動処理
		if (cameraMove == 1)
		{
			if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
			{
				if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_W))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A) && input->PushKey(DIK_S))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				else if (input->PushKey(DIK_D))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				else if (input->PushKey(DIK_W))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				else if (input->PushKey(DIK_S))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				playFlag = 1;
			}

			//コントローラー旋回と移動処理
			else if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				else if (IsButtonPush(ButtonKind::RightButton))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				else if (IsButtonPush(ButtonKind::UpButton))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				else if (IsButtonPush(ButtonKind::DownButton))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				playFlag = 1;
			}
			else
			{
				playFlag = 0;
			}
		}

		//左から見たときの移動処理
		if (cameraMove == 2)
		{
			//キーボードの移動処理
			if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
			{
				if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_W))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A) && input->PushKey(DIK_S))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				else if (input->PushKey(DIK_D))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				else if (input->PushKey(DIK_W))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				else if (input->PushKey(DIK_S))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				playFlag = 1;
			}

			//コントローラー旋回と移動処理
			else if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				else if (IsButtonPush(ButtonKind::RightButton))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				else if (IsButtonPush(ButtonKind::UpButton))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				else if (IsButtonPush(ButtonKind::DownButton))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				playFlag = 1;
			}
			else
			{
				playFlag = 0;
			}
		}

		//奥側から見たときの移動処理
		if (cameraMove == 3)
		{
			//キーボードの移動処理
			if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
			{
				if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_W))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A) && input->PushKey(DIK_S))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				else if (input->PushKey(DIK_D))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				else if (input->PushKey(DIK_W))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				else if (input->PushKey(DIK_S))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				playFlag = 1;
			}

			//コントローラー旋回と移動処理
			else if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				else if (IsButtonPush(ButtonKind::RightButton))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				else if (IsButtonPush(ButtonKind::UpButton))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				else if (IsButtonPush(ButtonKind::DownButton))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				playFlag = 1;
			}
			else
			{
				playFlag = 0;
			}
		}

		//右側から見たときの移動処理
		if (cameraMove == 4)
		{
			//キーボードの移動処理
			if (input->PushKey(DIK_A) || input->PushKey(DIK_D) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
			{
				if (input->PushKey(DIK_A) && input->PushKey(DIK_W))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_W))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_D) && input->PushKey(DIK_S))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A) && input->PushKey(DIK_S))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (input->PushKey(DIK_A))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				else if (input->PushKey(DIK_D))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				else if (input->PushKey(DIK_W))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				else if (input->PushKey(DIK_S))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				playFlag = 1;
			}

			//コントローラー旋回と移動処理
			else if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 225.0f;
					position.x -= Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
				{
					rotation.y = 315.0f;
					position.x -= Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 45.0f;
					position.x += Speed * 0.71f;
					position.z += Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
				{
					rotation.y = 135.0f;
					position.x += Speed * 0.71f;
					position.z -= Speed * 0.71f;
				}

				else if (IsButtonPush(ButtonKind::LeftButton))
				{
					position.z -= Speed;
					rotation.y = 180.0f;
				}

				else if (IsButtonPush(ButtonKind::RightButton))
				{
					position.z += Speed;
					rotation.y = 0.0f;
				}

				else if (IsButtonPush(ButtonKind::UpButton))
				{
					position.x -= Speed;
					rotation.y = 270.0f;
				}

				else if (IsButtonPush(ButtonKind::DownButton))
				{
					position.x += Speed;
					rotation.y = 90.0f;
				}

				playFlag = 1;
			}
			else
			{
				playFlag = 0;
			}
		}
	}
#pragma endregion

	// 移動ベクトルをY軸回りの角度で回転 Rotate the movement vector at an angle around the Y axis
	XMVECTOR move = { 0, 0, 0.1f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	// ワールド行列更新
	UpdateWorldMatrix();

	if (!onGround) {

		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;

		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	// ジャンプ操作 Jump operation
	/*else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 0.15f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}

	else if (IsButtonDown(ButtonKind::Button_A))
	{
		onGround = false;
		const float jumpVYFist = 0.15f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}*/
	
	// コライダー更新 Collider update
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	if (onGround) {
		
		const float adsDistance = 0.2f;

		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);

			Object3d::Update();
		}
		//
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {

			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);

			Object3d::Update();
		}
	}

	//落下したらプレイヤーの位置を初期値に戻す
	if (position.y <= -20)
	{
		position.x = -12.0f;
		position.y = 0.0f;
		position.z = -12.0f;

	}

	// 行列の更新など Matrix update, etc
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	// デバッグテキスト表示 Debug text display
	//DebugText::GetInstance()->Print("Collision detected.", 0.0f, 0.0f, 12.0f);

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