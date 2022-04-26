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
#include <vector>

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
	for (auto object_t1_1 : objects_t1_1)
	{
		safe_delete(object_t1_1);
	}

	for (auto object_t1_2 : objects_t1_2)
	{
		safe_delete(object_t1_2);
	}

	for (auto object_t2_1 : objects_t2_1)
	{
		safe_delete(object_t2_1);
	}

	for (auto object_t2_2 : objects_t2_2)
	{
		safe_delete(object_t2_2);
	}

	for (auto object_t3_1 : objects_t3_1)
	{
		safe_delete(object_t3_1);
	}

	for (auto object_t3_2 : objects_t3_2)
	{
		safe_delete(object_t3_2);
	}

	for (auto object_t3_y1_1 : objects_t3_y1_1)
	{
		safe_delete(object_t3_y1_1);
	}

	for (auto object_t3_y2_1 : objects_t3_y2_1)
	{
		safe_delete(object_t3_y2_1);
	}

	for (auto object : objects) {
		safe_delete(object);
	}

	for (auto object_2 : objects_2) {
		safe_delete(object_2);
	}

	for (auto object_s2_1 : objects_s2_1) {
		safe_delete(object_s2_1);
	}

	for (auto object_s2_2 : objects_s2_2) {
		safe_delete(object_s2_2);
	}

	for (auto object_s2_y : objects_s2_y) {
		safe_delete(object_s2_y);
	}

	for (auto object_s2_y2 : objects_s2_y2) {
		safe_delete(object_s2_y2);
	}

	for (auto object_s2_s : objects_s2_s) {
		safe_delete(object_s2_s);
	}

	for (auto object_s2_s2 : objects_s2_s2) {
		safe_delete(object_s2_s2);
	}

	safe_delete(spriteBG);
	//safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objFighter);
	safe_delete(objClone);
	safe_delete(objTempTrigger);
	safe_delete(objTempTriggerE);
	safe_delete(objTempYellowTrigger1);
	safe_delete(objTempBullet);
	safe_delete(objTempBulletE);
	//safe_delete(modelSkydome);
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
	//objSkydome = Object3d::Create();
	objTempTrigger = Object3d::Create();
	objTempTriggerE = Object3d::Create();
	objTempYellowTrigger1 = Object3d::Create();
	objTempYellowTrigger2 = Object3d::Create();
	objTempBullet = Object3d::Create();
	objTempBulletE = Object3d::Create();

	// テクスチャ2番に読み込み Load into texture # 2
	Sprite::LoadTexture(2, L"Resources/texture.png");

	//modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("kabe"); //chr_sword
	modelPlane = Model::CreateFromOBJ("yuka");
	modelBox = Model::CreateFromOBJ("box1x1x1");
	//modelPyramid = Model::CreateFromOBJ("pyramid1x1");
	modelTempWall = Model::CreateFromOBJ("kabe");
	modelYellowWall = Model::CreateFromOBJ("YellowKabe");
	modelTempTrigger = Model::CreateFromOBJ("TempTrigger");
	modelTempBullet = Model::CreateFromOBJ("bullet2");

	//objSkydome->SetModel(modelSkydome);
	//objGround->SetModel( modelGround );
	//objFighter->SetModel( modelFighter );
	objTempTrigger->SetModel(modelTempTrigger);
	objTempTriggerE->SetModel(modelTempTrigger);
	objTempYellowTrigger1->SetModel(modelTempTrigger);
	objTempYellowTrigger2->SetModel(modelTempTrigger);
	objTempBullet->SetModel(modelTempBullet);
	objTempBulletE->SetModel(modelTempBullet);

	//objGround = TouchableObject::Create(modelGround);
	objFighter = Player::Create(modelFighter);
	objClone = Enemy::Create(modelFighter);

	// FBXモデルの読み込み Loading FBX model
	modelPlayerRun = FbxLoader::GetInstance()->LoadModelFromFile("Running");
	modelPlayerStand = FbxLoader::GetInstance()->LoadModelFromFile("Standing");

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
	objPlayerRun->SetScale({ 0.5, 0.5, 0.5 });

	objPlayerStand->SetPosition({ 0, 0, 0 });
	objPlayerStand->SetRotation({ 0, 0, 0 });
	objPlayerStand->SetScale({ 0.5, 0.5, 0.5 });

	// クローン初期化 Clone initialization
	objCloneRun->SetPosition({ 0, 0, 0 });
	objCloneRun->SetRotation({ 0, 0, 0 });
	objCloneRun->SetScale({ 0.5, 0.5, 0.5 });

	objCloneStand->SetPosition({ 0, 0, 0 });
	objCloneStand->SetRotation({ 0, 0, 0 });
	objCloneStand->SetScale({ 0.5, 0.5, 0.5 });

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

	Model* modeltable_s2[2] = {
		modelPlane,
		modelTempWall,
	};

	Model* modeltable_y[1] = {
		modelYellowWall,
	};

	// チュートリアル 1
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i > 2 && i < 11 && j == 3 || i == 0 || i == 10 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			TouchableObject* object_t1_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t1_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_t1_1->SetRotation({ 0, 180, 0 });
			}
			objects_t1_1.push_back(object_t1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i > 2 && i < 11 && j == 3 || i == 0 || i == 10 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i < 10 && i > 7 && j < 6 && j > 3)
			{
				continue;
			}

			TouchableObject* object_t1_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t1_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t1_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_t1_2->SetRotation({ 0, 180, 0 });
			}
			objects_t1_2.push_back(object_t1_2);
		}
	}

	// チュートリアル 2
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 0 || i == 10 || j == 0 || j == 6 || i < 10 && i > 6 && j > 0 && j < 4 || i < 4 && j < 6 && j > 2 || i == 5 && j == 3)
			{
				modelIndex = 1;
			}

			if (i == 9 && j < 6 && j > 3 || i == 4 && j == 3)
			{
				continue;
			}

			TouchableObject* object_t2_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t2_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t2_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_t2_1->SetRotation({ 0, 180, 0 });
			}
			objects_t2_1.push_back(object_t2_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 0 || i == 10 || j == 0 || j == 6 || i < 10 && i > 6 && j > 0 && j < 4 || i < 4 && j < 6 && j > 2 || i == 5 && j == 3)
			{
				modelIndex = 1;
			}

			if (i == 9 && j < 6 && j > 3)
			{
				continue;
			}

			TouchableObject* object_t2_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t2_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_t2_2->SetRotation({ 0, 180, 0 });
			}
			objects_t2_2.push_back(object_t2_2);
		}
	}

	// チュートリアル 3
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 0 || i == 10 || j == 0 || j == 6 || i > 7 && i < 10 && j > 0 && j < 3 || i > 7 && i < 10 && j > 3 && j < 6 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6
				|| i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				modelIndex = 1;
			}

			TouchableObject* object_t3_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t3_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_t3_1->SetRotation({ 0, 180, 0 });
			}
			objects_t3_1.push_back(object_t3_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 0 || i == 10 || j == 0 || j == 6 || i > 7 && i < 10 && j > 0 && j < 3 || i > 7 && i < 10 && j > 3 && j < 6 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6
				|| i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				modelIndex = 1;
			}

			if (i == 9 && j == 3)
			{
				continue;
			}

			TouchableObject* object_t3_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t3_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_t3_2->SetRotation({ 0, 180, 0 });
			}
			objects_t3_2.push_back(object_t3_2);
		}
	}

	//黄色ブロック
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 2)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y1_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y1_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y1_1->SetRotation({ 0, 180, 0 });
			}
			objects_t3_y1_1.push_back(object_t3_y1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 2)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y2_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y2_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y2_1->SetRotation({ 0, 180, 0 });
			}
			objects_t3_y2_1.push_back(object_t3_y2_1);
		}
	}

	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 5)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y1_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y1_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y1_2->SetRotation({ 0, 180, 0 });
			}
			objects_t3_y1_2.push_back(object_t3_y1_2);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 5)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y2_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y2_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y2_2->SetRotation({ 0, 180, 0 });
			}
			objects_t3_y2_2.push_back(object_t3_y2_2);
		}
	}

	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 8)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y1_3 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y1_3->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_y1_3->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y1_3->SetRotation({ 0, 180, 0 });
			}
			objects_t3_y1_3.push_back(object_t3_y1_3);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 8)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y2_3 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y2_3->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t3_y2_3->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y2_3->SetRotation({ 0, 180, 0 });
			}
			objects_t3_y2_3.push_back(object_t3_y2_3);
		}
	}

	//自分側のマップチップ生成(Map chip generation) ステージ　１
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

			/*if (i == 5 && j == 5)
			{
				modelIndex = 0;
			}*/

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

	//敵側のマップチップ生成(Enemy side map chip generation)　ステージ　１
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

			/*if (i == 5 && j == 5)
			{
				modelIndex = 0;
			}*/


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

	//ステージ　２
	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex = 0;

			//ステージ外壁の配置
			//下
			if (i == 0 && j == j)
			{
				modelIndex = 1;
			}

			//上
			if (i == 11 && j == j)
			{
				modelIndex = 1;
			}

			//左
			if (i == i && j == 0)
			{
				modelIndex = 1;
			}

			//右
			if (i == i && j == 7)
			{
				modelIndex = 1;
			}

			if (i == 5 && j != 3 || i == 1 && j == 3 || i == 2 && j == 4 || i == 6 && j == 6 || i == 7 && j > 2 && j < 6 || i == 9 && j == 3 || i == 10 && j == 1)
			{
				modelIndex = 1;
			}

			if (i == 7 && j == 2 || i == 9 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s2_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s2_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s2_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (i == 11)
			{
				object_s2_1->SetRotation({ 0, 180, 0 });
			}
			objects_s2_1.push_back(object_s2_1);
		}
	}

	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex = 0;

			//ステージ外壁の配置
			//下
			if (i == 0 && j == j)
			{
				modelIndex = 1;
			}

			//上
			if (i == 11 && j == j)
			{
				modelIndex = 1;
			}

			//左
			if (i == i && j == 7)
			{
				modelIndex = 1;
			}

			//右
			if (i == i && j == 0)
			{
				modelIndex = 1;
			}

			if (i == 5 && j != 3 || i == 1 && j > 4 && j < 7 || i == 2 && j == 4 || i == 6 && j > 3 && j < 6 || i == 7 && j == 5 || i == 8 && j == 2 || i == 9 && j > 0 && j < 3 || i > 8 && i < 11 && j == 5)
			{
				modelIndex = 1;
			}

			if (i > 5 && i < 11 && j == 6)
			{
				continue;
			}

			TouchableObject* object_s2_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s2_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (i == 9)
			{
				object_s2_2->SetRotation({ 0, 180, 0 });
			}
			objects_s2_2.push_back(object_s2_2);
		}
	}

	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex;

			if (i == 5 && j == 3)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s2_y = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s2_y->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s2_y->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			objects_s2_y.push_back(object_s2_y);
		}
	}

	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex;

			if (i == 5 && j == 3)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s2_y2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s2_y2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s2_y2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			objects_s2_y2.push_back(object_s2_y2);
		}
	}

	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex;

			if (i == 7 && j == 2 || i == 9 && j == 5)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s2_s = TouchableObject::Create(modeltable[modelIndex]);
			object_s2_s->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s2_s->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			objects_s2_s.push_back(object_s2_s);
		}
	}

	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex;

			if (i > 5 && i < 11 && j == 6)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s2_s2 = TouchableObject::Create(modeltable[modelIndex]);
			object_s2_s2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s2_s2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			objects_s2_s2.push_back(object_s2_s2);
		}
	}

	//objFighter->SetPosition({ -10, 10, 0 });
	objFighter->SetScale({ 1,1,1 });
	objClone->SetScale({ 1,1,1 });

	objFighter->SetPosition({ -20, 0, 12 }); // -20, 12
	objClone->SetPosition({ 20, 0, 12 }); // 20, 12

	objTempTrigger->SetPosition({ -8.0f, 0, 6.0f }); // -12, 0
	objTempTriggerE->SetPosition({ 8.0f, 0, 6.0f }); // 12 0
	objTempYellowTrigger1->SetPosition({ 20.0f, 0, -3.0f });
	objTempYellowTrigger2->SetPosition({ 8.0f, 0, -12.0f });

	objTempBullet->SetPosition({ -6.0f, 1.0f, 0 });
	objTempBulletE->SetPosition({ 6.0f, 1.0f, 0 });
	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	camera->SetEye({0, 20, -30});

	camera->SetTarget({ 0, 1, 0 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	originalCamera = camera->GetEye();

	camera->Update();

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

	playerPosition = objFighter->GetPosition();
	playerRotation = objFighter->GetRotation();
	enemyPosition = objClone->GetPosition();
	enemyRotation = objClone->GetRotation();
	playerTrigger = objTempTrigger->GetPosition();
	enemyTrigger = objTempTriggerE->GetPosition();
	yellowTrigger1 = objTempYellowTrigger1->GetPosition();
	yellowTrigger2 = objTempYellowTrigger2->GetPosition();
	playerBullet = objTempBullet->GetPosition();
	enemyBullet = objTempBulletE->GetPosition();

	// GameSceneとの座標共有 Coordinate sharing with GameScene
	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	if (beginStage)
	{
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
	}
	else
	{
		FBXModelChange = 0;
		cloneRotationTemp = { 0,0,0 };
		playerRotationTemp = { 0,0,0 };
	}

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
			//InitInput();
			ConTimer = 0;
		}

		if (input->TriggerKey(DIK_SPACE))
		{
			if (!t1Time)
			{
				Tutorial1Reset();
			}
			sceneNo = 5;
			titleScene->Finalize();
			break;
		}

		if (IsButtonDown(ButtonKind::Button_A))
		{
			if (!t1Time)
			{
				Tutorial1Reset();
			}
			sceneNo = 5;
			titleScene->Finalize();
			break;
		}

		break;
	case 1:
		if (!beginStage)
		{
			if (firstTime)
			{
				firstTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				beginStage = true;
			}

			objFighter->SetPosition({ -12,0,-12 });
			objClone->SetPosition({ 12,0,-12 });

			playerPositionTemp = { -12,0,-12 };
			clonePositionTemp = { 12,0,-12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
			if (ConTimer <= 60)
			{
				ConTimer += 1;
			}

			if (ConTimer == 60)
			{
				//InitInput();
				ConTimer = 0;
			}

			//オブジェクトの移動スピードは通常の移動スピードに移動倍率係数を掛ける
			//move = Speed * rate;

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
				playerBullet.x += 1.0f;
				objTempBullet->SetPosition(playerBullet);
			}
			else
			{
				playerBullet.x = InitBulletPos_PX;
				objTempBullet->SetPosition(playerBullet);
			}

			if (enemyBulletF)
			{
				enemyBullet.x -= 1.0f;
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
				Stage2Reset();
				Stage1Move();
				sceneNo = 4;
				sceneChange = 0;
			}

			if (intersect(enemyBullet, playerPosition, 1.0f, 1.0f, 1.0f) && enemyBulletF == true)
			{
				playerAlive = false;
				Stage1Move();
				sceneNo = 3;
				sceneChange = 0;
			}

			//UpdateInput();

			//objFighter->SetPosition({ playerPosition });

			camera->Update();
			//particleMan->Update();

			//objSkydome->Update();

			/*for (auto object : objects) {
				object->Update();
			}

			for (auto object_2 : objects_2) {
				object_2->Update();
			}*/

			//objGround->Update();

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

			lastIntersect = intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f);
			lastIntersectE = intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f);

			collisionManager->CheckAllCollisions();
		}

		for (auto object : objects) {
			object->Update();
		}

		for (auto object_2 : objects_2) {
			object_2->Update();
		}

		objTempTrigger->Update();
		objTempTriggerE->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();

		objCloneRun->Update();
		objCloneStand->Update();

		objFighter->Update();
		objClone->Update();

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
			//InitInput();
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
			//InitInput();
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
		if (!beginStage)
		{
			if (secondTime)
			{
				secondTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				beginStage = true;
			}

			objFighter->SetPosition({ -24,0,-12 });
			objClone->SetPosition({ 24,0,-12 });

			playerPositionTemp = { -24,0,-12 };
			clonePositionTemp = { 24,0,-12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
			if (ConTimer <= 60)
			{
				ConTimer += 1;
			}

			if (ConTimer == 60)
			{
				//InitInput();
				ConTimer = 0;
			}

			if (intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f))
			{
				stage2YellowKabe = false;
			}
			if (intersect(playerPosition, yellowTrigger1, 1.0f, 1.0f, 1.0f) && lastYellowIntersct1 == false)
			{
				stage2Switch = true;
			}

			if (playerPosition.y <= -0.5f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage2Move();
				sceneChange = 0;
				gameOver->Initialize();
			}
			else if (enemyPosition.y <= -0.5f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage2Move();
				sceneChange = 0;
				gameClear->Initialize();
			}

			lastYellowIntersct1 = intersect(playerPosition, yellowTrigger1, 1.0f, 1.0f, 1.0f);

			collisionManager->CheckAllCollisions();
		}

		for (auto object_s2_1 : objects_s2_1) {
			object_s2_1->Update();
		}

		for (auto object_s2_2 : objects_s2_2) {
			object_s2_2->Update();
		}

		if (!stage2YellowKabe)
		{
			for (auto object_s2_y : objects_s2_y) {
				XMFLOAT3 position = object_s2_y->GetPosition();
				object_s2_y->SetPosition({ position.x, -100, position.z });
				object_s2_y->Update();
			}

			for (auto object_s2_y2 : objects_s2_y2) {
				XMFLOAT3 position = object_s2_y2->GetPosition();
				object_s2_y2->SetPosition({ position.x, -100, position.z });
				object_s2_y2->Update();
			}
		}

		if (stage2Switch)
		{
			for (auto object_s2_s : objects_s2_s) {
				XMFLOAT3 position = object_s2_s->GetPosition();
				object_s2_s->SetPosition({ position.x, -100, position.z });
				object_s2_s->Update();
			}

			for (auto object_s2_s2 : objects_s2_s2) {
				XMFLOAT3 position = object_s2_s2->GetPosition();
				object_s2_s2->SetPosition({ position.x, -100, position.z });
				object_s2_s2->Update();
			}
		}

		for (auto object_s2_y : objects_s2_y) {
			object_s2_y->Update();
		}

		for (auto object_s2_y2 : objects_s2_y2) {
			object_s2_y2->Update();
		}

		for (auto object_s2_s : objects_s2_s) {
			object_s2_s->Update();
		}

		for (auto object_s2_s2 : objects_s2_s2) {
			object_s2_s2->Update();
		}

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();

		objCloneRun->Update();
		objCloneStand->Update();

		objTempTrigger->Update();
		objTempTriggerE->Update();
		objTempYellowTrigger1->Update();

		camera->Update();

		break;

	case 5:
	//チュートリアル 1
		if (!beginStage)
		{
			if (t1Time)
			{
				t1Time = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				beginStage = true;
			}

			objFighter->SetPosition({ -20,0,12 });
			objClone->SetPosition({ 20,0,12 });

			playerPositionTemp = { -20,0,12 };
			clonePositionTemp = { 20,0,12 };

			//objFighter->Update();
			//objClone->Update();
		}
		if (beginStage)
		{
			//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
			if (ConTimer <= 60)
			{
				ConTimer += 1;
			}

			if (ConTimer == 60)
			{
				//InitInput();
				ConTimer = 0;
			}

			if (playerPosition.y <= -2.5f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial1Move();
				//Tutorial1Reset();
				sceneChange = 0;
				gameOver->Initialize();
			}
			else if (enemyPosition.y <= -2.5f)
			{
				enemyAlive = false;
				sceneNo = 6;
				Tutorial1Move();
				Tutorial2Reset();
				sceneChange = 0;
			}
		}
		
		for (auto object_t1_1 : objects_t1_1) {
			object_t1_1->Update();
		}

		for (auto object_t1_2 : objects_t1_2) {
			object_t1_2->Update();
		}

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();

		objCloneRun->Update();
		objCloneStand->Update();

		camera->Update();

		break;

	case 6:
	//チュートリアル 2
		if (!beginStage)
		{
			if (t2Time)
			{
				t2Time = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				beginStage = true;
			}

			objFighter->SetPosition({ -20, 0,-12 });
			objClone->SetPosition({ 20, 0, -12 });

			playerPositionTemp = { -20, 0,-12 };
			clonePositionTemp = { 20, 0, -12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
			if (ConTimer <= 60)
			{
				ConTimer += 1;
			}

			if (ConTimer == 60)
			{
				//InitInput();
				ConTimer = 0;
			}

			if (playerPosition.y <= -2.5f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial2Move();
				//Tutorial1Reset();
				sceneChange = 0;
				gameOver->Initialize();
			}
			else if (enemyPosition.y <= -2.5f)
			{
				enemyAlive = false;
				sceneNo = 7;
				Tutorial2Move();
				Tutorial3Reset();
				sceneChange = 0;
			}
		}

		for (auto object_t2_1 : objects_t2_1)
		{
			object_t2_1->Update();
		}

		for (auto object_t2_2 : objects_t2_2)
		{
			object_t2_2->Update();
		}

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		
		camera->Update();

		break;

	case 7:
	//チュートリアル 3
		if (!beginStage)
		{
			if (t3Time)
			{
				t3Time = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				beginStage = true;
			}

			objFighter->SetPosition({ -20, 0, -12 });
			objClone->SetPosition({ 20, 0, -12 });

			playerPositionTemp = { -20, 0, -12 };
			clonePositionTemp = { 20, 0, -12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			//コントローラーが接続されていなかったら60フレーム毎にコントローラーをさがす
			if (ConTimer <= 60)
			{
				ConTimer += 1;
			}

			if (ConTimer == 60)
			{
				//InitInput();
				ConTimer = 0;
			}

			if (intersect(enemyPosition, yellowTrigger2, 1.0f, 1.0f, 1.0f))
			{
				tutorial3YellowKabe1 = false;
			}
			if (intersect(enemyPosition, yellowTrigger1, 1.0f, 1.0f, 1.0f))
			{
				tutorial3YellowKabe2 = false;
			}
			if (intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f))
			{
				tutorial3YellowKabe3 = false;
			}

			if (!tutorial3YellowKabe1)
			{
				for (auto object_t3_y1_1 : objects_t3_y1_1) {
					XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
					object_t3_y1_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t3_y1_1->Update();
				}

				for (auto object_t3_y2_1 : objects_t3_y2_1) {
					XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
					object_t3_y2_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t3_y2_1->Update();
				}
			}
			if (!tutorial3YellowKabe2)
			{
				for (auto object_t3_y1_2 : objects_t3_y1_2) {
					XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
					object_t3_y1_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t3_y1_2->Update();
				}

				for (auto object_t3_y2_2 : objects_t3_y2_2) {
					XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
					object_t3_y2_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t3_y2_2->Update();
				}
			}
			if (!tutorial3YellowKabe3)
			{
				for (auto object_t3_y1_3 : objects_t3_y1_3) {
					XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
					object_t3_y1_3->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t3_y1_3->Update();
				}

				for (auto object_t3_y2_3 : objects_t3_y2_3) {
					XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
					object_t3_y2_3->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t3_y2_3->Update();
				}
			}

			if (playerPosition.y <= -2.5f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial3Move();
				//Tutorial1Reset();
				sceneChange = 0;
				gameOver->Initialize();
			}
			else if (enemyPosition.y <= -2.5f)
			{
				enemyAlive = false;
				sceneNo = 1;
				Tutorial3Move();
				Stage1Reset();
				sceneChange = 0;
			}
		}

		for (auto object_t3_1 : objects_t3_1)
		{
			object_t3_1->Update();
		}

		for (auto object_t3_2 : objects_t3_2)
		{
			object_t3_2->Update();
		}

		for (auto object_t3_y1_1 : objects_t3_y1_1)
		{
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1)
		{
			object_t3_y2_1->Update();
		}

		for (auto object_t3_y1_2 : objects_t3_y1_2)
		{
			object_t3_y1_2->Update();
		}

		for (auto object_t3_y2_2 : objects_t3_y2_2)
		{
			object_t3_y2_2->Update();
		}

		for (auto object_t3_y1_3 : objects_t3_y1_3)
		{
			object_t3_y1_3->Update();
		}

		for (auto object_t3_y2_3 : objects_t3_y2_3)
		{
			object_t3_y2_3->Update();
		}

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();

		objCloneRun->Update();
		objCloneStand->Update();

		objTempTrigger->Update();
		objTempTriggerE->Update();
		objTempYellowTrigger1->Update();
		objTempYellowTrigger2->Update();

		camera->Update();

		break;
	}

	//Debug Start
	/*XMFLOAT3 eye = camera->GetEye();

	char msgbuf[256];
	char msgbuf2[256];
	char msgbuf3[256];

	sprintf_s(msgbuf, 256, "Player X: %f\n", playerPosition.x);
	sprintf_s(msgbuf2, 256, "Player Y: %f\n", playerPosition.y);
	sprintf_s(msgbuf3, 256, "Player Z: %f\n", playerPosition.z);
	OutputDebugStringA(msgbuf);
	OutputDebugStringA(msgbuf2);
	OutputDebugStringA(msgbuf3);*/
	//Debug End

	//Left Side Eye: {-40, 20, 0}
	//Right Side Eye: {40, 20, 0}
	//Normal Eye: {0, 20, -30}
	//Opposite Side: {0, 20, 30}
	//カメラ回転 Camera Rotation
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
	case 4:
		spriteBG->Draw();
		break;
	case 5:
		spriteBG->Draw();
		break;
	case 6:
		spriteBG->Draw();
		break;
	case 7:
		spriteBG->Draw();
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
	case 4:
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

		for (auto object_s2_1 : objects_s2_1) {
			object_s2_1->Draw();
		}

		for (auto object_s2_2 : objects_s2_2) {
			object_s2_2->Draw();
		}

		if (stage2YellowKabe)
		{
			objTempYellowTrigger1->Draw();

			for (auto object_s2_y : objects_s2_y) {
				object_s2_y->Draw();
			}

			for (auto object_s2_y2 : objects_s2_y2) {
				object_s2_y2->Draw();
			}
		}

		if (!stage2Switch)
		{
			objTempTrigger->Draw();
			objTempTriggerE->Draw();

			for (auto object_s2_s : objects_s2_s) {
				object_s2_s->Draw();
			}

			for (auto object_s2_s2 : objects_s2_s2) {
				object_s2_s2->Draw();
			}
		}

		break;
	case 5:
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

		for (auto object_t1_1 : objects_t1_1) {
			object_t1_1->Draw();
		}

		for (auto object_t1_2 : objects_t1_2) {
			object_t1_2->Draw();
		}

		break;
	case 6:
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

		for (auto object_t2_1 : objects_t2_1)
		{
			object_t2_1->Draw();
		}

		for (auto object_t2_2 : objects_t2_2)
		{
			object_t2_2->Draw();
		}

		break;
	case 7:
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

		for (auto object_t3_1 : objects_t3_1)
		{
			object_t3_1->Draw();
		}

		for (auto object_t3_2 : objects_t3_2)
		{
			object_t3_2->Draw();
		}

		if (tutorial3YellowKabe1)
		{
			objTempYellowTrigger2->Draw();

			for (auto object_t3_y1_1 : objects_t3_y1_1)
			{
				object_t3_y1_1->Draw();
			}

			for (auto object_t3_y2_1 : objects_t3_y2_1)
			{
				object_t3_y2_1->Draw();
			}
		}
		if (tutorial3YellowKabe2)
		{
			objTempYellowTrigger1->Draw();

			for (auto object_t3_y1_2 : objects_t3_y1_2)
			{
				object_t3_y1_2->Draw();
			}

			for (auto object_t3_y2_2 : objects_t3_y2_2)
			{
				object_t3_y2_2->Draw();
			}
		}
		if (tutorial3YellowKabe3)
		{
			objTempTrigger->Draw();
			objTempTriggerE->Draw();

			for (auto object_t3_y1_3 : objects_t3_y1_3)
			{
				object_t3_y1_3->Draw();
			}

			for (auto object_t3_y2_3 : objects_t3_y2_3)
			{
				object_t3_y2_3->Draw();
			}
		}

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
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
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

void GameScene::Tutorial1Reset()
{
	if (!t1Time)
	{
		for (auto object_t1_1 : objects_t1_1) {
			XMFLOAT3 objectPosition = object_t1_1->GetPosition();
			object_t1_1->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t1_1->Update();
		}

		for (auto object_t1_2 : objects_t1_2) {
			XMFLOAT3 objectPosition = object_t1_2->GetPosition();
			object_t1_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t1_2->Update();
		}
	}

	objFighter->SetPosition({ -20,0,12 });
	objClone->SetPosition({ 20,0,12 });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	cameraMove = 1;

	beginStage = false;
}

void GameScene::Tutorial1Move()
{
	for (auto object_t1_1 : objects_t1_1) {
		XMFLOAT3 objectPosition = object_t1_1->GetPosition();
		object_t1_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t1_1->Update();
	}

	for (auto object_t1_2 : objects_t1_2) {
		XMFLOAT3 objectPosition = object_t1_2->GetPosition();
		object_t1_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t1_2->Update();
	}
}

void GameScene::Tutorial2Reset()
{
	if (!t2Time)
	{
		for (auto object_t2_1 : objects_t2_1) {
			XMFLOAT3 objectPosition = object_t2_1->GetPosition();
			object_t2_1->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t2_1->Update();
		}

		for (auto object_t2_2 : objects_t2_2) {
			XMFLOAT3 objectPosition = object_t2_2->GetPosition();
			object_t2_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t2_2->Update();
		}
	}

	objFighter->SetPosition({ -20, 0,-12 });
	objClone->SetPosition({ 20, 0, -12 });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	cameraMove = 1;

	beginStage = false;
}

void GameScene::Tutorial2Move()
{
	for (auto object_t2_1 : objects_t2_1) {
		XMFLOAT3 objectPosition = object_t2_1->GetPosition();
		object_t2_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t2_1->Update();
	}

	for (auto object_t2_2 : objects_t2_2) {
		XMFLOAT3 objectPosition = object_t2_2->GetPosition();
		object_t2_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t2_2->Update();
	}
}

void GameScene::Tutorial3Reset()
{
	if (!t3Time)
	{
		for (auto object_t3_1 : objects_t3_1) {
			XMFLOAT3 objectPosition = object_t3_1->GetPosition();
			object_t3_1->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t3_1->Update();
		}

		for (auto object_t3_2 : objects_t3_2) {
			XMFLOAT3 objectPosition = object_t3_2->GetPosition();
			object_t3_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t3_2->Update();
		}

		for (auto object_t3_y1_1 : objects_t3_y1_1) {
			XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
			object_t3_y1_1->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1) {
			XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
			object_t3_y2_1->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t3_y2_1->Update();
		}

		for (auto object_t3_y1_2 : objects_t3_y1_2) {
			XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
			object_t3_y1_2->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t3_y1_2->Update();
		}

		for (auto object_t3_y2_2 : objects_t3_y2_2) {
			XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
			object_t3_y2_2->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t3_y2_2->Update();
		}

		for (auto object_t3_y1_3 : objects_t3_y1_3) {
			XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
			object_t3_y1_3->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t3_y1_3->Update();
		}

		for (auto object_t3_y2_3 : objects_t3_y2_3) {
			XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
			object_t3_y2_3->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t3_y2_3->Update();
		}
	}

	objFighter->SetPosition({ -20, 0, -12 });
	objClone->SetPosition({ 20, 0, -12 });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	tutorial3YellowKabe1 = true;
	tutorial3YellowKabe2 = true;
	tutorial3YellowKabe3 = true;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	objTempTrigger->SetPosition({ -8.0f, 0, 6.0f });
	objTempTriggerE->SetPosition({ 8.0f, 0, 6.0f });
	objTempYellowTrigger1->SetPosition({ 20.0f, 0, -3.0f });
	objTempYellowTrigger2->SetPosition({ 8.0f, 0, -12.0f });

	cameraMove = 1;

	beginStage = false;
}

void GameScene::Tutorial3Move()
{
	for (auto object_t3_1 : objects_t3_1) {
		XMFLOAT3 objectPosition = object_t3_1->GetPosition();
		object_t3_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_1->Update();
	}

	for (auto object_t3_2 : objects_t3_2) {
		XMFLOAT3 objectPosition = object_t3_2->GetPosition();
		object_t3_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_2->Update();
	}

	for (auto object_t3_y1_1 : objects_t3_y1_1) {
		XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
		object_t3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_y1_1->Update();
	}

	for (auto object_t3_y2_1 : objects_t3_y2_1) {
		XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
		object_t3_y2_1->SetPosition({ objectPosition.x, objectPosition.y -50.0f, objectPosition.z });
		object_t3_y2_1->Update();
	}

	for (auto object_t3_y1_2 : objects_t3_y1_2) {
		XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
		object_t3_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_y1_2->Update();
	}

	for (auto object_t3_y2_2 : objects_t3_y2_2) {
		XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
		object_t3_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_y2_2->Update();
	}

	for (auto object_t3_y1_3 : objects_t3_y1_3) {
		XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
		object_t3_y1_3->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_y1_3->Update();
	}

	for (auto object_t3_y2_3 : objects_t3_y2_3) {
		XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
		object_t3_y2_3->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t3_y2_3->Update();
	}
}

void GameScene::Stage1Reset()
{
	if (!firstTime)
	{
		for (auto object : objects)
		{
			XMFLOAT3 objectPosition = object->GetPosition();
			object->SetPosition({ objectPosition.x, objectPosition.y + 25.0f, objectPosition.z });
			object->Update();
		}
		for (auto object_2 : objects_2)
		{
			XMFLOAT3 objectPosition = object_2->GetPosition();
			object_2->SetPosition({ objectPosition.x, objectPosition.y + 25.0f, objectPosition.z });
			object_2->Update();
		}
	}

	objFighter->SetPosition({ -12,0,-12 });
	objClone->SetPosition({ 12,0,-12 });

	objTempTrigger->SetPosition({ -12.0f, 0, 0 });
	objTempTriggerE->SetPosition({ 12.0f, 0, 0 });

	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
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

	cameraMove = 1;

	beginStage = false;
}

void GameScene::Stage1Move()
{
	for (auto object : objects)
	{
		XMFLOAT3 objectPosition = object->GetPosition();
		object->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object->Update();
	}
	for (auto object_2 : objects_2)
	{
		XMFLOAT3 objectPosition = object_2->GetPosition();
		object_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_2->Update();
	}
}

void GameScene::Stage2Reset()
{
	if (!secondTime)
	{
		for (auto object_s2_1 : objects_s2_1) {
			XMFLOAT3 objectPosition = object_s2_1->GetPosition();
			object_s2_1->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_s2_1->Update();
		}

		for (auto object_s2_2 : objects_s2_2) {
			XMFLOAT3 objectPosition = object_s2_2->GetPosition();
			object_s2_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_s2_2->Update();
		}

		if (!stage2YellowKabe)
		{
			for (auto object_s2_y : objects_s2_y) {
				XMFLOAT3 objectPosition = object_s2_y->GetPosition();
				object_s2_y->SetPosition({ objectPosition.x, objectPosition.y + 100.0f, objectPosition.z });
				object_s2_y->Update();
			}

			for (auto object_s2_y2 : objects_s2_y2) {
				XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
				object_s2_y2->SetPosition({ objectPosition.x, objectPosition.y + 100.0f, objectPosition.z });
				object_s2_y2->Update();
			}
		}

		if (stage2Switch)
		{
			for (auto object_s2_s : objects_s2_s) {
				XMFLOAT3 objectPosition = object_s2_s->GetPosition();
				object_s2_s->SetPosition({ objectPosition.x, objectPosition.y + 100.0f, objectPosition.z });
				object_s2_s->Update();
			}

			for (auto object_s2_s2 : objects_s2_s2) {
				XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
				object_s2_s2->SetPosition({ objectPosition.x, objectPosition.y + 100.0f, objectPosition.z });
				object_s2_s2->Update();
			}
		}
	}

	objFighter->SetPosition({ -24,0,-12 });
	objClone->SetPosition({ 24,0,-12 });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	enemyAlive = true;
	playerAlive = true;

	objTempTrigger->SetPosition({ -13.0f, 0, -12.0f });
	objTempTriggerE->SetPosition({ 10.0f, 0, -9.0f });
	objTempYellowTrigger1->SetPosition({ -7.0f, 0, 6.0f });

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	cameraMove = 1;

	stage2YellowKabe = true;
	stage2Switch = false;

	beginStage = false;
}

void GameScene::Stage2Move()
{
	for (auto object_s2_1 : objects_s2_1) {
		XMFLOAT3 objectPosition = object_s2_1->GetPosition();
		object_s2_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s2_1->Update();
	}

	for (auto object_s2_2 : objects_s2_2) {
		XMFLOAT3 objectPosition = object_s2_2->GetPosition();
		object_s2_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s2_2->Update();
	}

	for (auto object_s2_y : objects_s2_y) {
		XMFLOAT3 objectPosition = object_s2_y->GetPosition();
		object_s2_y->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s2_y->Update();
	}

	for (auto object_s2_y2 : objects_s2_y2) {
		XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
		object_s2_y2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s2_y2->Update();
	}

	for (auto object_s2_s : objects_s2_s) {
		XMFLOAT3 objectPosition = object_s2_s->GetPosition();
		object_s2_s->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s2_s->Update();
	}

	for (auto object_s2_s2 : objects_s2_s2) {
		XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
		object_s2_s2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s2_s2->Update();
	}
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