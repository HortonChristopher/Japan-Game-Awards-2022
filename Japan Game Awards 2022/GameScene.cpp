#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include "Collision.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include <sstream>
#include <iomanip>
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "Camera.h"
#include "Controller.h"
#include "TouchableObject.h"
#include "TitleScene.h"
#include "GameOver.h"
#include "GameClear.h"

using namespace DirectX;
extern int sceneNo = 0; //タイトル Title
extern int sceneChange = 0;

extern XMFLOAT3 playerPositionTemp = { 0,0,0 };
extern XMFLOAT3 playerRotationTemp = { 0,0,0 };

extern XMFLOAT3 clonePositionTemp = { 0,0,0 };
extern XMFLOAT3 cloneRotationTemp = { 0,0,0 };

extern int cameraMove = 1;
extern int prevCameraMove = 0;

//extern GameOver* gameOver = new GameOver();
//extern GameClear* gameClear = new GameClear();
extern DirectXCommon* dxCommon;

//あんじょうテスト
//よっしーテスト♡

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	for (auto object : objects) {
		safe_delete(object);
	}

	for (auto object_2 : objects_2) {
		safe_delete(object_2);
	}

	safe_delete(spriteBG);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objFighter);
	safe_delete(objClone);
	safe_delete(objTempTrigger);
	safe_delete(objTempTriggerE);
	safe_delete(objTempBullet);
	safe_delete(objTempBulletE);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelFighter);
	safe_delete(modelPlane);
	safe_delete(modelBox);
	safe_delete(modelPyramid);

	safe_delete(fbxobject1);
	//safe_delete( fbxmodel1 );
	safe_delete(objPlayerRun);
	safe_delete(modelPlayerRun);
	ReleaseInput();
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	//// nullptrチェック nullptr check
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	//コントローラー初期化
	InitInput();

	//// カメラ生成 Camera generation
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	collisionManager = CollisionManager::GetInstance();

	// カメラセット Camera set
	Object3d::SetCamera(camera);
	FbxObject3d::SetCamera(camera);

	// デバイスをセット Set the device
	FbxObject3d::SetDevice(dxCommon->GetDevice());

	// グラフィックスパイプライン生成 Graphics pipeline generation
	FbxObject3d::CreateGraphicsPipeline();

	//コントローラー初期化
	InitInput();

	// デバッグテキスト用テクスチャ読み込み Import texture for debug text
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化 Debug text initialization
	debugText.Initialize(debugTextTexNumber);

	//タイトルシーン初期化 Title Scene Initialization
	titleScene = new TitleScene();
	titleScene->Initialize();

	gameOver = new GameOver();
	gameOver->Initialize();

	gameClear = new GameClear();
	gameOver->Initialize();

	// テクスチャ読み込み Texture loading
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成 Background sprite generation
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// 3Dオブジェクト生成 3D object generation
	objSkydome = Object3d::Create();
	objTempTrigger = Object3d::Create();
	objTempTriggerE = Object3d::Create();
	objTempBullet = Object3d::Create();
	objTempBulletE = Object3d::Create();

	// テクスチャ2番に読み込み Load into texture # 2
	Sprite::LoadTexture(2, L"Resources/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");
	modelPlane = Model::CreateFromOBJ("yuka");
	modelBox = Model::CreateFromOBJ("box1x1x1");
	//modelPyramid = Model::CreateFromOBJ("pyramid1x1");
	modelTempWall = Model::CreateFromOBJ("kabe");
	modelTempTrigger = Model::CreateFromOBJ("TempTrigger");
	modelTempBullet = Model::CreateFromOBJ("bullet2");

	objSkydome->SetModel(modelSkydome);
	//objGround->SetModel( modelGround );
	//objFighter->SetModel( modelFighter );
	objTempTrigger->SetModel(modelTempTrigger);
	objTempTriggerE->SetModel(modelTempTrigger);
	objTempBullet->SetModel(modelTempBullet);
	objTempBulletE->SetModel(modelTempBullet);

	//objGround = TouchableObject::Create(modelGround);
	objFighter = Player::Create(modelFighter);
	objClone = Enemy::Create(modelFighter);

	// FBXモデルの読み込み Loading FBX model
	modelPlayerRun = FbxLoader::GetInstance()->LoadModelFromFile("Fast Run");
	modelPlayerStand = FbxLoader::GetInstance()->LoadModelFromFile("Standing W_Briefcase Idle");

	// FBX3Dオブジェクト生成とモデルとセット FBX3D object generation and model set
	// プレイヤー関連 Player related
	objPlayerRun = new FbxObject3d;
	objPlayerRun->Initialize();
	objPlayerRun->SetModel(modelPlayerRun);

	objPlayerStand = new FbxObject3d;
	objPlayerStand->Initialize();
	objPlayerStand->SetModel(modelPlayerStand);

	// クローン関連 Clone related
	objCloneRun = new FbxObject3d;
	objCloneRun->Initialize();
	objCloneRun->SetModel(modelPlayerRun);

	objCloneStand = new FbxObject3d;
	objCloneStand->Initialize();
	objCloneStand->SetModel(modelPlayerStand);

	// プレイヤー初期化 Player initialization
	objPlayerRun->SetPosition({ 0, 0, 0 });
	objPlayerRun->SetRotation({ 0, 0, 0 });
	objPlayerRun->SetScale({ 0.3, 0.3, 0.3 });

	objPlayerStand->SetPosition({ 0, 0, 0 });
	objPlayerStand->SetRotation({ 0, 0, 0 });
	objPlayerStand->SetScale({ 0.3, 0.3, 0.3 });

	// クローン初期化 Clone initialization
	objCloneRun->SetPosition({ 0, 0, 0 });
	objCloneRun->SetRotation({ 0, 0, 0 });
	objCloneRun->SetScale({ 0.3, 0.3, 0.3 });

	objCloneStand->SetPosition({ 0, 0, 0 });
	objCloneStand->SetRotation({ 0, 0, 0 });
	objCloneStand->SetScale({ 0.3, 0.3, 0.3 });

	// モデルテーブル Model table
	Model* modeltable[11] = {
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelTempWall,
	};

	Model* modeltable_2[11] = {
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelPlane,
		modelTempWall,
	};

	const int DIV_NUM = 10;
	const float LAND_SCALE = 3.0f;

	//ステージ1用外壁マップチップ
	const int WALL_NUM = 23;

	//自分側のマップチップ生成(Map chip generation)
	for (int i = 0; i < DIV_NUM; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 6; j++) { // x coordinate - Left to Right
			int modelIndex = rand() % 10;

			if (i == 2 && j == 2)
			{
				modelIndex = 10;
			}

			if (i == 5 && j == 3)
			{
				modelIndex = 10;
			}

			//ステージ外壁の配置
			//下
			if (i == 0 && j == j)
			{
				modelIndex = 10;
			}

			//上
			if (i == 9 && j == j)
			{
				modelIndex = 10;
			}

			//左
			if (i == i && j == 0)
			{
				modelIndex = 10;
			}

			//右
			if (i == i && j == 5)
			{
				modelIndex = 10;
			}

			if (i == 5 && j == 5)
			{
				modelIndex = 0;
			}

			TouchableObject* object = TouchableObject::Create(modeltable[modelIndex]);
			object->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (i == 9)
			{
				object->SetRotation({ 0, 180, 0 });
			}
			if (i == 2 && j == 2 || i == 5 && j == 3)
			{
				object->SetRotation({ -90, 0, 0 });
				object->SetScale({ LAND_SCALE, 2.0, LAND_SCALE });
			}
			objects.push_back(object);
		}
	}

	//敵側のマップチップ生成(Enemy side map chip generation)
	for (int i = 0; i < DIV_NUM; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 6; j++) { // x coordinate - Left to Right

			int modelIndex = rand() % 10;

			if (i == 5 && j == 3)
			{
				modelIndex = 10;
			}

			//ステージ外壁の配置
			//下
			if (i == 0 && j == j)
			{
				modelIndex = 10;
			}

			//上
			if (i == 9 && j == j)
			{
				modelIndex = 10;
			}

			//左
			if (i == i && j == 0)
			{
				modelIndex = 10;
			}

			//右
			if (i == i && j == 5)
			{
				modelIndex = 10;
			}

			if (i == 5 && j == 5)
			{
				modelIndex = 0;
			}


			TouchableObject* object_2 = TouchableObject::Create(modeltable_2[modelIndex]);
			object_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (i == 9)
			{
				object_2->SetRotation({ 0, 180, 0 });
			}
			if (i == 5 && j == 3)
			{
				object_2->SetRotation({ -90, 0, 0 });
				object_2->SetScale({ LAND_SCALE, 2.0, LAND_SCALE });
			}
			objects_2.push_back(object_2);
		}
	}

	//objFighter->SetPosition({ -10, 10, 0 });
	objFighter->SetScale({ 1,1,1 });
	objClone->SetScale({ 1,1,1 });

	objFighter->SetPosition({ -12,0,-12 });
	objClone->SetPosition({ 12,0,-12 });

	objTempTrigger->SetPosition({ -12.0f, 0, 0 });
	objTempTriggerE->SetPosition({ 12.0f, 0, 0 });

	objTempBullet->SetPosition({ -6.0f, 1.0f, 0 });
	objTempBulletE->SetPosition({ 6.0f, 1.0f, 0 });
	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	camera->SetTarget({ 0, 1, 0 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	enemyAlive = true;
	playerAlive = true;

	beginStage = false;
	currentFrame = 0;

	playerBulletF = false;
	enemyBulletF = false;
}

void GameScene::Update()
{
	UpdateInput();

	// 現在の座標を取得
	XMFLOAT3 FBXplayerPosition = playerPositionTemp;
	XMFLOAT3 FBXplayerRotation = playerRotationTemp;

	XMFLOAT3 FBXclonePosition = clonePositionTemp;
	XMFLOAT3 FBXcloneRotation = cloneRotationTemp;

	// 座標の変更を反映
	objPlayerRun->SetPosition(FBXplayerPosition);
	objPlayerRun->SetRotation(FBXplayerRotation);
	objPlayerStand->SetPosition(FBXplayerPosition);
	objPlayerStand->SetRotation(FBXplayerRotation);

	objCloneRun->SetPosition(FBXclonePosition);
	objCloneRun->SetRotation(FBXcloneRotation);
	objCloneStand->SetPosition(FBXclonePosition);
	objCloneStand->SetRotation(FBXcloneRotation);

	if (input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_W)
		|| IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::DownButton) || IsButtonPush(ButtonKind::UpButton))
	{
		FBXModelChange = 1;
	}
	else if (!input->PushKey(DIK_A) && !input->PushKey(DIK_S) && !input->PushKey(DIK_D) && !input->PushKey(DIK_W)
		|| IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::DownButton) || IsButtonPush(ButtonKind::UpButton))
	{
		FBXModelChange = 0;
	}

	/*if (playerBulletF)
	{
		playerBullet.x += 0.1f;
		objTempBullet->SetPosition(playerBullet);
	}
	else
	{
		playerBullet.x = InitBulletPos_PX;
		objTempBullet->SetPosition(playerBullet);
	}

	if (enemyBulletF)
	{
		enemyBullet.x -= 0.1f;
		objTempBulletE->SetPosition(enemyBullet);
	}
	else
	{
		enemyBullet.x = InitBulletPos_EX;
		objTempBulletE->SetPosition(enemyBullet);
	}

	if (playerBullet.x > 9.0f)
	{
		playerBulletF = false;
	}

	if (enemyBullet.x < -9.0f)
	{
		enemyBulletF = false;
	}*/

	//シーン遷移
	switch (sceneNo)
	{
	case 0:
		titleScene->Update();

		//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
		if (ConTimer <= 60)
		{
			ConTimer += 1;
		}

		if (ConTimer == 60)
		{
			InitInput();
			ConTimer = 0;
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNo = 1;
			titleScene->Finalize();
			break;
		}

		if (IsButtonDown(ButtonKind::Button_A))
		{
			sceneNo = 1;
			titleScene->Finalize();
			break;
		}


		break;
	case 1:
		playerPosition = objFighter->GetPosition();
		playerRotation = objFighter->GetRotation();
		enemyPosition = objClone->GetPosition();
		enemyRotation = objClone->GetRotation();
		playerTrigger = objTempTrigger->GetPosition();
		enemyTrigger = objTempTriggerE->GetPosition();
		playerBullet = objTempBullet->GetPosition();
		enemyBullet = objTempBulletE->GetPosition();

		// GameSceneとの座標共有 Coordinate sharing with GameScene
		playerPositionTemp = playerPosition;
		playerRotationTemp = playerRotation;
		clonePositionTemp = enemyPosition;
		cloneRotationTemp = enemyRotation;

		if (!beginStage)
		{
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				beginStage = true;
			}

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			// Camera Movement カメラ動く
			if (input->TriggerKey(DIK_Q) && cameraChange == false || input->TriggerKey(DIK_E) && cameraChange == false)
			{
				if (input->TriggerKey(DIK_Q))
				{
					if (cameraMove == 4)
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove = 1;
					}
					else
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove++;
					}
				}
				else if (input->TriggerKey(DIK_E))
				{
					if (cameraMove == 1)
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove = 4;
					}
					else
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove--;
					}
				}
			}

			if (IsButtonDown(ButtonKind::Button_LB) && cameraChange == false || IsButtonDown(ButtonKind::Button_RB) && cameraChange == false)
			{
				if (IsButtonDown(ButtonKind::Button_LB))
				{
					if (cameraMove == 4)
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove = 1;
					}
					else
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove++;
					}
				}
				else if (IsButtonDown(ButtonKind::Button_RB))
				{
					if (cameraMove == 1)
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove = 4;
					}
					else
					{
						prevCameraMove = cameraMove;
						cameraChange = true;
						cameraMove--;
					}
				}
			}

			// オブジェクト移動 Move object

			//if (IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::RightButton))
			//{
			//	if (IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			//	{
			//		//斜め移動の時は移動倍率を0.71に設定する
			//		rate = 0.71f;
			//	}
			//}

			//if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
			//{
			//	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
			//	{
			//		//斜め移動の時は移動倍率を0.71に設定する
			//		rate = 0.71f;
			//	}
			//}

			//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
			if (ConTimer <= 60)
			{
				ConTimer += 1;
			}

			if (ConTimer == 60)
			{
				InitInput();
				ConTimer = 0;
			}

			//オブジェクトの移動スピードは通常の移動スピードに移動倍率係数を掛ける
			move = Speed * rate;

			if (intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f) && lastIntersect == false)
			{
				playerBulletF = true;
			}

			if (intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f) && lastIntersectE == false)
			{
				enemyBulletF = true;
			}

			if (playerBulletF)
			{
				playerBullet.x += 0.1f;
				objTempBullet->SetPosition(playerBullet);
			}
			else
			{
				playerBullet.x = InitBulletPos_PX;
				objTempBullet->SetPosition(playerBullet);
			}

			if (enemyBulletF)
			{
				enemyBullet.x -= 0.1f;
				objTempBulletE->SetPosition(enemyBullet);
			}
			else
			{
				enemyBullet.x = InitBulletPos_EX;
				objTempBulletE->SetPosition(enemyBullet);
			}

			if (playerBullet.x > 9.0f)
			{
				playerBulletF = false;
			}

			if (enemyBullet.x < -9.0f)
			{
				enemyBulletF = false;
			}

			if (intersect(playerBullet, enemyPosition, 1.0f, 1.0f, 1.0f) && playerBulletF == true)
			{
				enemyAlive = false;
				sceneNo = 4;
				sceneChange = 0;
				//gameClear->Initialize();
			}

			if (intersect(enemyBullet, playerPosition, 1.0f, 1.0f, 1.0f) && enemyBulletF == true)
			{
				playerAlive = false;
				sceneNo = 3;
				sceneChange = 0;
				//gameOver->Initialize();
			}

			//Left Side Eye: {-40, 20, 0}
			//Right Side Eye: {40, 20, 0}
			//Normal Eye: {0, 20, -30}
			//Opposite Side: {0, 20, 30}

			if (cameraChange)
			{
				if (cameraMove == 1)
				{
					if (prevCameraMove == 4)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ -1.0f, 0.0f, -0.75f });
					}
					else if (prevCameraMove == 2)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ +1.0f, 0.0f, -0.75f });
					}

					if (currentCameraFrame >= 40)
					{
						camera->SetEye({ 0.0f, 20.0f, -30.0f });
						currentCameraFrame = 0;
						cameraChange = false;
					}
				}
				if (cameraMove == 2)
				{
					if (prevCameraMove == 1)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ -1.0f, 0.0f, +0.75f });
					}
					else if (prevCameraMove == 3)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ -1.0f, 0.0f, -0.75f });
					}

					if (currentCameraFrame >= 40)
					{
						camera->SetEye({ -40.0f, 20.0f, 0.0f });
						currentCameraFrame = 0;
						cameraChange = false;
					}
				}
				if (cameraMove == 3)
				{
					if (prevCameraMove == 2)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ +1.0f, 0.0f, +0.75f });
					}
					else if (prevCameraMove == 4)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ -1.0f, 0.0f, +0.75f });
					}

					if (currentCameraFrame >= 40)
					{
						camera->SetEye({ 0.0f, 20.0f, 30.0f });
						currentCameraFrame = 0;
						cameraChange = false;
					}
				}
				if (cameraMove == 4)
				{
					if (prevCameraMove == 3)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ +1.0f, 0.0f, -0.75f });
					}
					else if (prevCameraMove == 1)
					{
						currentCameraFrame++;
						camera->MoveEyeVector({ +1.0f, 0.0f, +0.75f });
					}

					if (currentCameraFrame >= 40)
					{
						camera->SetEye({ 40.0f, 20.0f, 0.0f });
						currentCameraFrame = 0;
						cameraChange = false;
					}
				}
			}

			UpdateInput();

			//objFighter->SetPosition({ playerPosition });

			camera->Update();
			//particleMan->Update();

			//objSkydome->Update();

			for (auto object : objects) {
				object->Update();
			}

			for (auto object_2 : objects_2) {
				object_2->Update();
			}

			//objGround->Update();
			objFighter->Update();
			objClone->Update();

			objTempTrigger->Update();
			objTempTriggerE->Update();

			objTempBullet->SetPosition(playerBullet);
			objTempBulletE->SetPosition(enemyBullet);

			if (playerBulletF == true)
			{
				objTempBullet->Update();
			}

			if (enemyBulletF == true)
			{
				objTempBulletE->Update();
			}

			//fbxobject1->Update();

			//debugText.Print( "", 50, 50, 1.0f );
			//debugText.Print( "WS: move camera UpDown", 50, 70, 1.0f );
			//debugText.Print( "ARROW: move camera FrontBack", 50, 90, 1.0f );

			lastIntersect = intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f);
			lastIntersectE = intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f);

			collisionManager->CheckAllCollisions();

			//Debug Start
			//XMFLOAT3 eye = camera->GetEye();

			//char msgbuf[256];
			//char msgbuf2[256];
			//char msgbuf3[256];

			//sprintf_s(msgbuf, 256, "Eye X: %f\n", eye.x);
			//sprintf_s(msgbuf2, 256, "Eye Y: %f\n", eye.y);
			//sprintf_s(msgbuf3, 256, "Eye Z: %f\n", eye.z);
			//OutputDebugStringA(msgbuf);
			//OutputDebugStringA(msgbuf2);
			//OutputDebugStringA(msgbuf3);
			//Debug End
		}

		for (auto object : objects) {
			object->Update();
		}

		for (auto object_2 : objects_2) {
			object_2->Update();
		}

		objPlayerRun->Update();
		objPlayerStand->Update();

		objCloneRun->Update();
		objCloneStand->Update();

		break;

	case 2:
		gameClear->Update();

		//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
		if (ConTimer <= 60)
		{
			ConTimer += 1;
		}

		if (ConTimer == 60)
		{
			InitInput();
			ConTimer = 0;
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNo = 0;
			gameClear->Finalize();
			titleScene->Initialize();
			break;
		}

		if (IsButtonUp(ButtonKind::Button_A))
		{
			sceneNo = 0;
			gameClear->Finalize();
			titleScene->Initialize();
			break;
		}

		break;
	case 3:
		gameOver->Update();

		//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
		if (ConTimer <= 60)
		{
			ConTimer += 1;
		}

		if (ConTimer == 60)
		{
			InitInput();
			ConTimer = 0;
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			sceneNo = 0;
			gameOver->Finalize();
			titleScene->Initialize();
			break;
		}

		if (IsButtonUp(ButtonKind::Button_A))
		{
			sceneNo = 0;
			gameOver->Finalize();
			titleScene->Initialize();
			break;
		}

		break;


	case 4:
	//ステージ2
		
		//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
		if (ConTimer <= 60)
		{
			ConTimer += 1;
		}

		if (ConTimer == 60)
		{
			InitInput();
			ConTimer = 0;
		}


		break;

	}

	//if (intersect(playerBullet, enemyPosition, 1.0f, 1.0f, 1.0f) && playerBulletF == true)
	//{
	//	enemyAlive = false;
	//}
	//if (intersect(enemyBullet, playerPosition, 1.0f, 1.0f, 1.0f) && enemyBulletF == true)
	//{
	//	playerAlive = false;
	//}


	// パーティクル生成 Particle generation
	//CreateParticles();

	//UpdateInput();

	//objFighter->SetPosition({ playerPosition });

	//camera->Update();
	//particleMan->Update();

	//objSkydome->Update();
	//for (auto object : objects) {
	//	object->Update();
	//}

	//for (auto object_2 : objects_2) {
	//	object_2->Update();
	//}

	//objGround->Update();
	/*objFighter->Update();
	objClone->Update();

	objTempTrigger->Update();
	objTempTriggerE->Update();

	objTempBullet->SetPosition(playerBullet);
	objTempBulletE->SetPosition(enemyBullet);

	objTempBullet->Update();
	objTempBulletE->Update();*/

	//fbxobject1->Update();

	//debugText.Print( "", 50, 50, 1.0f );
	//debugText.Print( "WS: move camera UpDown", 50, 70, 1.0f );
	//debugText.Print( "ARROW: move camera FrontBack", 50, 90, 1.0f );

	//lastIntersect = intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f);
	//lastIntersectE = intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f);

	//collisionManager->CheckAllCollisions();

	////Debug Start
	//char msgbuf[256];
	//char msgbuf2[256];
	////char msgbuf3[256];

	//sprintf_s(msgbuf, 256, "Enemy X: %f\n", enemyPosition.x);
	//sprintf_s(msgbuf2, 256, "Enemy Z: %f\n", enemyPosition.z);
	////sprintf_s(msgbuf3, 256, "isTouchingGround: %f\n", isTouchingGround);
	//OutputDebugStringA(msgbuf);
	//OutputDebugStringA(msgbuf2);
	////OutputDebugStringA(msgbuf3);
	////Debug End
}

void GameScene::Draw()
{
	// コマンドリストの取得 Get command list
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画 Background sprite drawing
	// 背景スプライト描画前処理 Background sprite drawing pre-processing
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画 Background sprite drawing
	//spriteBG->Draw();
	switch (sceneNo)
	{
	case 0:
		titleScene->Draw();
		break;
	case 1:
		spriteBG->Draw();
		break;
	case 2:
		if (sceneChange == 0)
		{
			Stage1Reset();
			gameClear->Initialize();
			sceneChange = 1;
		}
		gameClear->Draw();
		break;
	case 3:
		if (sceneChange == 0)
		{
			Stage1Reset();
			gameOver->Initialize();
			sceneChange = 1;
		}
		gameOver->Draw();
		break;
	}


	// ここに背景スプライトの描画処理を追加できる You can add background sprite drawing processing here

	// スプライト描画後処理 Post-processing of sprite drawing
	Sprite::PostDraw();
	// 深度バッファクリア Depth buffer clear
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画 3D object drawing
	// 3Dオブジェクト描画前処理 3D object drawing pre-processing
	Object3d::PreDraw(cmdList);

	switch (sceneNo)
	{
	case 0:
		break;
	case 1:
		// 3Dオブクジェクトの描画 Drawing 3D objects
		//objSkydome->Draw();
		//objGround->Draw();
		for (auto object : objects) {
			object->Draw();
		}

		for (auto object_2 : objects_2) {
			object_2->Draw();
		}

		//objFighter->Draw();
		//objClone->Draw();

		if (beginStage)
		{
			objTempTrigger->Draw();
			objTempTriggerE->Draw();

			if (playerBulletF == true)
			{
				objTempBullet->Draw();
			}

			if (enemyBulletF == true)
			{
				objTempBulletE->Draw();
			}
		}

		if (FBXModelChange == 1)
		{
			objPlayerRun->Draw(cmdList);
			objCloneRun->Draw(cmdList);
		}
		else if (FBXModelChange == 0)
		{
			objPlayerStand->Draw(cmdList);
			objCloneStand->Draw(cmdList);
		}
		break;
	case 2:
		break;
	case 3:
		break;
	}

	// パーティクルの描画 Drawing particles
	//if (IsButtonPush(ButtonKind::Button_A))
	//{
		//particleMan->Draw(cmdList);
	//}
	// ここに3Dオブジェクトの描画処理を追加できる You can add 3D object drawing process here

	// 3Dオブジェクト描画後処理 Post-processing of 3D object drawing
	Object3d::PostDraw();
#pragma endregion

	//#pragma region 前景スプライト描画 Foreground sprite drawing
	//	// 前景スプライト描画前処理 Foreground sprite drawing pre-processing
	//	Sprite::PreDraw( cmdList );
	//
	//	// ここに前景スプライトの描画処理を追加できる You can add foreground sprite drawing processing here
	//
	//	//// 描画 drawing
	//	//sprite1->Draw();
	//	//sprite2->Draw();

	switch (sceneNo)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}

	//	// デバッグテキストの描画 Debug text drawing
	//	// debugText.DrawAll(cmdList);
	//
	//	// スプライト描画後処理 Post-processing of sprite drawing
	//	Sprite::PostDraw();
	//#pragma endregion
}

void GameScene::MoveCamera()
{
	// カメラ移動 Camera movement
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { camera->MoveVector({ 0.0f,+0.06f,0.0f }); }
		else if (input->PushKey(DIK_S)) { camera->MoveVector({ 0.0f,-0.06f,0.0f }); }
		if (input->PushKey(DIK_D)) { camera->MoveVector({ +0.06f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { camera->MoveVector({ -0.06f,0.0f,0.0f }); }
	}

	// カメラ移動 Camera movement
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		if (input->PushKey(DIK_UP)) { camera->MoveVector({ 0.0f,0.0f,+0.06f }); }
		else if (input->PushKey(DIK_DOWN)) { camera->MoveVector({ 0.0f,0.0f,-0.06f }); }
	}
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布 All X, Y, Z are randomly distributed at [-5.0f, + 5.0f]
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加 addition
		//particleMan->Add( 60, pos, vel, acc, 1.0f, 0.0f );
	}
}

int GameScene::intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH)
{
	XMFLOAT2 circleDistance;

	circleDistance.x = abs(player.x - wall.x);
	circleDistance.y = abs(player.z - wall.z);

	if (circleDistance.x > (rectW / 2.0f + circleR)) { return false; }
	if (circleDistance.y > (rectH / 2.0f + circleR)) { return false; }

	if (circleDistance.x <= (rectW / 2.0f)) { return true; }
	if (circleDistance.y <= (rectH / 2.0f)) { return true; }

	float cornerDistance_sq = ((circleDistance.x - rectW / 2.0f) * (circleDistance.x - rectW / 2.0f)) + ((circleDistance.y - rectH / 2.0f) * (circleDistance.y - rectH / 2.0f));

	return (cornerDistance_sq <= (circleR * circleR));
}

void GameScene::Stage1Reset()
{
	objFighter->SetPosition({ -12,0,-12 });
	objClone->SetPosition({ 12,0,-12 });

	objTempTrigger->SetPosition({ -12.0f, 0, 0 });
	objTempTriggerE->SetPosition({ 12.0f, 0, 0 });

	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	//camera->SetTarget({ 0, 1, 0 });
	//camera->MoveEyeVector({ 0, 25.0f, 25.0f });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	playerBullet.x = InitBulletPos_PX;
	objTempBullet->SetPosition(playerBullet);
	objTempBullet->Update();

	enemyBullet.x = InitBulletPos_EX;
	objTempBulletE->SetPosition(enemyBullet);
	objTempBulletE->Update();

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	beginStage = false;
}

void GameScene::CinematicCamera()
{
	/*CinematicCamera cinematicCamera;
	if (!beginStage)
	{
		cameraFlag = true;

		maximumTime = 300;

		cameraStartPosition = { -100.0f, -100.0f, -100.0f };

		cameraEndPosition = { 0.0f, 0.0f, 0.0f };

		controlPoint = { cameraStartPosition.x - cameraEndPosition.x / 2.0f, cameraEndPosition.y + 500.0f, cameraEndPosition.z + 500.0f };
	}

	if (cameraFlag)
	{
		currentFrame++;

		timeRate = (float)currentFrame / (float)maximumTime;

		XMFLOAT3 Pos1 = cinematicCamera.cinematicCamera(cameraStartPosition, controlPoint, timeRate);
		XMFLOAT3 Pos2 = cinematicCamera.cinematicCamera(controlPoint, cameraEndPosition, timeRate);

		XMFLOAT3 cameraPos = cinematicCamera.cinematicCamera(Pos1, Pos2, timeRate);

		camera->SetEye(cameraPos);

		if (timeRate >= 1.0f)
		{
			currentFrame = 0.0f;
			cameraFlag = false;
			beginStage = true;
		}
	}*/
}