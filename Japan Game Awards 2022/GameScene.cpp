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
extern int stageSelect = 0; //ステージセレクト
extern int sceneChange = 0;

extern XMFLOAT3 playerPositionTemp = { 0,0,0 };
extern XMFLOAT3 playerRotationTemp = { 0,0,0 };

extern XMFLOAT3 clonePositionTemp = { 0,0,0 };
extern XMFLOAT3 cloneRotationTemp = { 0,0,0 };

extern int cameraMove = 1;
extern int prevCameraMove = 0;

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

	// Sprite
	safe_delete(spriteBG);
	safe_delete(GuideR);
	safe_delete(Guide_LRB);
	safe_delete(Order_1);
	safe_delete(Order_2);
	safe_delete(Mirror);
	safe_delete(GameOverLog);
	safe_delete(TitleLog);
	safe_delete(StageSelectRB);
	safe_delete(StageSelectLB);

	// obj object
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objFighter);
	safe_delete(objClone);
	safe_delete(objTempBullet);
	safe_delete(objTempBulletE);

	// obj model
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelPlane);

	// Fbx object
	safe_delete(fbxobject1);
	safe_delete(objPlayerStand);
	safe_delete(objPlayerRun);
	safe_delete(objPlayerFight);
	safe_delete(objPlayerWin);
	safe_delete(objPlayerLose);
	safe_delete(objCloneStand);
	safe_delete(objCloneRun);
	safe_delete(objCloneFight);

	// Fbx model
	safe_delete(modelPlayerStand);
	safe_delete(modelPlayerRun);
	safe_delete(modelPlayerFight);
	safe_delete(modelPlayerWin);
	safe_delete(modelPlayerLose);
	safe_delete(modelCloneStand);
	safe_delete(modelCloneRun);
	safe_delete(modelCloneFight);

	ReleaseInput();

	//オーディオ解放
	audio->Finalize();
	safe_delete(audio);
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

	//オーディオ初期化
	audio->Initialize();

	//音声データのロード
	audio->LoadWave("GameClear.wav");
	audio->LoadWave("GameOver.wav");
	audio->LoadWave("Title.wav");
	audio->LoadWave("Stage.wav");

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
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成 Background sprite generation
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

#pragma region 3Dオブジェクト生成
	// 3Dオブジェクト生成 3D object generation
	objSkydome = Object3d::Create();
	objTempBullet = Object3d::Create();
	objTempBulletE = Object3d::Create();
	objMenuSelection = Object3d::Create();
	objPlayerMarker = Object3d::Create();
	
	// スイッチ
	objButtonRed1 = Object3d::Create();
	objButtonRed2 = Object3d::Create();
	objButtonBlue = Object3d::Create();
	objButtonGreen1 = Object3d::Create();
	objButtonGreen2 = Object3d::Create();
	objButtonYellow = Object3d::Create();
	objButtonBomb1 = Object3d::Create();
	objButtonBomb2 = Object3d::Create();
	objButtonFloor = Object3d::Create();

	// ワープ
	objTeleporterIn1 = Object3d::Create();
	objTeleporterIn2 = Object3d::Create();
	objTeleporterIn3 = Object3d::Create();
	objTeleporterIn4 = Object3d::Create();
	objTeleporterOut1 = Object3d::Create();
	objTeleporterOut2 = Object3d::Create();
	objTeleporterOut3 = Object3d::Create();
	objTeleporterOut4 = Object3d::Create();
	 
#pragma endregion

#pragma region Sprite テクスチャの読み込み
	// テクスチャ5番に読み込み Load into texture # 2
	Sprite::LoadTexture(5, L"Resources/Sprite/GuideL.png");

	GuideR = Sprite::Create(5, { 40.0f,620.0f });

	// テクスチャ6番に読み込み Load into texture # 2
	Sprite::LoadTexture(6, L"Resources/Sprite/Guide_LRB.png");

	Guide_LRB = Sprite::Create(6, { 650.0f,620.0f });

	// テクスチャ7番に読み込み Load into texture # 2
	Sprite::LoadTexture(7, L"Resources/Sprite/order1.png");

	Order_1 = Sprite::Create(7, { 0.0f,0.0f });

	// テクスチャ8番に読み込み Load into texture # 2
	Sprite::LoadTexture(8, L"Resources/Sprite/order2.png");

	Order_2 = Sprite::Create(8, { 0.0f,0.0f });

	Sprite::LoadTexture(9, L"Resources/Sprite/press_a.png");

	Press_A = Sprite::Create(9, { 0.0f,500.0f });

	Sprite::LoadTexture(10, L"Resources/Sprite/Mirror.png");

	Mirror = Sprite::Create(10, { 0.0f,0.0f });

	Sprite::LoadTexture(11, L"Resources/Sprite/GameOver.png");

	GameOverLog = Sprite::Create(11, { 0.0f,0.0f });

	Sprite::LoadTexture(12, L"Resources/Sprite/ReStateLog.png");

	ReStateLog = Sprite::Create(12, { 0.0f,470.0f });

	Sprite::LoadTexture(13, L"Resources/Sprite/GameOverStageSelectLog.png");

	GameOverStageSelectLog = Sprite::Create(13, { 0.0f,570.0f });

	Sprite::LoadTexture(14, L"Resources/Sprite/StageClear.png");

	StageClearLog = Sprite::Create(14, { 0.0f,0.0f });

	Sprite::LoadTexture(15, L"Resources/Sprite/TitleLog.png");

	TitleLog = Sprite::Create(15, { 0.0f,20.0f });

	Sprite::LoadTexture(16, L"Resources/Sprite/NextStageLog.png");

	NextStageLog = Sprite::Create(16, { -250.0f,350.0f });

	Sprite::LoadTexture(17, L"Resources/Sprite/StageClearStageSelectLog.png");

	StageClearStageSelectLog = Sprite::Create(17, { -250.0f,500.0f });

	Sprite::LoadTexture(18, L"Resources/Sprite/T1.png");
	t1Background = Sprite::Create(18, t1BackgroundPosition);
	t1Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(19, L"Resources/Sprite/T2.png");
	t2Background = Sprite::Create(19, t2BackgroundPosition);
	t2Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(20, L"Resources/Sprite/T3.png");
	t3Background = Sprite::Create(20, t3BackgroundPosition);
	t3Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(23, L"Resources/Sprite/T4.png");
	t4Background = Sprite::Create(23, t4BackgroundPosition);
	t4Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(21, L"Resources/Sprite/S1.png");
	s1Background = Sprite::Create(21, s1BackgroundPosition);
	s1Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(22, L"Resources/Sprite/S2.png");
	s2Background = Sprite::Create(22, s2BackgroundPosition);
	s2Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(24, L"Resources/Sprite/S3.png");
	s3Background = Sprite::Create(24, s3BackgroundPosition);
	s3Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(25, L"Resources/Sprite/StageSelect_RB.png");
	StageSelectRB = Sprite::Create(25, { 1050.0f,250.0f });

	Sprite::LoadTexture(26, L"Resources/Sprite/StageSelect_LB.png");
	StageSelectLB = Sprite::Create(26, { 25.0f,250.0f });

	Sprite::LoadTexture(27, L"Resources/Sprite/StageSelectLog.png");
	StageSelectLog = Sprite::Create(27, { 0.0f,0.0f });

	Sprite::LoadTexture(28, L"Resources/Sprite/StageSelectFrame.png");
	t1StageSelectFrame = Sprite::Create(28, { t1StageSelectFramePosition });
	t2StageSelectFrame = Sprite::Create(28, { t2StageSelectFramePosition });
	t3StageSelectFrame = Sprite::Create(28, { t3StageSelectFramePosition });
	t4StageSelectFrame = Sprite::Create(28, { t4StageSelectFramePosition });
	s1StageSelectFrame = Sprite::Create(28, { s1StageSelectFramePosition });
	s2StageSelectFrame = Sprite::Create(28, { s2StageSelectFramePosition });
	s3StageSelectFrame = Sprite::Create(28, { s3StageSelectFramePosition });

	Sprite::LoadTexture(29, L"Resources/Sprite/GuideA.png");
	GuideA = Sprite::Create(29, { 40.0f,620.0f });

	Sprite::LoadTexture(30, L"Resources/Sprite/GuideB.png");
	GuideB = Sprite::Create(30, { 760.0f,620.0f });

	Sprite::LoadTexture(31, L"Resources/Sprite/New.png");
	t1NewLog = Sprite::Create(31, { t1NewLogPosition });
	t1NewLog->SetRotation({ -30.0f });

	t2NewLog = Sprite::Create(31, { t2NewLogPosition });
	t2NewLog->SetRotation({ -30.0f });

	t3NewLog = Sprite::Create(31, { t3NewLogPosition });
	t3NewLog->SetRotation({ -30.0f });

	t4NewLog = Sprite::Create(31, { t4NewLogPosition });
	t4NewLog->SetRotation({ -30.0f });

	s1NewLog = Sprite::Create(31, { s1NewLogPosition });
	s1NewLog->SetRotation({ -30.0f });

	s2NewLog = Sprite::Create(31, { s2NewLogPosition });
	s2NewLog->SetRotation({ -30.0f });

	s3NewLog = Sprite::Create(31, { s3NewLogPosition });
	s3NewLog->SetRotation({ -30.0f });

	Sprite::LoadTexture(32, L"Resources/Sprite/Clear.png");
	t1ClearLog = Sprite::Create(32, { t1ClearLogPosition });
	t1ClearLog->SetRotation({ -30.0f });

	t2ClearLog = Sprite::Create(32, { t2ClearLogPosition });
	t2ClearLog->SetRotation({ -30.0f });

	t3ClearLog = Sprite::Create(32, { t3ClearLogPosition });
	t3ClearLog->SetRotation({ -30.0f });

	t4ClearLog = Sprite::Create(32, { t4ClearLogPosition });
	t4ClearLog->SetRotation({ -30.0f });

	s1ClearLog = Sprite::Create(32, { s1ClearLogPosition });
	s1ClearLog->SetRotation({ -30.0f });

	s2ClearLog = Sprite::Create(32, { s2ClearLogPosition });
	s2ClearLog->SetRotation({ -30.0f });

	s3ClearLog = Sprite::Create(32, { s3ClearLogPosition });
	s3ClearLog->SetRotation({ -30.0f });

	Sprite::LoadTexture(33, L"Resources/Sprite/Pause1.png");
	pause1 = Sprite::Create(33, { 720, 200 });

	Sprite::LoadTexture(34, L"Resources/Sprite/Pause2.png");
	pause2 = Sprite::Create(34, { 720, 300 });

	Sprite::LoadTexture(35, L"Resources/Sprite/Pause3.png");
	pause3 = Sprite::Create(35, { 720, 400 });

	Sprite::LoadTexture(36, L"Resources/Sprite/Pause4.png");
	pause4 = Sprite::Create(36, { 720, 200 });

	Sprite::LoadTexture(37, L"Resources/Sprite/GameOverGameClearSelect.png");
	GameOverGameClearSelectBar = Sprite::Create(37, { -250.0f,350.0f });

#pragma endregion

#pragma region Obj モデル読み込み
	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelPlane = Model::CreateFromOBJ("yuka");
	modelTempWall = Model::CreateFromOBJ("TempWall");
	modelTempWall2 = Model::CreateFromOBJ("TempWall2");
	modelYellowWall = Model::CreateFromOBJ("YellowKabe");
	modelTempBullet = Model::CreateFromOBJ("bullet2");
	//modelTeleporterIn = Model::CreateFromOBJ("TeleporterIn");
	modelTeleporterIn_1 = Model::CreateFromOBJ("Warp");
	modelTeleporterOut_1 = Model::CreateFromOBJ("Warp_2");
	modelPlayerMarker = Model::CreateFromOBJ("player_cursor");

	// スイッチ
	modelButtonRed = Model::CreateFromOBJ("ButtonRed");
	modelButtonBlue = Model::CreateFromOBJ("ButtonBlue");
	modelButtonGreen = Model::CreateFromOBJ("ButtonGreen");
	modelButtonYellow = Model::CreateFromOBJ("ButtonYellow");
	modelButtonBomb = Model::CreateFromOBJ("ButtonBumb");
	modelButtonFloor = Model::CreateFromOBJ("ButtonFloor");

	// ワープ

	modelTESTONLY = Model::CreateFromOBJ("playerSphere"); // TEST

#pragma endregion

#pragma region Obj オブジェクト生成とモデルとセット
	objSkydome->SetModel(modelSkydome);

	objTempBullet->SetModel(modelTempBullet);
	objTempBulletE->SetModel(modelTempBullet);

	// スイッチ
	objButtonRed1->SetModel(modelButtonRed);
	objButtonRed2->SetModel(modelButtonRed);
	objButtonBlue->SetModel(modelButtonBlue);
	objButtonGreen1->SetModel(modelButtonGreen);
	objButtonGreen2->SetModel(modelButtonGreen);
	objButtonYellow->SetModel(modelButtonYellow);
	objButtonBomb1->SetModel(modelButtonBomb);
	objButtonBomb2->SetModel(modelButtonBomb);
	objButtonFloor->SetModel(modelButtonFloor);

	objMenuSelection->SetModel(modelTempBullet);
	objPlayerMarker->SetModel(modelPlayerMarker);

	objFighter = Player::Create(modelTESTONLY);
	objClone = Enemy::Create(modelTESTONLY);

	objTeleporterIn1->SetModel(modelTeleporterIn_1);
	objTeleporterIn2->SetModel(modelTeleporterIn_1);
	objTeleporterIn3->SetModel(modelTeleporterIn_1);
	objTeleporterIn4->SetModel(modelTeleporterIn_1);
	objTeleporterOut1->SetModel(modelTeleporterOut_1);
	objTeleporterOut2->SetModel(modelTeleporterOut_1);
	objTeleporterOut3->SetModel(modelTeleporterOut_1);
	objTeleporterOut4->SetModel(modelTeleporterOut_1);


#pragma endregion

#pragma region Fbx モデル読み込み
	// FBXモデルの読み込み Loading FBX model
	modelPlayerRun = FbxLoader::GetInstance()->LoadModelFromFile("PlayerRunning");
	modelPlayerStand = FbxLoader::GetInstance()->LoadModelFromFile("PlayerStanding");
	modelPlayerFight = FbxLoader::GetInstance()->LoadModelFromFile("PlayerFighting");
	modelPlayerWin = FbxLoader::GetInstance()->LoadModelFromFile("PlayerWin");
	modelPlayerLose = FbxLoader::GetInstance()->LoadModelFromFile("PlayerLose");
	modelPlayerFall = FbxLoader::GetInstance()->LoadModelFromFile("PlayerFalling");
	modelCloneRun = FbxLoader::GetInstance()->LoadModelFromFile("CloneRunning");
	modelCloneStand = FbxLoader::GetInstance()->LoadModelFromFile("CloneStanding");
	modelCloneFight = FbxLoader::GetInstance()->LoadModelFromFile("CloneFighting");
	modelCloneFall = FbxLoader::GetInstance()->LoadModelFromFile("CloneFalling");

#pragma endregion

#pragma region Fbx オブジェクト生成とモデルとセット

	// FBX3Dオブジェクト生成とモデルとセット FBX3D object generation and model set
	// プレイヤー関連 Player related
	objPlayerRun = new FbxObject3d;
	objPlayerRun->Initialize();
	objPlayerRun->SetModel(modelPlayerRun);

	objPlayerStand = new FbxObject3d;
	objPlayerStand->Initialize();
	objPlayerStand->SetModel(modelPlayerStand);

	objPlayerFight = new FbxObject3d;
	objPlayerFight->Initialize();
	objPlayerFight->SetModel(modelPlayerFight);

	objPlayerWin = new FbxObject3d;
	objPlayerWin->Initialize();
	objPlayerWin->SetModel(modelPlayerWin);

	objPlayerLose = new FbxObject3d;
	objPlayerLose->Initialize();
	objPlayerLose->SetModel(modelPlayerLose);

	objPlayerFall = new FbxObject3d;
	objPlayerFall->Initialize();
	objPlayerFall->SetModel(modelPlayerFall);

	// クローン関連 Clone related
	objCloneRun = new FbxObject3d;
	objCloneRun->Initialize();
	objCloneRun->SetModel(modelCloneRun);

	objCloneStand = new FbxObject3d;
	objCloneStand->Initialize();
	objCloneStand->SetModel(modelCloneStand);

	objCloneFight = new FbxObject3d;
	objCloneFight->Initialize();
	objCloneFight->SetModel(modelCloneFight);

	objCloneFall = new FbxObject3d;
	objCloneFall->Initialize();
	objCloneFall->SetModel(modelCloneFall);

#pragma endregion

#pragma region PlayerInitialization

	// プレイヤー初期化 Player initialization
	objPlayerRun->SetPosition({ 0, 0, 0 });
	objPlayerRun->SetRotation({ 0, 0, 0 });
	objPlayerRun->SetScale({ 0.5, 0.5, 0.5 });

	objPlayerStand->SetPosition({ 0, 0, 0 });
	objPlayerStand->SetRotation({ 0, 0, 0 });
	objPlayerStand->SetScale({ 0.5,0.5,0.5 });

	objPlayerFight->SetPosition({ -4,-2,7 });
	objPlayerFight->SetRotation({ 0,-160,0 });
	objPlayerFight->SetScale({ 1,1,1 });

	objPlayerWin->SetPosition({ 0,-7,-7.5 });
	objPlayerWin->SetRotation({ 0,-80,0 });
	objPlayerWin->SetScale({ 0.7,0.7,0.7 });

	objPlayerLose->SetPosition({ 0,0,0 });
	objPlayerLose->SetRotation({ 0,-90,0 });
	objPlayerLose->SetScale({ 0.7,0.7,0.7 });

	objPlayerFall->SetPosition({ 0,0,0 });
	objPlayerFall->SetRotation({ 0,0,0 });
	objPlayerFall->SetScale({ 0.5,0.5,0.5 });

	// クローン初期化 Clone initialization
	objCloneRun->SetPosition({ 0, 0, 0 });
	objCloneRun->SetRotation({ 0, 0, 0 });
	objCloneRun->SetScale({ 0.5, 0.5, 0.5 });

	objCloneStand->SetPosition({ 0, 0, 0 });
	objCloneStand->SetRotation({ 0, 0, 0 });
	objCloneStand->SetScale({ 0.5, 0.5, 0.5 });

	objCloneFight->SetPosition({ -4,-2,-7 });
	objCloneFight->SetRotation({ 0,-20,0 });
	objCloneFight->SetScale({ 1,1,1 });

	objCloneFall->SetPosition({ 0,0,0 });
	objCloneFall->SetRotation({ 0, 0, 0 });
	objCloneFall->SetScale({ 0.5,0.5,0.5 });

#pragma endregion

#pragma region ModelTable

	// モデルテーブル Model table
	Model* modeltable[12] = {
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
		modelTempWall2,
	};

	Model* modeltable_2[12] = {
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
		modelTempWall2,
	};

	Model* modeltable_s2[3] = {
		modelPlane,
		modelTempWall,
		modelTempWall2,
	};

	Model* modeltable_y[1] = {
		modelYellowWall,
	};

	Model* modeltable_y2[2] = {
		modelYellowWall,
		modelPlane,
	};

#pragma endregion

#pragma region マップチップ生成 チュートリアル1
	// チュートリアル 1
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 2 && i < 10 && j == 3)
			{
				modelIndex = 2;
			}

			TouchableObject* object_t1_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 2 && i < 10 && j == 3)
			{
				object_t1_1->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_t1_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t1_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t1_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t1_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t1_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t1_1->SetRotation({ 180, 90, 0 });
			}
			else if (i > 2 && i < 10 && j == 3)
			{
				object_t1_1->SetRotation({ 180, 0, 270 });
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

			if (i == 10 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 2 && i < 10 && j == 3)
			{
				modelIndex = 2;
			}

			if (i < 10 && i > 7 && j < 6 && j > 3)
			{
				continue;
			}

			TouchableObject* object_t1_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 2 && i < 10 && j == 3)
			{
				object_t1_2->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_t1_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t1_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t1_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t1_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t1_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t1_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t1_2->SetRotation({ 180, 90, 0 });
			}
			else if (i > 2 && i < 10 && j == 3)
			{
				object_t1_2->SetRotation({ 180, 0, 270 });
			}
			objects_t1_2.push_back(object_t1_2);
		}
	}

#pragma endregion

#pragma region マップチップ生成 チュートリアル2

	// チュートリアル 2
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i < 10 && i > 6 && j > 0 && j < 4 || i < 4 && j < 6 && j > 2)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			if (i == 9 && j < 6 && j > 3)
			{
				continue;
			}

			TouchableObject* object_t2_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (modelIndex == 1 && i < 10 && i > 6 && j > 0 && j < 4)
			{
				object_t2_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE * 1.5f });
			}
			else
			{
				object_t2_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t2_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t2_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t2_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t2_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t2_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t2_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 1 && i < 10 && i > 6 && j > 0 && j < 4)
			{
				object_t2_1->SetRotation({ -90, 180, 0 });
			}
			else if (modelIndex == 1 && i != 10)
			{
				object_t2_1->SetRotation({ 180, 180, 0 });
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

			if (i == 10 || j == 0 || j == 6 || i < 4 && j < 6 && j > 2)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			if (i < 10 && i > 7 && j == 3)
			{
				modelIndex = 1;
			}
			else if (i < 10 && i > 6 && j > 0 && j < 4)
			{
				modelIndex = 2;
			}

			if (i == 9 && j < 6 && j > 3)
			{
				continue;
			}

			TouchableObject* object_t2_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (modelIndex == 2 && i < 10 && i > 6 && j > 0 && j < 4)
			{
				object_t2_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE * 1.5f });
			}
			else
			{
				object_t2_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t2_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t2_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t2_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t2_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t2_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i < 10 && i > 6 && j > 0 && j < 4)
			{
				object_t2_2->SetRotation({ -90, 180, 0 });
			}
			else if (modelIndex == 1 && i != 10)
			{
				object_t2_2->SetRotation({ 180, 180, 0 });
			}
			objects_t2_2.push_back(object_t2_2);
		}
	}

#pragma endregion

#pragma region マップチップ生成 チュートリアル3

	// チュートリアル 3
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i > 7 && i < 10 && j > 0 && j < 3 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6
				|| i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6 || i > 7 && i < 10 && j == 4)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 7 && i < 10 && j == 5)
			{
				modelIndex = 2;
			}

			TouchableObject* object_t3_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 7 && i < 10 && j > 0 && j < 3 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6 || i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				object_t3_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE * 1.5f });
			}
			else if (i > 7 && i < 10 && j > 3 && j < 6)
			{
				object_t3_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			else
			{
				object_t3_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t3_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t3_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t3_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t3_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t3_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t3_1->SetRotation({ 180, 90, 0 });
			}
			else if (i > 7 && i < 10 && j > 0 && j < 3)
			{
				object_t3_1->SetRotation({ -90, 180, 0 });
			}
			else if (i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6 || i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				object_t3_1->SetRotation({ 270, 180, 0 });
			}
			else if (i > 7 && i < 10 && j > 3 && j < 6)
			{
				object_t3_1->SetRotation({ 180, 90, 0 });
			}
			/*else if (i > 7 && i < 10 && j > 0 && j < 3 || i > 7 && i < 10 && j > 3 && j < 6 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6 || i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				object_t3_1->SetRotation({ 180, 0, 0 });
			}*/
			/*else if (i > 8 && i < 11 && j == 5 || i == 6 && j > 3 && j < 6)
			{
				object_t3_1->SetRotation({ 180, 0, 270 });
			}*/
			else if (modelIndex == 1 && i != 10)
			{
				object_t3_1->SetRotation({ 180, 180, 0 });
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

			if (i == 10 || j == 0 || j == 6 || i > 7 && i < 10 && j > 0 && j < 3 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6
				|| i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6 || i > 7 && i < 10 && j == 5)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 7 && i < 10 && j == 4)
			{
				modelIndex = 2;
			}

			if (i == 9 && j == 3)
			{
				continue;
			}

			TouchableObject* object_t3_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 7 && i < 10 && j > 0 && j < 3 || i > 7 && i < 10 && j > 3 && j < 6 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6 || i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				object_t3_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			else
			{
				object_t3_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t3_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t3_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t3_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t3_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t3_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t3_2->SetRotation({ 180, 90, 0 });
			}
			/*else if (i > 7 && i < 10 && j > 0 && j < 3 || i > 7 && i < 10 && j > 3 && j < 6)
			{
				object_t3_2->SetRotation({ -90, 180, 0 });
			}
			else if (i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6 || i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				object_t3_2->SetRotation({ 270, 180, 0 });
			}*/
			else if (i > 7 && i < 10 && j > 0 && j < 3 || i > 7 && i < 10 && j > 3 && j < 6 || i == 5 && j > 0 && j < 3 || i == 5 && j > 3 && j < 6 || i == 2 && j > 0 && j < 3 || i == 2 && j > 3 && j < 6)
			{
				object_t3_2->SetRotation({ 180, 90, 0 });
			}
			/*else if (i > 8 && i < 11 && j == 5 || i == 6 && j > 3 && j < 6)
			{
				object_t3_2->SetRotation({ 180, 0, 270 });
			}*/
			else if (modelIndex == 1 && i != 10)
			{
				object_t3_2->SetRotation({ 180, 180, 0 });
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

#pragma endregion

#pragma region マップチップ生成 チュートリアル4

	// チュートリアル 4
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i < 10 && i > 2 && j > 0 && j < 3 || i == 6 || i == 3 && j != 5 || i < 10 && i > 6 && j < 6 && j > 3)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			if (i == 5 && j == 5)
			{
				continue;
			}

			TouchableObject* object_t4_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i == 3 && j != 5 && j != 6 || i < 10 && i > 2 && j > 0 && j < 3)
			{
				object_t4_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE * 1.5f });
			}
			else
			{
				object_t4_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t4_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t4_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t4_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t4_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t4_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t4_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 6 && j == 3)
			{
				object_t4_1->SetRotation({ 180,0,0 });
			}
			else if (i == 6 && j < 6 && j > 3)
			{
				object_t4_1->SetRotation({ 180,0,0 });
			}
			else if (i < 10 && i > 6 && j < 6 && j > 3)
			{
				object_t4_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 3 && j != 5 && j != 6 || i < 10 && i > 2 && j > 0 && j < 3)
			{
				object_t4_1->SetRotation({ 270, 180, 0 });
			}
			else if (modelIndex == 1 && i != 10)
			{
				object_t4_1->SetRotation({ 180, 180, 0 });
			}
			objects_t4_1.push_back(object_t4_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i < 10 && i > 5 && j < 6 && j > 3|| i == 3 && j == 4)
			{
				modelIndex = 1;
			}

			if (i == 0 || i < 10 && i > 2 && j > 0 && j < 3 || i == 3 && j != 4 && j != 5 && j != 6 && j != 0 || i == 6 && j == 3)
			{
				modelIndex = 2;
			}

			if (i == 5 && j == 5)
			{
				continue;
			}

			TouchableObject* object_t4_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i < 10 && i > 3 && j > 0 && j < 3 || i < 10 && i > 6 && j < 6 && j > 3 || i == 3 && j == 4 || i == 3 && j == 0)
			{
				object_t4_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE * 1.5f });
			}
			else if (i == 3 && j != 5 && j != 6 && j != 4 && j != 0)
			{
				object_t4_2->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_t4_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_t4_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t4_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_t4_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t4_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_t4_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t4_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 6 && j < 6 && j > 3)
			{
				object_t4_2->SetRotation({ 180,0,0 });
			}
			else if (i < 10 && i > 6 && j < 6 && j > 3)
			{
				object_t4_2->SetRotation({ 270, 270, 270 });
			}
			else if (i == 6 && j == 3)
			{
				object_t4_2->SetRotation({ 180,0,0 });
			}
			else if (i == 3 && j > 0 && j < 4)
			{
				object_t4_2->SetRotation({ 180, 270, 270 });
			}
			else if (i == 3 && j == 4)
			{
				object_t4_2->SetRotation({ 270, 180, 0 });
			}
			else if (i == 3 && j != 5 && j != 6 || i < 10 && i > 2 && j > 0 && j < 3)
			{
				object_t4_2->SetRotation({ 270, 180, 90 });
			}
			else if (modelIndex == 1 && i != 10)
			{
				object_t4_2->SetRotation({ 180, 180, 0 });
			}
			objects_t4_2.push_back(object_t4_2);
		}
	}

	//黄色ブロック
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 5 && i == 3)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t4_y = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t4_y->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t4_y->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t4_y->SetRotation({ 0, 180, 0 });
			}
			objects_t4_y.push_back(object_t4_y);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 5 && i == 3)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t4_y2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t4_y2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t4_y2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t4_y2->SetRotation({ 0, 180, 0 });
			}
			objects_t4_y2.push_back(object_t4_y2);
		}
	}
#pragma endregion

#pragma region マップチップ生成 ステージ1
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
				modelIndex = 11;
			}

			//上
			if (i == 9 && j == j)
			{
				modelIndex = 10;
			}

			//左
			if (i != 9 && i != 0 && j == 0)
			{
				modelIndex = 10;
			}

			//右
			if (i != 9 && i != 0 && j == 5)
			{
				modelIndex = 10;
			}

			/*if (i == 5 && j == 5)
			{
				modelIndex = 0;
			}*/

			TouchableObject* object = TouchableObject::Create(modeltable[modelIndex]);
			object->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 10 && i == 9 && j == 0 || modelIndex == 10 && i == 9 && j == 5)
			{
				object->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 11 && i == 0 && j == 0 || modelIndex == 11 && i == 0 && j == 5)
			{
				object->SetRotation({ 180, 90, 0 });
			}
			else if (i == 9 && modelIndex == 10)
			{
				object->SetRotation({ 180, 0, 0 });
			}
			else if (i == 2 && j == 2 || i == 5 && j == 3)
			{
				object->SetRotation({ 180, 90, 0 });
			}
			else if (i != 9 && modelIndex == 11)
			{
				object->SetRotation({ 180, 0, 0 });
			}
			else if (i != 9 && modelIndex == 10)
			{
				object->SetRotation({ 180, 180, 0 });
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
				modelIndex = 11;
			}

			//上
			if (i == 9 && j == j)
			{
				modelIndex = 10;
			}

			//左
			if (i != 9 && i != 0 && j == 0)
			{
				modelIndex = 10;
			}

			//右
			if (i != 9 && i != 0 && j == 5)
			{
				modelIndex = 10;
			}

			/*if (i == 5 && j == 5)
			{
				modelIndex = 0;
			}*/


			TouchableObject* object_2 = TouchableObject::Create(modeltable_2[modelIndex]);
			object_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 10 && i == 9 && j == 0 || modelIndex == 10 && i == 9 && j == 5)
			{
				object_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 11 && i == 0 && j == 0 || modelIndex == 11 && i == 0 && j == 5)
			{
				object_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 9 && modelIndex == 10)
			{
				object_2->SetRotation({ 180, 0, 0 });
			}
			else if (i == 5 && j == 3)
			{
				object_2->SetRotation({ 180, 90, 0 });
			}
			else if (i != 9 && modelIndex == 11)
			{
				object_2->SetRotation({ 180, 0, 0 });
			}
			else if (i != 9 && modelIndex == 10)
			{
				object_2->SetRotation({ 180, 180, 0 });
			}
			objects_2.push_back(object_2);
		}
	}

#pragma endregion

#pragma region マップチップ生成 ステージ2

	//ステージ　２
	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex = 0;

			//ステージ外壁の配置
			//下
			if (i == 0 && j == j)
			{
				modelIndex = 2;
			}

			//上
			if (i == 11 && j == j)
			{
				modelIndex = 1;
			}

			//左
			if (i != 0 && i != 11 && j == 0)
			{
				modelIndex = 1;
			}

			//右
			if (i != 0 && i != 11 && j == 7)
			{
				modelIndex = 1;
			}

			if (i == 1 && j == 3 || i == 2 && j == 4 || i == 6 && j == 6 || i == 9 && j == 3 || i == 10 && j == 1 || i == 5 && j != 3 && j != 0 && j != 7 || i == 7 && j > 2 && j < 6)
			{
				modelIndex = 2;
			}

			if (i == 7 && j == 2 || i == 9 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s2_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s2_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s2_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s2_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 11)
			{
				object_s2_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 7)
			{
				object_s2_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 11 && j == 0 || modelIndex == 1 && i == 11 && j == 7)
			{
				object_s2_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 7)
			{
				object_s2_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 5 && j != 3 && j != 0 && j != 7 || i == 7 && j > 2 && j < 6 || i == 1 && j == 3 || i == 2 && j == 4 || i == 6 && j == 6 || i == 9 && j == 3 || i == 10 && j == 1)
			{
				object_s2_1->SetRotation({ 180, 90, 0 });
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
				modelIndex = 2;
			}

			//上
			if (i == 11 && j == j)
			{
				modelIndex = 1;
			}

			//左
			if (i != 0 && i != 11 && j == 0)
			{
				modelIndex = 1;
			}

			//右
			if (i != 0 && i != 11 && j == 7)
			{
				modelIndex = 1;
			}

			if (i == 5 && j != 3 || i == 1 && j > 4 && j < 7 || i == 2 && j == 4 || i == 7 && j == 5 || i == 8 && j == 2 || i == 9 && j > 0 && j < 3)
			{
				modelIndex = 1;
			}

			if (i > 8 && i < 11 && j == 5 || i == 6 && j > 3 && j < 6)
			{
				modelIndex = 2;
			}

			if (i > 5 && i < 11 && j == 6)
			{
				continue;
			}

			TouchableObject* object_s2_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 8 && i < 11 && j == 5 || i == 6 && j > 3 && j < 6)
			{
				object_s2_2->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_s2_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_s2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s2_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 11)
			{
				object_s2_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 7)
			{
				object_s2_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 11 && j == 0 || modelIndex == 1 && i == 11 && j == 7)
			{
				object_s2_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 7)
			{
				object_s2_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 5 && j != 3 || i == 1 && j > 4 && j < 7 || i == 2 && j == 4 || i == 7 && j == 5 || i == 8 && j == 2 || i == 9 && j > 0 && j < 3)
			{
				object_s2_2->SetRotation({ 180, 90, 0 });
			}
			else if (i > 8 && i < 11 && j == 5 || i == 6 && j > 3 && j < 6)
			{
				object_s2_2->SetRotation({ 180, 0, 270 });
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

#pragma endregion

#pragma region マップチップ生成 ステージ３

	// ステージ３
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;
			
			if (i == 10 || j == 0 || j == 7 || i < 10 && i > 7 && j > 1 && j < 7 || i == 7 && j > 2 || i == 5 && j == 4 || i == 4 && j == 5 || i == 3 && j == 1 || i == 3 && j == 4 || i == 6 && j != 0 && j != 7)
			{
				modelIndex = 1;
			}

			if (i == 0 || j == 3 && i > 0 && i < 3)
			{
				modelIndex = 2;
			}

			if (i == 9 && j == 1 || i == 1 && j == 2 || i == 4 && j == 2)
			{
				continue;
			}

			TouchableObject* object_s3_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (j == 3 && i > 0 && i < 3 || i == 6 && j != 0 && j != 7)
			{
				object_s3_1->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_s3_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_s3_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s3_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s3_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 7)
			{
				object_s3_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 7)
			{
				object_s3_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 7)
			{
				object_s3_1->SetRotation({ 180, 90, 0 });
			}
			else if (i < 10 && i > 7 && j > 1 && j < 7 || i == 7 && j > 2 || i == 5 && j == 4 || i == 4 && j == 5 || i == 3 && j == 1 || i == 3 && j == 4)
			{
				object_s3_1->SetRotation({ 180, 90, 0 });
			}
			else if (j == 3 && i > 0 && i < 3)
			{
				object_s3_1->SetRotation({ 180, 0, 270 });
			}
			else if (i == 6 && j != 0 && j != 7)
			{
				object_s3_1->SetRotation({ 180, 270, 270 });
			}
			objects_s3_1.push_back(object_s3_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 7 || i == 9 && j == 3 || i == 8 && j == 4 || i == 6 && j != 1 || i == 5 && j == 5 || i == 3 && j > 0 && j < 4 || i == 1 && j > 2 && j < 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i < 4 && i > 1 && j == 5 || i < 6 && i > 3 && j == 3)
			{
				modelIndex = 2;
			}

			if (i == 9 && j == 6 || i == 7 && j == 6)
			{
				continue;
			}

			TouchableObject* object_s3_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i < 4 && i > 1 && j == 5 || i < 6 && i > 3 && j == 3)
			{
				object_s3_2->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_s3_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_s3_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s3_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s3_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 7)
			{
				object_s3_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 7)
			{
				object_s3_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 7)
			{
				object_s3_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 9 && j == 3 || i == 8 && j == 4 || i == 6 && j != 1 || i == 5 && j == 5 || i == 3 && j > 0 && j < 4 || i == 1 && j > 2 && j < 6)
			{
				object_s3_2->SetRotation({ 180, 90, 0 });
			}
			else if (i < 4 && i > 1 && j == 5 || i < 6 && i > 3 && j == 3)
			{
				object_s3_2->SetRotation({ 180, 0, 270 });
			}
			/*else if (modelIndex == 1 && i == 3 && j > 0 && j < 4)
			{
				object_s3_2->SetRotation({ 180, 90, 0 });
			}*/
			objects_s3_2.push_back(object_s3_2);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 2 && i == 3)
			{
				modelIndex = 0;
			}
			else if (j == 2 && i == 4)
			{
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y1_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s3_y1_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s3_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_s3_y1_1->SetRotation({ 0, 180, 0 });
			}
			objects_s3_y1_1.push_back(object_s3_y1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 6 && i == 3)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y2_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s3_y2_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s3_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_s3_y2_1->SetRotation({ 0, 180, 0 });
			}
			objects_s3_y2_1.push_back(object_s3_y2_1);
		}
	}

	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 1 && i == 8)
			{
				int modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y1_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s3_y1_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s3_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_s3_y1_2->SetRotation({ 0, 180, 0 });
			}
			objects_s3_y1_2.push_back(object_s3_y1_2);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 1 && i == 6)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y2_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s3_y2_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s3_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_s3_y2_2->SetRotation({ 0, 180, 0 });
			}
			objects_s3_y2_2.push_back(object_s3_y2_2);
		}
	}
#pragma endregion

#pragma region PositionRotationScale

	//objFighter->SetPosition({ -10, 10, 0 });
	objFighter->SetScale({ 1.0f,1.0f,1.0f }); //3.0
	//objFighter->SetCollider(new SphereCollider);
	objClone->SetScale({ 1.0f,1.0f,1.0f });
	//objClone->SetCollider(new SphereCollider);

	objFighter->SetPosition({ -20, 0, 12 }); // -20, 12
	objClone->SetPosition({ 20, 0, 12 }); // 20, 12

	objTempBullet->SetPosition({ -6.0f, 1.0f, 0 });
	objTempBulletE->SetPosition({ 6.0f, 1.0f, 0 });
	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	objMenuSelection->SetScale({ 0.25f, 0.25f, 0.25f });

	objPlayerMarker->SetScale({ 0.5f, 0.5f, 0.5f });

	objTeleporterIn1->SetPosition({ -20.0f, 0.0f, -3.0f }); // -14, -9
	objTeleporterIn1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn2->SetPosition({ 5.0f, 0.0f, -12.0f }); // 14, -9
	objTeleporterIn2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn2->SetRotation({ 0.0f, 0.0f, 0.0f });


	objTeleporterIn3->SetPosition({ -11.0f, 0.0f, -9.0f }); // -14, 12
	objTeleporterIn3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn3->SetRotation({ 0.0f, 0.0f, 0.0f });


	objTeleporterIn4->SetPosition({ 5.0f, 0.0f, 0.0f }); // 14, 12
	objTeleporterIn4->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn4->SetRotation({ 0.0f, 0.0f, 0.0f });



	objTeleporterOut1->SetPosition({ 11.0f, 0.0f, 12.0f }); // -14, 6
	objTeleporterOut1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut2->SetPosition({ -17.0f, 0.0f, 6.0f }); // 14, 6
	objTeleporterOut2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut2->SetRotation({ 0.0f, 0.0f, 0.0f });


	objTeleporterOut3->SetPosition({ 11.0f, 0.0f, 0.0f }); // -8 -12
	objTeleporterOut3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut4->SetPosition({ -8.0f, 0.0f, 0.0f }); // 8, -12
	objTeleporterOut4->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut4->SetRotation({ 0.0f, 0.0f, 0.0f });




#pragma endregion

	camera->SetEye({ 0, 20, -30 });

	camera->SetTarget({ 0, 1, 0 });
	//camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f }); //debug

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
	FBXplayerPosition = playerPositionTemp;
	FBXplayerRotation = playerRotationTemp;

	FBXclonePosition = clonePositionTemp;
	FBXcloneRotation = cloneRotationTemp;

	// 座標の変更を反映
	objPlayerRun->SetPosition(FBXplayerPosition);
	objPlayerRun->SetRotation(FBXplayerRotation);
	objPlayerStand->SetPosition(FBXplayerPosition);
	objPlayerStand->SetRotation(FBXplayerRotation);
	objPlayerFall->SetPosition(FBXplayerPosition);
	objPlayerFall->SetRotation(FBXplayerRotation);

	objCloneRun->SetPosition(FBXclonePosition);
	objCloneRun->SetRotation(FBXcloneRotation);
	objCloneStand->SetPosition(FBXclonePosition);
	objCloneStand->SetRotation(FBXcloneRotation);
	objCloneFall->SetPosition(FBXclonePosition);
	objCloneFall->SetRotation(FBXcloneRotation);

	playerPosition = objFighter->GetPosition();
	playerRotation = objFighter->GetRotation();
	enemyPosition = objClone->GetPosition();
	enemyRotation = objClone->GetRotation();
	BlueButton = objButtonBlue->GetPosition();
	YellowButton = objButtonYellow->GetPosition();
	GreenButton1 = objButtonGreen1->GetPosition();
	GreenButton2 = objButtonGreen2->GetPosition();
	RedButton1 = objButtonRed1->GetPosition();
	RedButton2 = objButtonRed2->GetPosition();
	BombButton1 = objButtonBomb1->GetPosition();
	BombButton2 = objButtonBomb2->GetPosition();
	FloorButton = objButtonFloor->GetPosition();
	playerBullet = objTempBullet->GetPosition();
	enemyBullet = objTempBulletE->GetPosition();

	playerMarkerPosition = objPlayerMarker->GetPosition();
	playerMarkerRotation = objPlayerMarker->GetRotation();

	// GameSceneとの座標共有 Coordinate sharing with GameScene
	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	if (beginStage && !falling && !pause)
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
	}

	if (falling)
	{
		if (playerPosition.y <= 0.3f || playerPositionTemp.y <= 0.3f)
		{
			falling = false;
		}
	}

	playerMarkerPosition.x = playerPosition.x;
	playerMarkerPosition.y = playerPosition.y + 8.0f;
	playerMarkerPosition.z = playerPosition.z;

	objPlayerMarker->SetPosition(playerMarkerPosition);
	objPlayerMarker->Update();

	if (!beginStage)
	{
		playerMarkerRotation.y = 0.0f;
		objPlayerMarker->SetRotation(playerMarkerRotation);
		objPlayerMarker->Update();
	}

	if (input->TriggerKey(DIK_M) || IsButtonPush(ButtonKind::Button_LeftMenu))
	{
		if (marker)
		{
			marker = false;
		}
		else
		{
			marker = true;
		}
	}

	//ButtonKind::Button_RightMenu = Start

#pragma region ポーズ画面
	if (input->TriggerKey(DIK_ESCAPE) && sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && beginStage && !falling ||
		IsButtonPush(ButtonKind::Button_RightMenu) && sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && beginStage && !falling)
	{
		if (!pause)
		{
			pause = true;
		}
	}

	if (pause)
	{
		if (!pausePosition)
		{
			playerPausePosition = objFighter->GetPosition();
			playerPauseRotation = objFighter->GetRotation();
			clonePausePosition = objClone->GetPosition();
			clonePauseRotation = objClone->GetRotation();
			playerMarkerPausePosition = objPlayerMarker->GetPosition();
			playerMarkerPauseRotation = objPlayerMarker->GetRotation();

			pausePosition = true;
		}

		objFighter->SetPosition(playerPausePosition);
		objFighter->SetRotation(playerPauseRotation);
		objPlayerStand->SetPosition(playerPausePosition);
		objPlayerStand->SetRotation(playerPauseRotation);
		objClone->SetPosition(clonePausePosition);
		objClone->SetRotation(clonePauseRotation);
		objCloneStand->SetPosition(clonePausePosition);
		objCloneStand->SetRotation(clonePauseRotation);

		objPlayerMarker->SetPosition(playerMarkerPausePosition);
		objPlayerMarker->SetRotation(playerMarkerPauseRotation);

		objFighter->Update();
		objClone->Update();
		objPlayerMarker->Update();

		if (input->TriggerKey(DIK_S) && pauseMenuSelection < 2 || IsButtonPush(ButtonKind::DownButton) && pauseMenuSelection < 2)
		{
			pauseMenuSelection++;
		}
		else if (input->TriggerKey(DIK_W) && pauseMenuSelection > 0 || IsButtonPush(ButtonKind::UpButton) && pauseMenuSelection > 0)
		{
			pauseMenuSelection--;
		}

		switch (pauseMenuSelection)
		{
		case 0:
			pause4->SetPosition({ 720, 200 });
			break;
		case 1:
			pause4->SetPosition({ 720, 300 });
			break;
		case 2:
			pause4->SetPosition({ 720, 400 });
			break;
		}

		if (input->TriggerKey(DIK_SPACE) || IsButtonPush(ButtonKind::Button_A))
		{
			if (pauseMenuSelection == 0)
			{
				pause = false;
				pausePosition = false;
			}
			else if (pauseMenuSelection == 1)
			{
				switch (sceneNo)
				{
				case 0:
					break;
				case 1:
					Stage1Move();
					Stage1Reset();
					sceneNo = 1;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					Stage2Move();
					Stage2Reset();
					sceneNo = 4;
					break;
				case 5:
					Tutorial1Move();
					Tutorial1Reset();
					sceneNo = 5;
					break;
				case 6:
					Tutorial2Move();
					Tutorial2Reset();
					sceneNo = 6;
					break;
				case 7:
					Tutorial3Move();
					Tutorial3Reset();
					sceneNo = 7;
					break;
				case 8:
					break;
				case 9:
					Tutorial4Move();
					Tutorial4Reset();
					sceneNo = 9;
					break;
				case 10:
					Stage3Move();
					Stage3Reset();
					sceneNo = 10;
					break;
				}

				pause = false;
				pausePosition = false;
			}
			else if (pauseMenuSelection == 2)
			{
				switch (sceneNo)
				{
				case 0:
					break;
				case 1:
					Stage1Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					Stage2Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				case 5:
					Tutorial1Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				case 6:
					Tutorial2Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				case 7:
					Tutorial3Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				case 8:
					break;
				case 9:
					Tutorial4Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				case 10:
					Stage3Move();
					sceneNo = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					delay = true;
					break;
				}

				pause = false;
				pausePosition = false;
			}
		}
	}
#pragma endregion

#pragma region カメラ回転 Camera Rotation
	// Camera Movement カメラ動く
	if (beginStage && sceneNo != 8 && sceneNo != 2 && sceneNo != 0 && sceneNo != 3 && !pause)
	{
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
	}

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

			playerMarkerRotation.y = 0.0f;
			objPlayerMarker->SetRotation(playerMarkerRotation);
			objPlayerMarker->Update();

			if (currentCameraFrame >= 40)
			{
				camera->SetEye({ 0.0f, 25.0f, -30.0f });
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

			playerMarkerRotation.y = 90.0f;
			objPlayerMarker->SetRotation(playerMarkerRotation);
			objPlayerMarker->Update();

			if (currentCameraFrame >= 40)
			{
				camera->SetEye({ -40.0f, 25.0f, 0.0f });
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

			playerMarkerRotation.y = 180.0f;
			objPlayerMarker->SetRotation(playerMarkerRotation);
			objPlayerMarker->Update();

			if (currentCameraFrame >= 40)
			{
				camera->SetEye({ 0.0f, 25.0f, 30.0f });
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

			playerMarkerRotation.y = 270.0f;
			objPlayerMarker->SetRotation(playerMarkerRotation);
			objPlayerMarker->Update();

			if (currentCameraFrame >= 40)
			{
				camera->SetEye({ 40.0f, 25.0f, 0.0f });
				currentCameraFrame = 0;
				cameraChange = false;
			}
		}
	}
#pragma endregion

#pragma region シーン遷移
	//シーン遷移
	switch (sceneNo)
	{
	case 0: // タイトル画面
#pragma region case0 タイトル画面
		camera->SetEye({ -15,10,0 });
		camera->SetTarget({ 0, 10, 0 });

		titleScene->Update();
		objPlayerFight->Update();
		objCloneFight->Update();
		objSkydome->Update();
		camera->Update();

		//音楽再生	
		if (PlayFlag == false )
		{
			PlayFlag = true;
			audio->PlayWave("Title.wav", Volume, true);
			//audio->WaveVolume("Title.wav", 0.3f);
		}

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
			/*if (!t1Time)
			{
				Tutorial1Reset();
			}
			else
			{
				camera->SetEye({ 0, 20, -30 });
				camera->SetTarget({ 0, 1, 0 });
				camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });
			}*/

			SceneSelectionReset();
			sceneNo = 8;
			titleScene->Finalize();
			break;
		}

		if (IsButtonDown(ButtonKind::Button_A))
		{
			/*if (!t1Time)
			{
				Tutorial1Reset();
			}
			else
			{
				camera->SetEye({ 0, 20, -30 });
				camera->SetTarget({ 0, 1, 0 });
				camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });
			}*/

			SceneSelectionReset();
			sceneNo = 8;
			titleScene->Finalize();
			break;
		}
#pragma endregion
		break;

	case 1: // ステージ1
#pragma region case1 ステージ1
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
				lastScene = 1;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -12,30,-12 });
			objClone->SetPosition({ 12,30,-12 });

			playerPositionTemp = { -12,30,-12 };
			clonePositionTemp = { 12,30,-12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -12.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -12.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 12.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 12.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerFall->SetRotation({ 0, 320, 0 });
				objCloneFall->SetRotation({ 0, 320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

			if (intersect(playerPosition, BombButton1, 1.0f, 1.0f, 1.0f) && lastIntersect == false)
			{
				playerBulletF = true;
			}

			if (intersect(enemyPosition, BombButton2, 1.0f, 1.0f, 1.0f) && lastIntersectE == false)
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

			if (playerBullet.x > 14.0f)
			{
				playerBulletF = false;
			}

			if (enemyBullet.x < -14.0f)
			{
				enemyBulletF = false;
			}

			if (intersect(enemyBullet, playerPosition, 1.0f, 1.0f, 1.0f) && enemyBulletF == true)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage1Move();
				GameOverReset();
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				sceneChange = 0;

				s1FirstPlayFlag = false;
			}
			else if (intersect(playerBullet, enemyPosition, 1.0f, 1.0f, 1.0f) && playerBulletF == true)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage1Move();
				GameClearReset();
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				sceneChange = 0;

				s1FirstPlayFlag = false;
				s1ClearFlag = true;
			}

			//UpdateInput();

			//objFighter->SetPosition({ playerPosition });

			//particleMan->Update();

			//objSkydome->Update();

			for (auto object : objects) {
				object->Update();
			}

			for (auto object_2 : objects_2) {
				object_2->Update();
			}

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

			/*if (input->TriggerKey(DIK_R))
			{
				Stage1Move();
				Stage1Reset();
				sceneNo = 1;
			}*/

			if (input->TriggerKey(DIK_T))
			{
				Stage1Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			/*if (input->TriggerKey(DIK_L))
			{
				Stage1Move();
				sceneNo = 8;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
			}*/
		}

		for (auto object : objects) {
			object->Update();
		}

		for (auto object_2 : objects_2) {
			object_2->Update();
		}

		
		objButtonBomb1->Update();
		
		objButtonBomb2->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objFighter->Update();
		objClone->Update();

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;

	case 2: // クリア画面
#pragma region case2 クリア画面
		camera->SetEye({ -15,0,0 });
		camera->SetTarget({ 0, 0, 0 });

		if (input->TriggerKey(DIK_SPACE) && !menuMoving || IsButtonDown(ButtonKind::Button_A) && !menuMoving)
		{
			audio->StopWave("GameClear.wav");
			if (menuSelection == 0)
			{
				switch (lastScene)
				{
				case 0:
					break;
				case 1:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage2Reset();
					sceneNo = 4;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage3Reset();
					sceneNo = 10;
					break;
				case 5:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial2Reset();
					sceneNo = 6;
					break;
				case 6:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial3Reset();
					sceneNo = 7;
					break;
				case 7:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial4Reset();
					sceneNo = 9;
					break;
				case 8:
					break;
				case 9:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage1Reset();
					sceneNo = 1;
					break;
				case 10:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage3Reset();
					sceneNo = 10;
					break;
				}

				break;
			}
			else if (menuSelection == 1)
			{
				audio->PlayWave("Title.wav", Volume, true);
				sceneNo = 8;
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
				break;
			}
		}

		if (input->PushKey(DIK_S) && menuSelection == 0 && !menuMoving || IsButtonDown(ButtonKind::DownButton) && menuSelection == 0 && !menuMoving)
		{
			menuMoving = true;
		}
		else if (input->PushKey(DIK_W) && menuSelection == 1 && !menuMoving || IsButtonDown(ButtonKind::UpButton) && menuSelection == 1 && !menuMoving)
		{
			menuMoving = true;
		}

		if (menuMoving)
		{
			if (menuSelection == 0)
			{
				GameOverGameClearSelectBar->SetPosition({ -250.0f,500.0f });
				menuSelection = 1;
				menuMoving = false;
			}
			else if (menuSelection == 1)
			{
				GameOverGameClearSelectBar->SetPosition({ -250.0f,350.0f });
				menuSelection = 0;
				menuMoving = false;
			}
		}

		if (!menuMoving)
		{
			if (menuSelection == 0)
			{
				objMenuSelection->SetPosition({ -7.0f, -0.5f, 6.5f });
			}
			else if (menuSelection == 1)
			{
				objMenuSelection->SetPosition({ -7.0f, -2.5f, 6.5f });
			}
		}

		menuBallRotation.x += 1.0f;
		objMenuSelection->SetRotation(menuBallRotation);
		objMenuSelection->Update();

		objPlayerWin->Update();
		gameClear->Update();
		objSkydome->Update();
		camera->Update();

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

		/*if (input->TriggerKey(DIK_SPACE))
		{
			sceneNo = 0;

			audio->StopWave("GameClear.wav");
			audio->PlayWave("Title.wav");
			gameClear->Finalize();
			titleScene->Initialize();
			break;
		}

		if (IsButtonUp(ButtonKind::Button_A))
		{
			sceneNo = 0;

			audio->StopWave("GameClear.wav");
			audio->PlayWave("Title.wav");
			gameClear->Finalize();
			titleScene->Initialize();
			break;
		}*/
#pragma endregion
		break;

	case 3: // ゲームオーバー画面
#pragma region case3 ゲームオーバー画面
		camera->SetEye({ -15,0,0 });
		camera->SetTarget({ 0, 0, 0 });

		objPlayerLose->Update();
		gameOver->Update();

		if (input->TriggerKey(DIK_SPACE) && !menuMoving || IsButtonDown(ButtonKind::Button_A) && !menuMoving)
		{
			audio->StopWave("GameOver.wav");
			if (menuSelection == 0)
			{
				switch (lastScene)
				{
				case 0:
					break;
				case 1:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage1Reset();
					sceneNo = 1;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage2Reset();
					sceneNo = 4;
					break;
				case 5:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial1Reset();
					sceneNo = 5;
					break;
				case 6:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial2Reset();
					sceneNo = 6;
					break;
				case 7:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial3Reset();
					sceneNo = 7;
					break;
				case 8:
					break;
				case 9:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial4Reset();
					sceneNo = 9;
					break;
				case 10:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage3Reset();
					sceneNo = 10;
					break;
				}

				break;
			}
			else if (menuSelection == 1)
			{
				audio->PlayWave("Title.wav", Volume, true);
				sceneNo = 8;
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
				break;
			}
		}

		if (input->PushKey(DIK_S) && menuSelection == 0 && !menuMoving || IsButtonDown(ButtonKind::DownButton) && menuSelection == 0 && !menuMoving)
		{
			menuMoving = true;
		}
		else if (input->PushKey(DIK_W) && menuSelection == 1 && !menuMoving || IsButtonDown(ButtonKind::UpButton) && menuSelection == 1 && !menuMoving)
		{
			menuMoving = true;
		}

		if (menuMoving)
		{
			if (menuSelection == 0)
			{
				GameOverGameClearSelectBar->SetPosition({ 0.0f, 570.0f });
				menuSelection = 1;
				menuMoving = false;
			}
			else if (menuSelection == 1)
			{
				GameOverGameClearSelectBar->SetPosition({ 0.0f, 470.0f });
				menuSelection = 0;
				menuMoving = false;
			}
		}

		if (!menuMoving)
		{
			if (menuSelection == 0)
			{
				objMenuSelection->SetPosition({ -5.0f, -2.65f, 4.0f });
			}
			else if (menuSelection == 1)
			{
				objMenuSelection->SetPosition({ -5.0f, -4.25f, 4.0f });
			}
		}

		menuBallRotation.x += 1.0f;
		objMenuSelection->SetRotation(menuBallRotation);
		objMenuSelection->Update();

		camera->Update();
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

		/*if (input->TriggerKey(DIK_SPACE))
		{
			sceneNo = 0;
			audio->StopWave("GameOver.wav");
			audio->PlayWave("Title.wav");
			gameOver->Finalize();
			titleScene->Initialize();
			break;
		}

		if (IsButtonUp(ButtonKind::Button_A))
		{
			sceneNo = 0;

			audio->StopWave("GameOver.wav");
			audio->PlayWave("Title.wav");
			gameOver->Finalize();
			titleScene->Initialize();
			break;
		}*/
#pragma endregion
		break;

	case 4: // ステージ2
#pragma region case4 ステージ2

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
				lastScene = 4;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -22,30,-12 });
			objClone->SetPosition({ 22,30,-12 });

			playerPositionTemp = { -22,30,-12 };
			clonePositionTemp = { 22,30,-12 };

			objFighter->Update();
			objClone->Update();
		}

		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -22.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -22.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 22.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 22.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerFall->SetRotation({ 0, 320, 0 });
				objCloneFall->SetRotation({ 0, 320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

			if (intersect(playerPosition, RedButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, RedButton2, 1.0f, 1.0f, 1.0f))
			{
				stage2YellowKabe = false;
			}
			if (intersect(playerPosition, FloorButton, 1.0f, 1.0f, 1.0f) && lastYellowIntersct1 == false)
			{
				stage2Switch = true;
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage2Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s2FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage2Move();
				GameClearReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				gameClear->Initialize();

				s2FirstPlayFlag = false;
				s2ClearFlag = true;
			}

			lastYellowIntersct1 = intersect(playerPosition, yellowTrigger1, 1.0f, 1.0f, 1.0f);

			/*if (input->TriggerKey(DIK_R))
			{
				Stage2Move();
				Stage2Reset();
				sceneNo = 4;
			}*/

			if (input->TriggerKey(DIK_T))
			{
				Stage2Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			/*if (input->TriggerKey(DIK_L))
			{
				Stage2Move();
				sceneNo = 8;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
			}*/
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
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		
		objButtonRed1->Update();
		
		objButtonRed2->Update();
		objButtonFloor->Update();

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;

	case 5: // チュートリアル 1
#pragma region case5 チュートリアル1
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
				lastScene = 5;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20,30,12 });
			objClone->SetPosition({ 20,30,12 });

			playerPositionTemp = { -20,30,12 };
			clonePositionTemp = { 20,30,12 };

			//objFighter->Update();
			//objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = 12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = 12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = 12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = 12.0f;

				objPlayerFall->SetRotation({ 0, 140, 0 });
				objCloneFall->SetRotation({ 0, 140, 0 });
				
				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial1Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				t1FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Tutorial1Move();
				GameClearReset();
				sceneChange = 0;

				t1FirstPlayFlag = false;
				t1ClearFlag = true;
			}

			//if (input->TriggerKey(DIK_R)) //リセット
			//{
			//	Tutorial1Move();
			//	Tutorial1Reset();
			//	sceneNo = 5;
			//}

			if (input->TriggerKey(DIK_T)) //タイトル
			{
				Tutorial1Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			//if (input->TriggerKey(DIK_L)) //レベルセレクト
			//{
			//	Tutorial1Move();
			//	sceneNo = 8;
			//	audio->StopWave("Stage.wav");
			//	audio->PlayWave("Title.wav", Volume, true);
			//	camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
			//	camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
			//	menuBallRotation = { 0.0f, 0.0f, 0.0f };
			//	menuSelection = 0;
			//}
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
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objSkydome->Update();

		camera->Update();

		collisionManager->CheckAllCollisions();
#pragma endregion
		break;

	case 6: // チュートリアル 2
#pragma region case6 チュートリアル2

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
				lastScene = 6;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30,-12 });
			objClone->SetPosition({ 20, 30, -12 });

			playerPositionTemp = { -20, 30,-12 };
			clonePositionTemp = { 20, 30, -12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerFall->SetRotation({ 0, 320, 0 });
				objCloneFall->SetRotation({ 0, 320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial2Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				t2FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Tutorial2Move();
				GameClearReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				gameClear->Initialize();

				t2FirstPlayFlag = false;
				t2ClearFlag = true;
			}

			/*if (input->TriggerKey(DIK_R))
			{
				Tutorial2Move();
				Tutorial2Reset();
				sceneNo = 6;
			}*/

			if (input->TriggerKey(DIK_T))
			{
				Tutorial2Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			/*if (input->TriggerKey(DIK_L))
			{
				Tutorial2Move();
				sceneNo = 8;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
			}*/
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
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objSkydome->Update();

		camera->Update();

		collisionManager->CheckAllCollisions();
#pragma endregion
		break;

	case 7: // チュートリアル 3
#pragma region case7 チュートリアル3

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
				lastScene = 7;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -12 });
			objClone->SetPosition({ 20, 30, -12 });

			playerPositionTemp = { -20, 30, -12 };
			clonePositionTemp = { 20, 30, -12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerFall->SetRotation({ 0, 320, 0 });
				objCloneFall->SetRotation({ 0, 320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

			if (intersect(enemyPosition, RedButton1, 1.0f, 1.0f, 1.0f))
			{
				tutorial3YellowKabe1 = false;
			}
			if (intersect(enemyPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				tutorial3YellowKabe2 = false;
			}
			if (intersect(playerPosition, GreenButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, GreenButton2, 1.0f, 1.0f, 1.0f))
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

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial3Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				t3FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Tutorial3Move();
				GameClearReset();
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				sceneChange = 0;

				t3FirstPlayFlag = false;
				t3ClearFlag = true;
			}

			/*if (input->TriggerKey(DIK_R))
			{
				Tutorial3Move();
				Tutorial3Reset();
				sceneNo = 7;
			}*/

			if (input->TriggerKey(DIK_T))
			{
				Tutorial3Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			/*if (input->TriggerKey(DIK_L))
			{
				Tutorial3Move();
				sceneNo = 8;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
			}*/
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
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objButtonGreen1->Update();
		objButtonGreen2->Update();
		objButtonBlue->Update();
		objButtonRed1->Update();

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;

	case 8: //ステージセレクト画面
#pragma region case8 ステージセレクト画面

		camera->SetEye({ -15,0,0 });
		camera->SetTarget({ 0, 0, 0 });

		if (input->TriggerKey(DIK_SPACE) || IsButtonDown(ButtonKind::Button_A))
		{
			PlayFlag = false;
			audio->StopWave("Title.wav");
		}

		if (input->TriggerKey(DIK_D) && stageMoveRight == false && stageMoveLeft == false && stageSelect < 6 ||
			IsButtonDown(ButtonKind::Button_RB) && stageMoveRight == false && stageMoveLeft == false && stageSelect < 6)
		{
			stageMoveRight = true;
			stageSelect++;
		}

		if (input->TriggerKey(DIK_A) && stageMoveRight == false && stageMoveLeft == false && stageSelect > 0 ||
			IsButtonDown(ButtonKind::Button_LB) && stageMoveRight == false && stageMoveLeft == false && stageSelect > 0)
		{
			stageMoveLeft = true;
			stageSelect--;
		}

		if (stageMoveRight == true)
		{
			camera->MoveVector({ +2.0f, 0.0f, 0.0f });
			camera->Update();


			t1BackgroundPosition.x -= 25.6f;
			t2BackgroundPosition.x -= 25.6f;
			t3BackgroundPosition.x -= 25.6f;
			t4BackgroundPosition.x -= 25.6f;
			s1BackgroundPosition.x -= 25.6f;
			s2BackgroundPosition.x -= 25.6f;
			s3BackgroundPosition.x -= 25.6f;

			t1StageSelectFramePosition.x -= 25.6f;
			t2StageSelectFramePosition.x -= 25.6f;
			t3StageSelectFramePosition.x -= 25.6f;
			t4StageSelectFramePosition.x -= 25.6f;
			s1StageSelectFramePosition.x -= 25.6f;
			s2StageSelectFramePosition.x -= 25.6f;
			s3StageSelectFramePosition.x -= 25.6f;

			t1NewLogPosition.x -= 25.6f;
			t2NewLogPosition.x -= 25.6f;
			t3NewLogPosition.x -= 25.6f;
			t4NewLogPosition.x -= 25.6f;
			s1NewLogPosition.x -= 25.6f;
			s2NewLogPosition.x -= 25.6f;
			s3NewLogPosition.x -= 25.6f;

			t1ClearLogPosition.x -= 25.6f;
			t2ClearLogPosition.x -= 25.6f;
			t3ClearLogPosition.x -= 25.6f;
			t4ClearLogPosition.x -= 25.6f;
			s1ClearLogPosition.x -= 25.6f;
			s2ClearLogPosition.x -= 25.6f;
			s3ClearLogPosition.x -= 25.6f;

			currentFrame++;

			if (currentFrame >= 50)
			{
				currentFrame = 0;
				stageMoveRight = false;
			}
		}

		if (stageMoveLeft == true)
		{
			camera->MoveVector({ -2.0f, 0.0f, 0.0f });
			camera->Update();

			t1BackgroundPosition.x += 25.6f;
			t2BackgroundPosition.x += 25.6f;
			t3BackgroundPosition.x += 25.6f;
			t4BackgroundPosition.x += 25.6f;
			s1BackgroundPosition.x += 25.6f;
			s2BackgroundPosition.x += 25.6f;
			s3BackgroundPosition.x += 25.6f;

			t1StageSelectFramePosition.x += 25.6f;
			t2StageSelectFramePosition.x += 25.6f;
			t3StageSelectFramePosition.x += 25.6f;
			t4StageSelectFramePosition.x += 25.6f;
			s1StageSelectFramePosition.x += 25.6f;
			s2StageSelectFramePosition.x += 25.6f;
			s3StageSelectFramePosition.x += 25.6f;

			t1NewLogPosition.x += 25.6f;
			t2NewLogPosition.x += 25.6f;
			t3NewLogPosition.x += 25.6f;
			t4NewLogPosition.x += 25.6f;
			s1NewLogPosition.x += 25.6f;
			s2NewLogPosition.x += 25.6f;
			s3NewLogPosition.x += 25.6f;

			t1ClearLogPosition.x += 25.6f;
			t2ClearLogPosition.x += 25.6f;
			t3ClearLogPosition.x += 25.6f;
			t4ClearLogPosition.x += 25.6f;
			s1ClearLogPosition.x += 25.6f;
			s2ClearLogPosition.x += 25.6f;
			s3ClearLogPosition.x += 25.6f;

			currentFrame++;

			if (currentFrame >= 50)
			{
				currentFrame = 0;
				stageMoveLeft = false;
			}
		}

		if (delay)
		{
			delayFrame++;

			if (delayFrame >= 5)
			{
				delay = false;
				delayFrame = 0;
			}
		}

		if (stageMoveLeft == false && stageMoveRight == false && input->TriggerKey(DIK_SPACE) && !delay||
			stageMoveLeft == false && stageMoveRight == false && IsButtonDown(ButtonKind::Button_A) && !delay)
		{
			audio->PlayWave("Stage.wav", Volume, true);
			switch (stageSelect)
			{
			case 0:
				Tutorial1Reset();
				sceneNo = 5;

				break;
			case 1:
				Tutorial2Reset();
				sceneNo = 6;

				break;
			case 2:
				Tutorial3Reset();
				sceneNo = 7;

				break;
			case 3:
				Tutorial4Reset();
				sceneNo = 9;

				break;
			case 4:
				Stage1Reset();
				sceneNo = 1;

				break;
			case 5:
				Stage2Reset();
				sceneNo = 4;

				break;
			case 6:
				Stage3Reset();
				sceneNo = 10;

				break;
			}
		}

		// 移動後の座標をセット
		t1Background->SetPosition(t1BackgroundPosition);
		t2Background->SetPosition(t2BackgroundPosition);
		t3Background->SetPosition(t3BackgroundPosition);
		t4Background->SetPosition(t4BackgroundPosition);
		s1Background->SetPosition(s1BackgroundPosition);
		s2Background->SetPosition(s2BackgroundPosition);
		s3Background->SetPosition(s3BackgroundPosition);

		t1StageSelectFrame->SetPosition(t1StageSelectFramePosition);
		t2StageSelectFrame->SetPosition(t2StageSelectFramePosition);
		t3StageSelectFrame->SetPosition(t3StageSelectFramePosition);
		t4StageSelectFrame->SetPosition(t4StageSelectFramePosition);
		s1StageSelectFrame->SetPosition(s1StageSelectFramePosition);
		s2StageSelectFrame->SetPosition(s2StageSelectFramePosition);
		s3StageSelectFrame->SetPosition(s3StageSelectFramePosition);

		t1NewLog->SetPosition(t1NewLogPosition);
		t2NewLog->SetPosition(t2NewLogPosition);
		t3NewLog->SetPosition(t3NewLogPosition);
		t4NewLog->SetPosition(t4NewLogPosition);
		s1NewLog->SetPosition(s1NewLogPosition);
		s2NewLog->SetPosition(s2NewLogPosition);
		s3NewLog->SetPosition(s3NewLogPosition);

		t1ClearLog->SetPosition(t1ClearLogPosition);
		t2ClearLog->SetPosition(t2ClearLogPosition);
		t3ClearLog->SetPosition(t3ClearLogPosition);
		t4ClearLog->SetPosition(t4ClearLogPosition);
		s1ClearLog->SetPosition(s1ClearLogPosition);
		s2ClearLog->SetPosition(s2ClearLogPosition);
		s3ClearLog->SetPosition(s3ClearLogPosition);

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;

	case 9: // チュートリアル 4
#pragma region case9 チュートリアル4
		if (!beginStage)
		{
			if (t4Time)
			{
				t4Time = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 9;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -12 });
			objClone->SetPosition({ 20, 30, -12 });

			playerPositionTemp = { -20, 30, -12 };
			clonePositionTemp = { 20, 30, -12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerFall->SetRotation({ 0, 320, 0 });
				objCloneFall->SetRotation({ 0, 320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

			//ワープゾーンの回転
			//ワープ入口
			XMFLOAT3 teleRotationIn_1 = objTeleporterIn1->GetRotation();
			teleRotationIn_1.y += WarpRotate;
			objTeleporterIn1->SetRotation(teleRotationIn_1);
			objTeleporterIn1->Update();

			XMFLOAT3 teleRotationIn_2 = objTeleporterIn2->GetRotation();
			teleRotationIn_2.y += WarpRotate;
			objTeleporterIn2->SetRotation(teleRotationIn_2);
			objTeleporterIn2->Update();

			XMFLOAT3 teleRotationIn_3 = objTeleporterIn3->GetRotation();
			teleRotationIn_3.y += WarpRotate;
			objTeleporterIn3->SetRotation(teleRotationIn_3);
			objTeleporterIn3->Update();

			XMFLOAT3 teleRotationIn_4 = objTeleporterIn4->GetRotation();
			teleRotationIn_4.y += WarpRotate;
			objTeleporterIn4->SetRotation(teleRotationIn_4);
			objTeleporterIn4->Update();


			//ワープ出口
			XMFLOAT3 teleRotationOut_1 = objTeleporterOut1->GetRotation();
			teleRotationOut_1.y += WarpRotate;
			objTeleporterOut1->SetRotation(teleRotationOut_1);
			objTeleporterOut1->Update();

			XMFLOAT3 teleRotationOut_2 = objTeleporterOut2->GetRotation();
			teleRotationOut_2.y += WarpRotate;
			objTeleporterOut2->SetRotation(teleRotationOut_2);
			objTeleporterOut2->Update();

			XMFLOAT3 teleRotationOut_3 = objTeleporterOut3->GetRotation();
			teleRotationOut_3.y += WarpRotate;
			objTeleporterOut3->SetRotation(teleRotationOut_3);
			objTeleporterOut3->Update();

			XMFLOAT3 teleRotationOut_4 = objTeleporterOut4->GetRotation();
			teleRotationOut_4.y += WarpRotate;
			objTeleporterOut4->SetRotation(teleRotationOut_4);
			objTeleporterOut4->Update();


			if (intersect(playerPosition, RedButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, RedButton2, 1.0f, 1.0f, 1.0f))
			{
				tutorial4YellowKabe = false;
			}

			if (!tutorial4YellowKabe)
			{
				for (auto object_t4_y : objects_t4_y) {
					XMFLOAT3 objectPosition = object_t4_y->GetPosition();
					object_t4_y->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t4_y->Update();
				}

				for (auto object_t4_y2 : objects_t4_y2) {
					XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
					object_t4_y2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_t4_y2->Update();
				}
			}

			if (intersect(playerPosition, objTeleporterIn1->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objFighter->SetPosition(objTeleporterOut1->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn2->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut2->GetPosition());
			}

			if (intersect(playerPosition, objTeleporterIn3->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objFighter->SetPosition(objTeleporterOut3->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn4->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut4->GetPosition());
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial4Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				t4FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Tutorial4Move();
				GameClearReset();
				sceneChange = 0;

				t4FirstPlayFlag = false;
				t4ClearFlag = true;
			}

			/*if (input->TriggerKey(DIK_R))
			{
				Tutorial4Move();
				Tutorial4Reset();
				sceneNo = 9;
			}*/

			if (input->TriggerKey(DIK_T))
			{
				Tutorial4Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			/*if (input->TriggerKey(DIK_L))
			{
				Tutorial4Move();
				sceneNo = 8;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
			}*/
		}

		for (auto object_t4_1 : objects_t4_1)
		{
			object_t4_1->Update();
		}

		for (auto object_t4_2 : objects_t4_2)
		{
			object_t4_2->Update();
		}

		for (auto object_t4_y : objects_t4_y)
		{
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2)
		{
			object_t4_y2->Update();
		}

		objButtonRed1->Update();
		objButtonRed2->Update();

		objTeleporterIn1->Update();
		objTeleporterIn2->Update();
		objTeleporterIn3->Update();
		objTeleporterIn4->Update();
		
		objTeleporterOut1->Update();
		objTeleporterOut2->Update();
		objTeleporterOut3->Update();
		objTeleporterOut4->Update();

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;

	case 10: //ステージ　３
#pragma region case10 ステージ３
		if (!beginStage)
		{
			if (thirdTime)
			{
				thirdTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 10;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -12 });
			objClone->SetPosition({ 20, 30, -12 });

			playerPositionTemp = { -20, 30, -12 };
			clonePositionTemp = { 20, 30, -12 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerFall->SetRotation({ 0, 320, 0 });
				objCloneFall->SetRotation({ 0, 320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

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

				//ワープゾーンの回転
				//ワープ入口
				XMFLOAT3 teleRotationIn_1 = objTeleporterIn1->GetRotation();
				teleRotationIn_1.y += WarpRotate;
				objTeleporterIn1->SetRotation(teleRotationIn_1);
				objTeleporterIn1->Update();

				XMFLOAT3 teleRotationIn_2 = objTeleporterIn2->GetRotation();
				teleRotationIn_2.y += WarpRotate;
				objTeleporterIn2->SetRotation(teleRotationIn_2);
				objTeleporterIn2->Update();

				XMFLOAT3 teleRotationIn_3 = objTeleporterIn3->GetRotation();
				teleRotationIn_3.y += WarpRotate;
				objTeleporterIn3->SetRotation(teleRotationIn_3);
				objTeleporterIn3->Update();

				XMFLOAT3 teleRotationIn_4 = objTeleporterIn4->GetRotation();
				teleRotationIn_4.y += WarpRotate;
				objTeleporterIn4->SetRotation(teleRotationIn_4);
				objTeleporterIn4->Update();


				//ワープ出口
				XMFLOAT3 teleRotationOut_1 = objTeleporterOut1->GetRotation();
				teleRotationOut_1.y += WarpRotate;
				objTeleporterOut1->SetRotation(teleRotationOut_1);
				objTeleporterOut1->Update();

				XMFLOAT3 teleRotationOut_2 = objTeleporterOut2->GetRotation();
				teleRotationOut_2.y += WarpRotate;
				objTeleporterOut2->SetRotation(teleRotationOut_2);
				objTeleporterOut2->Update();

				XMFLOAT3 teleRotationOut_3 = objTeleporterOut3->GetRotation();
				teleRotationOut_3.y += WarpRotate;
				objTeleporterOut3->SetRotation(teleRotationOut_3);
				objTeleporterOut3->Update();

				XMFLOAT3 teleRotationOut_4 = objTeleporterOut4->GetRotation();
				teleRotationOut_4.y += WarpRotate;
				objTeleporterOut4->SetRotation(teleRotationOut_4);
				objTeleporterOut4->Update();
			

			if (intersect(playerPosition, objTeleporterIn1->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objFighter->SetPosition(objTeleporterOut1->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn2->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut2->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn3->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut3->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn4->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut4->GetPosition());
			}

			if (intersect(enemyPosition, RedButton1, 1.0f, 1.0f, 1.0f))
			{
				stage3YellowKabe1 = false;
			}

			if (!stage3YellowKabe1)
			{
				for (auto object_s3_y1_1 : objects_s3_y1_1) {
					XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
					object_s3_y1_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s3_y1_1->Update();
				}
			}

			if (intersect(playerPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				stage3YellowKabe2 = false;
			}

			if (!stage3YellowKabe2)
			{
				for (auto object_s3_y2_1 : objects_s3_y2_1) {
					XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
					object_s3_y2_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s3_y2_1->Update();
				}
			}

			if (intersect(enemyPosition, GreenButton1, 1.0f, 1.0f, 1.0f))
			{
				stage3YellowKabe3 = false;
			}

			if (!stage3YellowKabe3)
			{
				for (auto object_s3_y2_2 : objects_s3_y2_2) {
					XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
					object_s3_y2_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s3_y2_2->Update();
				}
			}

			if (intersect(playerPosition,YellowButton, 1.0f, 1.0f, 1.0f))
			{
				stage3YellowKabe4 = false;
			}

			if (!stage3YellowKabe4)
			{
				for (auto object_s3_y1_2 : objects_s3_y1_2) {
					XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
					object_s3_y1_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s3_y1_2->Update();
				}
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage3Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s3FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage3Move();
				GameClearReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				gameClear->Initialize();

				s3FirstPlayFlag = false;
				s3ClearFlag = true;
			}

			/*if (input->TriggerKey(DIK_R))
			{
				Stage3Move();
				Stage3Reset();
				sceneNo = 10;
			}*/

			if (input->TriggerKey(DIK_T))
			{
				Stage3Move();
				titleScene->Initialize();
				sceneNo = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
			}

			/*if (input->TriggerKey(DIK_L))
			{
				Stage3Move();
				sceneNo = 8;
				audio->StopWave("Stage.wav");
				audio->PlayWave("Title.wav", Volume, true);
				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
			}*/
		}

		for (auto object_s3_1 : objects_s3_1)
		{
			object_s3_1->Update();
		}

		for (auto object_s3_2 : objects_s3_2)
		{
			object_s3_2->Update();
		}

		for (auto object_s3_y1_1 : objects_s3_y1_1)
		{
			object_s3_y1_1->Update();
		}

		for (auto object_s3_y2_1 : objects_s3_y2_1)
		{
			object_s3_y2_1->Update();
		}

		for (auto object_s3_y1_2 : objects_s3_y1_2)
		{
			object_s3_y1_2->Update();
		}

		for (auto object_s3_y2_2 : objects_s3_y2_2)
		{
			object_s3_y2_2->Update();
		}
		
		objButtonBlue->Update();
		objButtonGreen1->Update();
		objButtonYellow->Update();
		objButtonRed1->Update();

		objTeleporterIn1->Update();
		objTeleporterIn2->Update();
		objTeleporterIn3->Update();
		objTeleporterIn4->Update();
		

		objTeleporterOut1->Update();
		objTeleporterOut2->Update();
		objTeleporterOut3->Update();
		objTeleporterOut4->Update();
		

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();
		objPlayerFall->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;
	}

#pragma endregion

#pragma region DebugLog
	//Debug Start
	//XMFLOAT3 eye = camera->GetEye();

	//char msgbuf[256];
	//char msgbuf2[256];
	//char msgbuf3[256];

	//sprintf_s(msgbuf, 256, "Last Scene: %d\n", lastScene);
	//sprintf_s(msgbuf2, 256, "Player Y: %f\n", playerPosition.y);
	//sprintf_s(msgbuf3, 256, "Player Z: %f\n", playerPosition.z);
	//OutputDebugStringA(msgbuf);
	//OutputDebugStringA(msgbuf2);
	//OutputDebugStringA(msgbuf3);
	//Debug End
#pragma endregion

	//Left Side Eye: {-40, 25, 0}
	//Right Side Eye: {40, 25, 0}
	//Normal Eye: {0, 25, -30}
	//Opposite Side: {0, 25, 30}
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
		GuideR->Draw();
		Guide_LRB->Draw();
		break;
	case 2:
		if (sceneChange == 0)
		{
			//Stage1Reset();
			//audio->PlayWave("GemeClear.wav");
			gameClear->Initialize();
			sceneChange = 1;
		}


		gameClear->Draw();
		break;
	case 3:
		if (sceneChange == 0)
		{
			//5Stage1Reset();
			gameOver->Initialize();
			sceneChange = 1;
		}

		gameOver->Draw();
		break;
	case 4:
		spriteBG->Draw();
		GuideR->Draw();
		Guide_LRB->Draw();
		break;
	case 5:
		spriteBG->Draw();
		GuideR->Draw();
		Guide_LRB->Draw();
		break;
	case 6:
		spriteBG->Draw();
		GuideR->Draw();
		Guide_LRB->Draw();
		break;
	case 7:
		spriteBG->Draw();
		GuideR->Draw();
		Guide_LRB->Draw();
		break;
	case 8:
		spriteBG->Draw();
		break;
	case 9:
		spriteBG->Draw();
		GuideR->Draw();
		Guide_LRB->Draw();
		break;
	case 10:
		spriteBG->Draw();
		GuideR->Draw();
		Guide_LRB->Draw();
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

	if (sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && beginStage && marker)
	{
		objPlayerMarker->Draw();
	}

	switch (sceneNo)
	{
	case 0:
		objPlayerFight->Draw(cmdList);
		objCloneFight->Draw(cmdList);
		objSkydome->Draw();
		break;
	case 1:
		// 3Dオブクジェクトの描画 Drawing 3D objects
		for (auto object : objects) {
			object->Draw();
		}

		for (auto object_2 : objects_2) {
			object_2->Draw();
		}

		
		objButtonBomb1->Draw();
		objButtonBomb2->Draw();

		if (beginStage)
		{
			if (playerBulletF == true)
			{
				objTempBullet->Draw();
			}

			if (enemyBulletF == true)
			{
				objTempBulletE->Draw();
			}
		}

		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
		}
		
		objSkydome->Draw();
		break;
	case 2:
		objPlayerWin->Draw(cmdList);
		objSkydome->Draw();
		//objMenuSelection->Draw();
		break;
	case 3:
		objPlayerLose->Draw(cmdList);
		//objMenuSelection->Draw();
		break;
	case 4:
		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
		}

		for (auto object_s2_1 : objects_s2_1) {
			object_s2_1->Draw();
		}

		for (auto object_s2_2 : objects_s2_2) {
			object_s2_2->Draw();
		}

		if (stage2YellowKabe)
		{
			objButtonRed1->Draw();
			objButtonRed2->Draw();

			for (auto object_s2_y : objects_s2_y) {
				object_s2_y->Draw();
			}

			for (auto object_s2_y2 : objects_s2_y2) {
				object_s2_y2->Draw();
			}
		}

		if (!stage2Switch)
		{
			//objTempYellowTrigger1->Draw();
			objButtonFloor->Draw();

			for (auto object_s2_s : objects_s2_s) {
				object_s2_s->Draw();
			}

			for (auto object_s2_s2 : objects_s2_s2) {
				object_s2_s2->Draw();
			}
		}

		objSkydome->Draw();

		break;
	case 5:
		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
		}

		for (auto object_t1_1 : objects_t1_1) {
			object_t1_1->Draw();
		}

		for (auto object_t1_2 : objects_t1_2) {
			object_t1_2->Draw();
		}

		objSkydome->Draw();

		break;
	case 6:
		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
		}

		for (auto object_t2_1 : objects_t2_1)
		{
			object_t2_1->Draw();
		}

		for (auto object_t2_2 : objects_t2_2)
		{
			object_t2_2->Draw();
		}

		objSkydome->Draw();

		break;
	case 7:
		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
			//objTempYellowTrigger2->Draw();
			objButtonRed1->Draw();

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
			objButtonBlue->Draw();

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
			objButtonGreen1->Draw();
			objButtonGreen2->Draw();

			for (auto object_t3_y1_3 : objects_t3_y1_3)
			{
				object_t3_y1_3->Draw();
			}

			for (auto object_t3_y2_3 : objects_t3_y2_3)
			{
				object_t3_y2_3->Draw();
			}
		}

		objSkydome->Draw();

		break;
	case 8:

		objSkydome->Draw();

		break;
	case 9:
		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
		}

		for (auto object_t4_1 : objects_t4_1)
		{
			object_t4_1->Draw();
		}

		for (auto object_t4_2 : objects_t4_2)
		{
			object_t4_2->Draw();
		}

		if (tutorial4YellowKabe)
		{
			objButtonRed1->Draw();
			objButtonRed2->Draw();

			for (auto object_t4_y : objects_t4_y)
			{
				object_t4_y->Draw();
			}

			for (auto object_t4_y2 : objects_t4_y2)
			{
				object_t4_y2->Draw();
			}
		}

		objTeleporterIn1->Draw();
		objTeleporterIn2->Draw();
		objTeleporterIn3->Draw();
		objTeleporterIn4->Draw();

		objTeleporterOut1->Draw();
		objTeleporterOut2->Draw();
		objTeleporterOut3->Draw();
		objTeleporterOut4->Draw();

		objSkydome->Draw();
		break;
	case 10:
		if (falling && beginStage)
		{
			objPlayerFall->Draw(cmdList);
			objCloneFall->Draw(cmdList);
		}
		else if (!falling && beginStage)
		{
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
		}

		for (auto object_s3_1 : objects_s3_1)
		{
			object_s3_1->Draw();
		}

		for (auto object_s3_2 : objects_s3_2)
		{
			object_s3_2->Draw();
		}

		if (stage3YellowKabe1)
		{
			objButtonRed1->Draw();

			for (auto object_s3_y1_1 : objects_s3_y1_1)
			{
				object_s3_y1_1->Draw();
			}
		}

		if (stage3YellowKabe2)
		{
			objButtonBlue->Draw();

			for (auto object_s3_y2_1 : objects_s3_y2_1)
			{
				object_s3_y2_1->Draw();
			}
		}

		if (stage3YellowKabe3)
		{
			objButtonGreen1->Draw();

			for (auto object_s3_y2_2 : objects_s3_y2_2)
			{
				object_s3_y2_2->Draw();
			}
		}

		if (stage3YellowKabe4)
		{
			//objTempYellowTrigger2->Draw();
			objButtonYellow->Draw();

			for (auto object_s3_y1_2 : objects_s3_y1_2)
			{
				object_s3_y1_2->Draw();
			}
		}

		objTeleporterIn1->Draw();
		objTeleporterIn2->Draw();
		objTeleporterIn3->Draw();
		objTeleporterIn4->Draw();

		objTeleporterOut1->Draw();
		objTeleporterOut2->Draw();
		objTeleporterOut3->Draw();
		objTeleporterOut4->Draw();

		objSkydome->Draw();
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

#pragma region 前景スプライト描画 Foreground sprite drawing
	// 前景スプライト描画前処理 Foreground sprite drawing pre-processing
	Sprite::PreDraw(cmdList);

	// ここに前景スプライトの描画処理を追加できる You can add foreground sprite drawing processing here

	// 描画 drawing
	switch (sceneNo)
	{
	case 0:

		// Mirror->Draw();
		TitleLog->Draw();
		Press_A->Draw();
		break;
	case 1:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_1->Draw();
		break;
	case 2:
		StageClearLog->Draw();
		GameOverGameClearSelectBar->Draw();
		NextStageLog->Draw();
		StageClearStageSelectLog->Draw();
		break;
	case 3:
		GameOverLog->Draw();
		GameOverGameClearSelectBar->Draw();
		ReStateLog->Draw();
		GameOverStageSelectLog->Draw();
		break;
	case 4:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_2->Draw();
		break;
	case 5:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_2->Draw();
		break;
	case 6:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_2->Draw();
		break;
	case 7:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_2->Draw();
		break;
	case 8:
		StageSelectLog->Draw();

		t1Background->Draw();
		t2Background->Draw();
		t3Background->Draw();
		t4Background->Draw();
		s1Background->Draw();
		s2Background->Draw();
		s3Background->Draw();

		t1StageSelectFrame->Draw();
		t2StageSelectFrame->Draw();
		t3StageSelectFrame->Draw();
		t4StageSelectFrame->Draw();
		s1StageSelectFrame->Draw();
		s2StageSelectFrame->Draw();
		s3StageSelectFrame->Draw();

		// NEWロゴの表示
		if (t1FirstPlayFlag == true)
		{
			t1NewLog->Draw();
		}
		
		if (t2FirstPlayFlag == true)
		{
			t2NewLog->Draw();
		}

		if (t3FirstPlayFlag == true)
		{
			t3NewLog->Draw();
		}

		if (t4FirstPlayFlag == true)
		{
			t4NewLog->Draw();
		}
	
		if (s1FirstPlayFlag == true)
		{
			s1NewLog->Draw();
		}

		if (s2FirstPlayFlag == true)
		{
			s2NewLog->Draw();
		}

		if (s3FirstPlayFlag == true)
		{
			s3NewLog->Draw();
		}

		// Clearロゴの表示
		if (t1ClearFlag == true)
		{
			t1ClearLog->Draw();
		}

		if (t2ClearFlag == true)
		{
			t2ClearLog->Draw();
		}

		if (t3ClearFlag == true)
		{
			t3ClearLog->Draw();
		}

		if (t4ClearFlag == true)
		{
			t4ClearLog->Draw();
		}
		
		if (s1ClearFlag == true)
		{
			s1ClearLog->Draw();
		}

		if (s2ClearFlag == true)
		{
			s2ClearLog->Draw();
		}

		if (s3ClearFlag == true)
		{
			s3ClearLog->Draw();
		}

		GuideA->Draw();
		GuideB->Draw();

		switch (stageSelect)
		{
		case 0:
			StageSelectRB->Draw();
			break;

		case 1:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 2:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 3:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 4:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 5:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 6:
			StageSelectLB->Draw();
			break;
		}
		break;
	case 9:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_2->Draw();
		break;
	case 10:
		GuideR->Draw();
		Guide_LRB->Draw();
		Order_2->Draw();
		break;
	}

	if (pause)
	{
		pause4->Draw();
		pause1->Draw();
		pause2->Draw();
		pause3->Draw();
	}

	// デバッグテキストの描画 Debug text drawing
	// debugText.DrawAll(cmdList);
	// 
	// スプライト描画後処理 Post-processing of sprite drawing
	Sprite::PostDraw();
#pragma endregion
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

void GameScene::SceneSelectionReset()
{
	camera->SetEye(originalCamera);
	camera->SetTarget({ 0, 1, 0 });
	camera->Update();

	//stageSelect = 0;

	stageMoveLeft = false;
	stageMoveRight = false;

	currentFrame = 0;

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

	objFighter->SetPosition({ -20,30,12 });
	objClone->SetPosition({ 20,30,12 });

	falling = false;

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
	cameraChange = false;

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

	objFighter->SetPosition({ -20, 30,-12 });
	objClone->SetPosition({ 20, 30, -12 });

	falling = false;

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
	cameraChange = false;

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

	objFighter->SetPosition({ -20, 30, -12 });
	objClone->SetPosition({ 20, 30, -12 });

	falling = false;

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

	objButtonGreen1->SetPosition({ -8.0f, 0, 6.0f });
	objButtonGreen2->SetPosition({ 8.0f, 0, 6.0f });
	objButtonBlue->SetPosition({ 20.0f, 0, -3.0f });
	objButtonRed1->SetPosition({ 8.0f, 0, -12.0f });

	cameraMove = 1;
	cameraChange = false;

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

	if (tutorial3YellowKabe1)
	{
		for (auto object_t3_y1_1 : objects_t3_y1_1) {
			XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
			object_t3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1) {
			XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
			object_t3_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t3_y2_1->Update();
		}
	}
	else
	{
		for (auto object_t3_y1_1 : objects_t3_y1_1) {
			XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
			object_t3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1) {
			XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
			object_t3_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_t3_y2_1->Update();
		}
	}

	if (tutorial3YellowKabe2)
	{
		for (auto object_t3_y1_2 : objects_t3_y1_2) {
			XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
			object_t3_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t3_y1_2->Update();
		}

		for (auto object_t3_y2_2 : objects_t3_y2_2) {
			XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
			object_t3_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t3_y2_2->Update();
		}
	}
	else
	{
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
	}

	if (tutorial3YellowKabe3)
	{
		for (auto object_t3_y1_3 : objects_t3_y1_3) {
			XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
			object_t3_y1_3->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t3_y1_3->Update();
		}

		for (auto object_t3_y2_3 : objects_t3_y2_3) {
			XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
			object_t3_y2_3->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t3_y2_3->Update();
		}
	}
	else
	{
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
}

void GameScene::Tutorial4Reset()
{
	if (!t4Time)
	{
		for (auto object_t4_1 : objects_t4_1) {
			XMFLOAT3 objectPosition = object_t4_1->GetPosition();
			object_t4_1->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t4_1->Update();
		}

		for (auto object_t4_2 : objects_t4_2) {
			XMFLOAT3 objectPosition = object_t4_2->GetPosition();
			object_t4_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_t4_2->Update();
		}

		for (auto object_t4_y : objects_t4_y) {
			XMFLOAT3 objectPosition = object_t4_y->GetPosition();
			object_t4_y->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2) {
			XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
			object_t4_y2->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_t4_y2->Update();
		}
	}

	objFighter->SetPosition({ -20, 30, -12 });
	objClone->SetPosition({ 20, 30, -12 });

	objTeleporterIn1->SetPosition({ -14.0f, 0.0f, -9.0f });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn2->SetPosition({ 14.0f, 0.0f, -9.0f });
	objTeleporterIn3->SetPosition({ -14.0f, 0.0f, 12.0f });
	objTeleporterIn4->SetPosition({ 14.0f, 0.0f, 12.0f });

	objTeleporterOut1->SetPosition({ -14.0f, 0.0f, 6.0f });
	objTeleporterOut2->SetPosition({ 14.0f, 0.0f, 6.0f });
	objTeleporterOut3->SetPosition({ -8.0f, 0.0f, -12.0f });
	objTeleporterOut4->SetPosition({ 8.0f, 0.0f, -12.0f });

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	tutorial4YellowKabe = true;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	objButtonRed1->SetPosition({ -14.0f, 0, 9.0f });
	objButtonRed2->SetPosition({ 14.0f, 0, 9.0f });

	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Tutorial4Move()
{
	for (auto object_t4_1 : objects_t4_1) {
		XMFLOAT3 objectPosition = object_t4_1->GetPosition();
		object_t4_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t4_1->Update();
	}

	for (auto object_t4_2 : objects_t4_2) {
		XMFLOAT3 objectPosition = object_t4_2->GetPosition();
		object_t4_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_t4_2->Update();
	}

	if (!tutorial4YellowKabe)
	{
		for (auto object_t4_y : objects_t4_y) {
			XMFLOAT3 objectPosition = object_t4_y->GetPosition();
			object_t4_y->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2) {
			XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
			object_t4_y2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_t4_y2->Update();
		}
	}
	else
	{
		for (auto object_t4_y : objects_t4_y) {
			XMFLOAT3 objectPosition = object_t4_y->GetPosition();
			object_t4_y->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2) {
			XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
			object_t4_y2->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_t4_y2->Update();
		}
	}
}

void GameScene::Stage1Reset()
{
	if (!firstTime)
	{
		for (auto object : objects)
		{
			XMFLOAT3 objectPosition = object->GetPosition();
			object->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object->Update();
		}
		for (auto object_2 : objects_2)
		{
			XMFLOAT3 objectPosition = object_2->GetPosition();
			object_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_2->Update();
		}
	}

	objFighter->SetPosition({ -12,30,-12 });
	objClone->SetPosition({ 12,30,-12 });

	objButtonBomb1->SetPosition({ -17.0f, 0, 0 });
	objButtonBomb2->SetPosition({ 17.0f, 0, 0 });

	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	falling = false;

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
	cameraChange = false;

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

		for (auto object_s2_y : objects_s2_y) {
			XMFLOAT3 objectPosition = object_s2_y->GetPosition();
			object_s2_y->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s2_y->Update();
		}

		for (auto object_s2_y2 : objects_s2_y2) {
			XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
			object_s2_y2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s2_y2->Update();
		}

		for (auto object_s2_s : objects_s2_s) {
			XMFLOAT3 objectPosition = object_s2_s->GetPosition();
			object_s2_s->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s2_s->Update();
		}

		for (auto object_s2_s2 : objects_s2_s2) {
			XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
			object_s2_s2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s2_s2->Update();
		}
	}

	objFighter->SetPosition({ -22,30,-12 });
	objClone->SetPosition({ 22,30,-12 });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	objButtonRed1->SetPosition({ -13.0f, 0, -12.0f });
	objButtonRed2->SetPosition({ 10.0f, 0, -9.0f });
	objButtonFloor->SetPosition({ -7.0f, 0, 6.0f });

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	cameraMove = 1;
	cameraChange = false;

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

	if (!stage2YellowKabe)
	{
		for (auto object_s2_y : objects_s2_y) {
			XMFLOAT3 objectPosition = object_s2_y->GetPosition();
			object_s2_y->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_y->Update();
		}

		for (auto object_s2_y2 : objects_s2_y2) {
			XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
			object_s2_y2->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_y2->Update();
		}
	}
	else
	{
		for (auto object_s2_y : objects_s2_y) {
			XMFLOAT3 objectPosition = object_s2_y->GetPosition();
			object_s2_y->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_y->Update();
		}

		for (auto object_s2_y2 : objects_s2_y2) {
			XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
			object_s2_y2->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_y2->Update();
		}
	}

	if (stage2Switch)
	{
		for (auto object_s2_s : objects_s2_s) {
			XMFLOAT3 objectPosition = object_s2_s->GetPosition();
			object_s2_s->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_s->Update();
		}

		for (auto object_s2_s2 : objects_s2_s2) {
			XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
			object_s2_s2->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_s2->Update();
		}
	}
	else
	{
		for (auto object_s2_s : objects_s2_s) {
			XMFLOAT3 objectPosition = object_s2_s->GetPosition();
			object_s2_s->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_s->Update();
		}

		for (auto object_s2_s2 : objects_s2_s2) {
			XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
			object_s2_s2->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
			object_s2_s2->Update();
		}
	}
}

void GameScene::Stage3Reset()
{
	if (!thirdTime)
	{
		for (auto object_s3_1 : objects_s3_1) {
			XMFLOAT3 objectPosition = object_s3_1->GetPosition();
			object_s3_1->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_s3_1->Update();
		}

		for (auto object_s3_2 : objects_s3_2) {
			XMFLOAT3 objectPosition = object_s3_2->GetPosition();
			object_s3_2->SetPosition({ objectPosition.x, objectPosition.y + 50.0f, objectPosition.z });
			object_s3_2->Update();
		}

		for (auto object_s3_y1_1 : objects_s3_y1_1) {
			XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
			object_s3_y1_1->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_s3_y1_1->Update();
		}

		for (auto object_s3_y2_1 : objects_s3_y2_1) {
			XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
			object_s3_y2_1->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_s3_y2_1->Update();
		}

		for (auto object_s3_y1_2 : objects_s3_y1_2) {
			XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
			object_s3_y1_2->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_s3_y1_2->Update();
		}

		for (auto object_s3_y2_2 : objects_s3_y2_2) {
			XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
			object_s3_y2_2->SetPosition({ objectPosition.x, objectPosition.y + 150.0f, objectPosition.z });
			object_s3_y2_2->Update();
		}
	}

	objFighter->SetPosition({ -20, 30, -12 });
	objClone->SetPosition({ 20, 30, -12 });

	objTeleporterIn1->SetPosition({ -20.0f, 0.0f, -3.0f });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn2->SetPosition({ 5.0f, 0.0f, -12.0f });
	objTeleporterIn3->SetPosition({ -11.0f, 0.0f, -9.0f });
	objTeleporterIn4->SetPosition({ 5.0f, 0.0f, 0.0f });

	objTeleporterOut1->SetPosition({ 11.0f, 0.0f, 12.0f });
	objTeleporterOut2->SetPosition({ -17.0f, 0.0f, 6.0f });
	objTeleporterOut3->SetPosition({ 11.0f, 0.0f, 0.0f });
	objTeleporterOut4->SetPosition({ -8.0f, 0.0f, 0.0f });

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	stage3YellowKabe1 = true;
	stage3YellowKabe2 = true;
	stage3YellowKabe3 = true;
	stage3YellowKabe4 = true;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	objButtonBlue->SetPosition({ 14.0f, 0, 9.0f });
	objButtonRed1->SetPosition({ 14.0f, 0, -9.0f });
	objButtonGreen1->SetPosition({ 5.0f, 0, -9.0f });
	objButtonYellow->SetPosition({ 17.0f, 0, -3.0f });

	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Stage3Move()
{
	for (auto object_s3_1 : objects_s3_1) {
		XMFLOAT3 objectPosition = object_s3_1->GetPosition();
		object_s3_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s3_1->Update();
	}

	for (auto object_s3_2 : objects_s3_2) {
		XMFLOAT3 objectPosition = object_s3_2->GetPosition();
		object_s3_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
		object_s3_2->Update();
	}

	if (!stage3YellowKabe1)
	{
		for (auto object_s3_y1_1 : objects_s3_y1_1) {
			XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
			object_s3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_s3_y1_1->Update();
		}
	}
	else
	{
		for (auto object_s3_y1_1 : objects_s3_y1_1) {
			XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
			object_s3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_s3_y1_1->Update();
		}
	}

	if (!stage3YellowKabe2)
	{
		for (auto object_s3_y2_1 : objects_s3_y2_1) {
			XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
			object_s3_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_s3_y2_1->Update();
		}
	}
	else
	{
		for (auto object_s3_y2_1 : objects_s3_y2_1) {
			XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
			object_s3_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_s3_y2_1->Update();
		}
	}

	if (!stage3YellowKabe3)
	{
		for (auto object_s3_y2_2 : objects_s3_y2_2) {
			XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
			object_s3_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_s3_y2_2->Update();
		}
	}
	else
	{
		for (auto object_s3_y2_2 : objects_s3_y2_2) {
			XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
			object_s3_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_s3_y2_2->Update();
		}
	}

	if (!stage3YellowKabe4)
	{
		for (auto object_s3_y1_2 : objects_s3_y1_2) {
			XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
			object_s3_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 50.0f, objectPosition.z });
			object_s3_y1_2->Update();
		}
	}
	else
	{
		for (auto object_s3_y1_2 : objects_s3_y1_2) {
			XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
			object_s3_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 150.0f, objectPosition.z });
			object_s3_y1_2->Update();
		}
	}
}

void GameScene::GameClearReset()
{
	GameOverGameClearSelectBar->SetPosition({ -250.0f,350.0f });
	menuSelection = 0;
	menuMoving = false;
}

void GameScene::GameOverReset()
{
	GameOverGameClearSelectBar->SetPosition({ 0.0f, 470.0f });
	menuSelection = 0;
	menuMoving = false;
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