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
extern int playFlag = 0;

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
	safe_delete(TutorialBG);
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
	audio->LoadWave("Switch.wav");
	audio->LoadWave("Decision.wav");
	audio->LoadWave("Esc.wav");
	audio->LoadWave("Warp.wav");
	audio->LoadWave("Walk.wav");
	audio->LoadWave("OpenDoor.wav");
	audio->LoadWave("MoveCursor.wav");
	audio->LoadWave("StageSelection.wav");
	audio->LoadWave("StageRotation.wav");

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
	objButtonBlue2 = Object3d::Create();
	objButtonGreen1 = Object3d::Create();
	objButtonGreen2 = Object3d::Create();
	objButtonYellow = Object3d::Create();
	objButtonYellow2 = Object3d::Create();
	objButtonBomb1 = Object3d::Create();
	objButtonBomb2 = Object3d::Create();
	objButtonFloor = Object3d::Create();

	// ワープ
	objTeleporterIn1 = Object3d::Create();
	objTeleporterIn2 = Object3d::Create();
	objTeleporterIn3 = Object3d::Create();
	objTeleporterIn4 = Object3d::Create();
	objTeleporterIn5 = Object3d::Create();
	objTeleporterOut1 = Object3d::Create();
	objTeleporterOut2 = Object3d::Create();
	objTeleporterOut3 = Object3d::Create();
	objTeleporterOut4 = Object3d::Create();
	objTeleporterOut5 = Object3d::Create();

	// 扉
	objDoorBase = Object3d::Create();
	objDoorRed = Object3d::Create();
	objDoorBlue = Object3d::Create();
	objDoorGreen = Object3d::Create();
	objDoorYellow = Object3d::Create();

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

	Order_1 = Sprite::Create(7, { 0.0f,10.0f });

	// テクスチャ8番に読み込み Load into texture # 2
	Sprite::LoadTexture(8, L"Resources/Sprite/order2.png");

	Order_2 = Sprite::Create(8, { 0.0f,10.0f });

	Sprite::LoadTexture(9, L"Resources/Sprite/press_a.png");

	Press_A = Sprite::Create(9, { 0.0f,500.0f });

	Sprite::LoadTexture(10, L"Resources/Sprite/Mirror.png");

	TutorialBG = Sprite::Create(10, { 0.0f,0.0f });

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

	Sprite::LoadTexture(65, L"Resources/Sprite/T5.png");
	t5Background = Sprite::Create(65, t5BackgroundPosition);
	t5Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(21, L"Resources/Sprite/S1.png");
	s1Background = Sprite::Create(21, s1BackgroundPosition);
	s1Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(22, L"Resources/Sprite/S2.png");
	s2Background = Sprite::Create(22, s2BackgroundPosition);
	s2Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(24, L"Resources/Sprite/S3.png");
	s3Background = Sprite::Create(24, s3BackgroundPosition);
	s3Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(68, L"Resources/Sprite/S4.png");
	s4Background = Sprite::Create(68, s4BackgroundPosition);
	s4Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(66, L"Resources/Sprite/S5.png");
	s5Background = Sprite::Create(66, s5BackgroundPosition);
	s5Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(67, L"Resources/Sprite/S6.png");
	s6Background = Sprite::Create(67, s6BackgroundPosition);
	s6Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(69, L"Resources/Sprite/S7.png");
	s7Background = Sprite::Create(69, s7BackgroundPosition);
	s7Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(70, L"Resources/Sprite/S8.png");
	s8Background = Sprite::Create(70, s8BackgroundPosition);
	s8Background->SetSize(SpriteStagSize);

	Sprite::LoadTexture(71, L"Resources/Sprite/S9.png");
	s9Background = Sprite::Create(71, s9BackgroundPosition);
	s9Background->SetSize(SpriteStagSize);

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
	t5StageSelectFrame = Sprite::Create(28, { t5StageSelectFramePosition });
	s1StageSelectFrame = Sprite::Create(28, { s1StageSelectFramePosition });
	s2StageSelectFrame = Sprite::Create(28, { s2StageSelectFramePosition });
	s3StageSelectFrame = Sprite::Create(28, { s3StageSelectFramePosition });
	s4StageSelectFrame = Sprite::Create(28, { s4StageSelectFramePosition });
	s5StageSelectFrame = Sprite::Create(28, { s5StageSelectFramePosition });
	s6StageSelectFrame = Sprite::Create(28, { s6StageSelectFramePosition });
	s7StageSelectFrame = Sprite::Create(28, { s7StageSelectFramePosition });
	s8StageSelectFrame = Sprite::Create(28, { s8StageSelectFramePosition });
	s9StageSelectFrame = Sprite::Create(28, { s9StageSelectFramePosition });

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

	t5NewLog = Sprite::Create(31, { t5NewLogPosition });
	t5NewLog->SetRotation({ -30.0f });

	s1NewLog = Sprite::Create(31, { s1NewLogPosition });
	s1NewLog->SetRotation({ -30.0f });

	s2NewLog = Sprite::Create(31, { s2NewLogPosition });
	s2NewLog->SetRotation({ -30.0f });

	s3NewLog = Sprite::Create(31, { s3NewLogPosition });
	s3NewLog->SetRotation({ -30.0f });

	s4NewLog = Sprite::Create(31, { s4NewLogPosition });
	s4NewLog->SetRotation({ -30.0f });

	s5NewLog = Sprite::Create(31, { s5NewLogPosition });
	s5NewLog->SetRotation({ -30.0f });

	s6NewLog = Sprite::Create(31, { s6NewLogPosition });
	s6NewLog->SetRotation({ -30.0f });

	s7NewLog = Sprite::Create(31, { s7NewLogPosition });
	s7NewLog->SetRotation({ -30.0f });

	s8NewLog = Sprite::Create(31, { s8NewLogPosition });
	s8NewLog->SetRotation({ -30.0f });

	s9NewLog = Sprite::Create(31, { s9NewLogPosition });
	s9NewLog->SetRotation({ -30.0f });

	Sprite::LoadTexture(32, L"Resources/Sprite/Clear.png");
	t1ClearLog = Sprite::Create(32, { t1ClearLogPosition });
	t1ClearLog->SetRotation({ -30.0f });

	t2ClearLog = Sprite::Create(32, { t2ClearLogPosition });
	t2ClearLog->SetRotation({ -30.0f });

	t3ClearLog = Sprite::Create(32, { t3ClearLogPosition });
	t3ClearLog->SetRotation({ -30.0f });

	t4ClearLog = Sprite::Create(32, { t4ClearLogPosition });
	t4ClearLog->SetRotation({ -30.0f });

	t5ClearLog = Sprite::Create(32, { t5ClearLogPosition });
	t5ClearLog->SetRotation({ -30.0f });

	s1ClearLog = Sprite::Create(32, { s1ClearLogPosition });
	s1ClearLog->SetRotation({ -30.0f });

	s2ClearLog = Sprite::Create(32, { s2ClearLogPosition });
	s2ClearLog->SetRotation({ -30.0f });

	s3ClearLog = Sprite::Create(32, { s3ClearLogPosition });
	s3ClearLog->SetRotation({ -30.0f });

	s4ClearLog = Sprite::Create(32, { s4ClearLogPosition });
	s4ClearLog->SetRotation({ -30.0f });

	s5ClearLog = Sprite::Create(32, { s5ClearLogPosition });
	s5ClearLog->SetRotation({ -30.0f });

	s6ClearLog = Sprite::Create(32, { s6ClearLogPosition });
	s6ClearLog->SetRotation({ -30.0f });

	s7ClearLog = Sprite::Create(32, { s7ClearLogPosition });
	s7ClearLog->SetRotation({ -30.0f });

	s8ClearLog = Sprite::Create(32, { s8ClearLogPosition });
	s8ClearLog->SetRotation({ -30.0f });

	s9ClearLog = Sprite::Create(32, { s9ClearLogPosition });
	s9ClearLog->SetRotation({ -30.0f });

	Sprite::LoadTexture(33, L"Resources/Sprite/Pause1.png");
	pause1 = Sprite::Create(33, { 820, 200 });

	Sprite::LoadTexture(34, L"Resources/Sprite/Pause2.png");
	pause2 = Sprite::Create(34, { 770, 300 });

	Sprite::LoadTexture(35, L"Resources/Sprite/Pause3.png");
	pause3 = Sprite::Create(35, { 720, 400 });

	Sprite::LoadTexture(36, L"Resources/Sprite/Pause4.png");
	pause4 = Sprite::Create(36, { 550, 200 });

	Sprite::LoadTexture(37, L"Resources/Sprite/GameOverGameClearSelect.png");
	GameOverGameClearSelectBar = Sprite::Create(37, { -250.0f,350.0f });

	Sprite::LoadTexture(38, L"Resources/Sprite/SpeechBubble.png");
	SpeechBubble = Sprite::Create(38, { 0.0f, 360.0f });

	// チュートリアル1
	Sprite::LoadTexture(39, L"Resources/Sprite/Tutorial_1/T1Chat1.png");
	T1Chat1 = Sprite::Create(39, { 0.0f, 360.0f });

	Sprite::LoadTexture(40, L"Resources/Sprite/Tutorial_1/T1Chat2.png");
	T1Chat2 = Sprite::Create(40, { 0.0f, 360.0f });

	Sprite::LoadTexture(41, L"Resources/Sprite/Tutorial_1/T1Chat3.png");
	T1Chat3 = Sprite::Create(41, { 0.0f, 360.0f });

	Sprite::LoadTexture(42, L"Resources/Sprite/Tutorial_1/T1Chat4.png");
	T1Chat4 = Sprite::Create(42, { 0.0f, 360.0f });

	Sprite::LoadTexture(43, L"Resources/Sprite/Tutorial_1/T1Chat5.png");
	T1Chat5 = Sprite::Create(43, { 0.0f, 360.0f });

	// チュートリアル2
	Sprite::LoadTexture(44, L"Resources/Sprite/Tutorial_2/T2Chat1.png");
	T2Chat1 = Sprite::Create(44, { 0.0f, 360.0f });

	Sprite::LoadTexture(45, L"Resources/Sprite/Tutorial_2/T2Chat2.png");
	T2Chat2 = Sprite::Create(45, { 0.0f, 360.0f });

	Sprite::LoadTexture(46, L"Resources/Sprite/Tutorial_2/T2Chat3.png");
	T2Chat3 = Sprite::Create(46, { 0.0f, 360.0f });

	Sprite::LoadTexture(47, L"Resources/Sprite/Tutorial_2/T2Chat4.png");
	T2Chat4 = Sprite::Create(47, { 0.0f, 360.0f });

	Sprite::LoadTexture(48, L"Resources/Sprite/Tutorial_2/T2Chat5.png");
	T2Chat5 = Sprite::Create(48, { 0.0f, 360.0f });

	Sprite::LoadTexture(49, L"Resources/Sprite/Tutorial_2/T2Chat6.png");
	T2Chat6 = Sprite::Create(49, { 0.0f, 360.0f });

	// チュートリアル3
	Sprite::LoadTexture(50, L"Resources/Sprite/Tutorial_3/T3Chat1.png");
	T3Chat1 = Sprite::Create(50, { 0.0f, 360.0f });

	Sprite::LoadTexture(51, L"Resources/Sprite/Tutorial_3/T3Chat2.png");
	T3Chat2 = Sprite::Create(51, { 0.0f, 360.0f });

	Sprite::LoadTexture(52, L"Resources/Sprite/Tutorial_3/T3Chat3.png");
	T3Chat3 = Sprite::Create(52, { 0.0f, 360.0f });

	Sprite::LoadTexture(53, L"Resources/Sprite/Tutorial_3/T3Chat4.png");
	T3Chat4 = Sprite::Create(53, { 0.0f, 360.0f });

	Sprite::LoadTexture(54, L"Resources/Sprite/Tutorial_3/T3Chat5.png");
	T3Chat5 = Sprite::Create(54, { 0.0f, 360.0f });

	Sprite::LoadTexture(55, L"Resources/Sprite/Tutorial_3/T3Chat6.png");
	T3Chat6 = Sprite::Create(55, { 0.0f, 360.0f });

	// チュートリアル4
	Sprite::LoadTexture(56, L"Resources/Sprite/Tutorial_4/T4Chat1.png");
	T4Chat1 = Sprite::Create(56, { 0.0f, 360.0f });

	Sprite::LoadTexture(57, L"Resources/Sprite/Tutorial_4/T4Chat2.png");
	T4Chat2 = Sprite::Create(57, { 0.0f, 360.0f });

	Sprite::LoadTexture(58, L"Resources/Sprite/Tutorial_4/T4Chat3.png");
	T4Chat3 = Sprite::Create(58, { 0.0f, 360.0f });

	Sprite::LoadTexture(59, L"Resources/Sprite/Tutorial_4/T4Chat4.png");
	T4Chat4 = Sprite::Create(59, { 0.0f, 360.0f });

	// チュートリアル5
	Sprite::LoadTexture(60, L"Resources/Sprite/Tutorial_5/T5Chat1.png");
	T5Chat1 = Sprite::Create(60, { 0.0f, 360.0f });

	Sprite::LoadTexture(61, L"Resources/Sprite/Tutorial_5/T5Chat2.png");
	T5Chat2 = Sprite::Create(61, { 0.0f, 360.0f });

	Sprite::LoadTexture(62, L"Resources/Sprite/Tutorial_5/T5Chat3.png");
	T5Chat3 = Sprite::Create(62, { 0.0f, 360.0f });

	Sprite::LoadTexture(63, L"Resources/Sprite/Tutorial_5/T5Chat4.png");
	T5Chat4 = Sprite::Create(63, { 0.0f, 360.0f });

	Sprite::LoadTexture(64, L"Resources/Sprite/Tutorial_5/T5Chat5.png");
	T5Chat5 = Sprite::Create(64, { 0.0f, 360.0f });

	// GuideState
	Sprite::LoadTexture(72, L"Resources/Sprite/GuideState.png");
	GuideState = Sprite::Create(72, { 0.0f, 0.0f });

	// ポーズ画面背景
	Sprite::LoadTexture(73, L"Resources/Sprite/PauseBG.png");
	PauseBG = Sprite::Create(73, { 0.0f, 0.0f });

	Sprite::LoadTexture(74, L"Resources/Sprite/PauseLog.png");
	PauseLog = Sprite::Create(74, { 0.0f, 0.0f });

#pragma endregion

#pragma region Obj モデル読み込み
	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelPlane = Model::CreateFromOBJ("yuka");
	modelTempWall = Model::CreateFromOBJ("TempWall");
	modelTempWall2 = Model::CreateFromOBJ("TempWall2");
	modelYellowWall = Model::CreateFromOBJ("DoorBase"); //YellowKabe
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

	// 扉
	modelDoorBase = Model::CreateFromOBJ("DoorBase");
	modelDoorRed = Model::CreateFromOBJ("DoorRed");
	modelDoorBlue = Model::CreateFromOBJ("DoorBlue");
	modelDoorGreen = Model::CreateFromOBJ("DoorGreen");
	modelDoorYellow = Model::CreateFromOBJ("DoorYellow");

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
	objButtonBlue2->SetModel(modelButtonBlue);
	objButtonGreen1->SetModel(modelButtonGreen);
	objButtonGreen2->SetModel(modelButtonGreen);
	objButtonYellow->SetModel(modelButtonYellow);
	objButtonYellow2->SetModel(modelButtonYellow);
	objButtonBomb1->SetModel(modelButtonBomb);
	objButtonBomb2->SetModel(modelButtonBomb);
	objButtonFloor->SetModel(modelButtonFloor);

	objMenuSelection->SetModel(modelTempBullet);
	objPlayerMarker->SetModel(modelPlayerMarker);

	objFighter = Player::Create(modelTESTONLY);
	objClone = Enemy::Create(modelTESTONLY);

	// ワープ
	objTeleporterIn1->SetModel(modelTeleporterIn_1);
	objTeleporterIn2->SetModel(modelTeleporterIn_1);
	objTeleporterIn3->SetModel(modelTeleporterIn_1);
	objTeleporterIn4->SetModel(modelTeleporterIn_1);
	objTeleporterIn5->SetModel(modelTeleporterIn_1);
	objTeleporterOut1->SetModel(modelTeleporterOut_1);
	objTeleporterOut2->SetModel(modelTeleporterOut_1);
	objTeleporterOut3->SetModel(modelTeleporterOut_1);
	objTeleporterOut4->SetModel(modelTeleporterOut_1);
	objTeleporterOut5->SetModel(modelTeleporterOut_1);

	// 扉
	objDoorBase->SetModel(modelDoorBase);
	objDoorRed->SetModel(modelDoorRed);
	objDoorBlue->SetModel(modelDoorBlue);
	objDoorGreen->SetModel(modelDoorGreen);
	objDoorYellow->SetModel(modelDoorYellow);

#pragma endregion

#pragma region Fbx モデル読み込み
	// FBXモデルの読み込み Loading FBX model
	modelPlayerRun = FbxLoader::GetInstance()->LoadModelFromFile("PlayerRunning");
	modelPlayerStand = FbxLoader::GetInstance()->LoadModelFromFile("PlayerStanding");
	modelPlayerFight = FbxLoader::GetInstance()->LoadModelFromFile("PlayerFighting");
	modelPlayerWin = FbxLoader::GetInstance()->LoadModelFromFile("PlayerWin");
	modelPlayerLose = FbxLoader::GetInstance()->LoadModelFromFile("PlayerLose");
	modelPlayerFall = FbxLoader::GetInstance()->LoadModelFromFile("PlayerFalling");
	modelPlayerTalk = FbxLoader::GetInstance()->LoadModelFromFile("PlayerTalking");
	modelPlayerThinking = FbxLoader::GetInstance()->LoadModelFromFile("PlayerThinking");

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

	objPlayerTalk = new FbxObject3d;
	objPlayerTalk->Initialize();
	objPlayerTalk->SetModel(modelPlayerTalk);

	objPlayerThinking = new FbxObject3d;
	objPlayerThinking->Initialize();
	objPlayerThinking->SetModel(modelPlayerThinking);

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

	objPlayerTalk->SetPosition({ -11,4,-25 });
	objPlayerTalk->SetRotation({ -30,120,40 });

	objPlayerThinking->SetPosition({ -8,4,-20 });
	objPlayerThinking->SetRotation({ -36,180,0 });

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

	Model* modeltable_y[6] = {
		modelYellowWall,
		modelDoorBlue,
		modelDoorGreen,
		modelDoorRed,
		modelDoorYellow,
		modelPlane,
	};

	Model* modeltable_y2[6] = {
		modelYellowWall,
		modelPlane,
		modelDoorBlue,
		modelDoorGreen,
		modelDoorRed,
		modelDoorYellow,
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
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y1_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y1_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t3_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y1_1->SetRotation({ 0, 0, 0 });
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
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y2_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y2_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t3_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y2_1->SetRotation({ 0, 0, 0 });
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
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y1_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y1_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t3_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y1_2->SetRotation({ 0, 0, 0 });
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
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y2_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y2_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t3_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y2_2->SetRotation({ 0, 0, 0 });
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
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y1_3 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y1_3->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t3_y1_3->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y1_3->SetRotation({ 0, 0, 0 });
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
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t3_y2_3 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t3_y2_3->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t3_y2_3->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_t3_y2_3->SetRotation({ 0, 0, 0 });
			}
			objects_t3_y2_3.push_back(object_t3_y2_3);
		}
	}

#pragma endregion

#pragma region マップチップ生成 チュートリアル4

	//チュートリアル４
	for (int i = 0; i < 12; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 11 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			TouchableObject* object_at4_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_at4_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_at4_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_at4_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 11)
			{
				object_at4_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_at4_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 11 && j == 0 || modelIndex == 1 && i == 11 && j == 6)
			{
				object_at4_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_at4_1->SetRotation({ 180, 90, 0 });
			}
			objects_at4_1.push_back(object_at4_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 12; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 11 || i == 5 && j != 0 && j != 6 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i == 6 && j != 0 && j != 6)
			{
				modelIndex = 2;
			}

			if (i == 10 && j == 5)
			{
				continue;
			}

			TouchableObject* object_at4_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_at4_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_at4_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0 || modelIndex == 2 && i == 6 && j != 0 && j != 6)
			{
				object_at4_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 11 || modelIndex == 1 && i == 5 && j != 0 && j != 6)
			{
				object_at4_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_at4_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 11 && j == 0 || modelIndex == 1 && i == 11 && j == 6)
			{
				object_at4_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_at4_2->SetRotation({ 180, 90, 0 });
			}
			objects_at4_2.push_back(object_at4_2);
		}
	}
#pragma endregion

#pragma region マップチップ生成 チュートリアル5

	//チュートリアル４
	for (int i = 0; i < 12; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 11 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			TouchableObject* object_t5_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t5_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_t5_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t5_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 11)
			{
				object_t5_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t5_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 11 && j == 0 || modelIndex == 1 && i == 11 && j == 6)
			{
				object_t5_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t5_1->SetRotation({ 180, 90, 0 });
			}
			objects_t5_1.push_back(object_t5_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 12; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 11 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			if (i > 3 && i < 7 && j != 0 && j != 6)
			{
				continue;
			}

			TouchableObject* object_t5_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t5_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_t5_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_t5_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 11)
			{
				object_t5_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_t5_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 11 && j == 0 || modelIndex == 1 && i == 11 && j == 6)
			{
				object_t5_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_t5_2->SetRotation({ 180, 90, 0 });
			}
			objects_t5_2.push_back(object_t5_2);
		}
	}

	//スイッチ switch
	for (int i = 0; i < 12; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i > 3 && i <= 6 && j != 0 && j != 6)
			{
				modelIndex = 0;
			}
			else if (i == 7)
			{
				modelIndex = 0;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t5_s2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_t5_s2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_t5_s2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			objects_t5_s2.push_back(object_t5_s2);
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
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s2_y = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s2_y->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s2_y->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 7, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			objects_s2_y.push_back(object_s2_y);
		}
	}

	for (int i = 0; i < 12; i++) { // y coordinate - Bottom to Top
		for (int j = 0; j < 8; j++) { // x coordinate - Left to Right

			int modelIndex;

			if (i == 5 && j == 3)
			{
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s2_y2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s2_y2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
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

#pragma region マップチップ生成 ステージ3

	// ステージ3
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

			if (i == 10 || j == 0 || j == 6 || i < 10 && i > 5 && j < 6 && j > 3 || i == 3 && j == 4)
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
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t4_y = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t4_y->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t4_y->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_t4_y->SetRotation({ 0, 0, 0 });
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
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_t4_y2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_t4_y2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_t4_y2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_t4_y2->SetRotation({ 0, 0, 0 });
			}
			objects_t4_y2.push_back(object_t4_y2);
		}
	}
#pragma endregion

#pragma region マップチップ生成 ステージ4

	// ステージ4
	for (int i = 0; i < 10; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 9 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i == 7 && j > 1 && j < 6)
			{
				modelIndex = 2;
			}

			if (i == 3 && j != 0 && j != 6 || i == 5 && j == 3 || i == 6 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s4_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s4_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s4_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s4_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 9)
			{
				object_s4_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s4_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 9 && j == 0 || modelIndex == 1 && i == 9 && j == 6)
			{
				object_s4_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s4_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 7 && j > 1 && j < 6)
			{
				object_s4_1->SetRotation({ 180, 90, 0 });
			}
			objects_s4_1.push_back(object_s4_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 10; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 9 || j == 0 || j == 6 || i == 7 && j > 1 && j < 6 || i == 5 && j == 2 || i == 5 && j == 4 || i == 3 && j != 0 && j != 6 && j != 3)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			if (i == 8 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s4_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s4_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s4_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s4_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 9)
			{
				object_s4_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s4_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 9 && j == 0 || modelIndex == 1 && i == 9 && j == 6)
			{
				object_s4_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s4_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 7 && j > 1 && j < 6 || i == 5 && j == 2 || i == 5 && j == 4 || i == 3 && j != 0 && j != 6 && j != 3)
			{
				object_s4_2->SetRotation({ 180, 90, 0 });
			}
			objects_s4_2.push_back(object_s4_2);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 10; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 3)
			{
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s4_y = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s4_y->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s4_y->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, -15, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_s4_y->SetRotation({ 0, 0, 0 });
			}
			objects_s4_y.push_back(object_s4_y);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 10; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 1 && i == 7)
			{
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s4_y2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s4_y2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s4_y2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_s4_y2->SetRotation({ 0, 0, 0 });
			}
			objects_s4_y2.push_back(object_s4_y2);
		}
	}

#pragma endregion

#pragma region マップチップ生成 ステージ5

	// ステージ5
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i > 4 && i < 9 && j == 4)
			{
				modelIndex = 1;
			}

			if (i == 0 || i == 8 && j > 0 && j < 3 || i == 2 && j > 0 && j < 3 || i == 4 && j > 0 && j < 3 || i == 4 && j == 4)
			{
				modelIndex = 2;
			}

			if (i == 2 && j < 6 && j > 3 || i == 8 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s5_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s5_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s5_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s5_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s5_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s5_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_s5_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s5_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 8 && j > 0 && j < 3 || i == 2 && j > 0 && j < 3 || i == 4 && j > 0 && j < 3 || i == 4 && j == 4)
			{
				object_s5_1->SetRotation({ 180, 90, 0 });
			}
			else if (i > 4 && i < 9 && j == 4)
			{
				object_s5_1->SetRotation({ 0, 0, 180 });
			}
			objects_s5_1.push_back(object_s5_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i == 2 && j != 0 && j != 1 && j != 6 || i == 8 && j != 0 && j != 3 && j != 6 || i == 6 && j > 3 && j < 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 2 && i < 7 && j == 2)
			{
				modelIndex = 2;
			}

			if (i == 7 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s5_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 2 && i < 7 && j == 2)
			{
				object_s5_2->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_s5_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_s5_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s5_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s5_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s5_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_s5_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s5_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 2 && j != 0 && j != 1 && j != 6 || i == 8 && j != 0 && j != 3 && j != 6 || i == 6 && j > 3 && j < 6)
			{
				object_s5_2->SetRotation({ 180, 90, 0 });
			}
			else if (i > 2 && i < 7 && j == 2)
			{
				object_s5_2->SetRotation({ 0, 180, 90 });
			}
			objects_s5_2.push_back(object_s5_2);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 5 && i == 8)
			{
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s5_y1_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s5_y1_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s5_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_s5_y1_1->SetRotation({ 0, 0, 0 });
			}
			objects_s5_y1_1.push_back(object_s5_y1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 8)
			{
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s5_y2_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s5_y2_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s5_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_s5_y2_1->SetRotation({ 0, 0, 0 });
			}
			objects_s5_y2_1.push_back(object_s5_y2_1);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 8 || j == 3 && i == 4)
			{
				modelIndex = 4;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s5_y1_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s5_y1_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s5_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 4)
			{
				object_s5_y1_2->SetRotation({ 0, 0, 0 });
			}
			objects_s5_y1_2.push_back(object_s5_y1_2);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 3 && i == 6)
			{
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s5_y2_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s5_y2_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s5_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_s5_y2_2->SetRotation({ 0, 0, 0 });
			}
			objects_s5_y2_2.push_back(object_s5_y2_2);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 4 && i == 7)
			{
				modelIndex = 5;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s5_y2_3 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s5_y2_3->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s5_y2_3->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 5)
			{
				object_s5_y2_3->SetRotation({ 0, 90, 0 });
			}
			objects_s5_y2_3.push_back(object_s5_y2_3);
		}
	}

#pragma endregion

#pragma region マップチップ生成 ステージ6

	// ステージ6
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i == 8 && j > 2 && j < 6 || i == 2 && j > 1 && j < 6 || i == 9 && j == 5)
			{
				modelIndex = 2;
			}

			if (i > 2 && i < 8 && j == 5 || i == 8 && j > 0 && j < 3)
			{
				continue;
			}

			TouchableObject* object_as6_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_as6_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_as6_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_as6_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_as6_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_as6_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_as6_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_as6_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 8 && j > 2 && j < 6 || i == 2 && j > 1 && j < 6 || i == 9 && j == 5)
			{
				object_as6_1->SetRotation({ 180, 90, 0 });
			}
			objects_as6_1.push_back(object_as6_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i == 2 && j > 0 && j < 5 || i == 8 && j > 2 && j < 6)
			{
				modelIndex = 1;
			}

			if (i == 0)
			{
				modelIndex = 2;
			}

			TouchableObject* object_as6_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_as6_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_as6_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_as6_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_as6_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_as6_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_as6_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_as6_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 2 && j > 0 && j < 5 || i == 8 && j > 2 && j < 6)
			{
				object_as6_2->SetRotation({ 180, 90, 0 });
			}
			objects_as6_2.push_back(object_as6_2);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 1 && i == 2)
			{
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_as6_y1_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_as6_y1_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_as6_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_as6_y1_1->SetRotation({ 0, 0, 0 });
			}
			objects_as6_y1_1.push_back(object_as6_y1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (j == 5 && i == 2)
			{
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_as6_y2_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_as6_y2_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_as6_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_as6_y2_1->SetRotation({ 0, 0, 0 });
			}
			objects_as6_y2_1.push_back(object_as6_y2_1);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j < 3 && j > 0 && i == 8)
			{
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_as6_y1_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_as6_y1_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_as6_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, -15, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_as6_y1_2->SetRotation({ 0, 0, 0 });
			}
			objects_as6_y1_2.push_back(object_as6_y1_2);
		}
	}

#pragma endregion

#pragma region マップチップ生成 ステージ7

	// ステージ7
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
				modelIndex = 4;
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
			if (modelIndex == 4)
			{
				object_s3_y1_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			}
			else
			{
				object_s3_y1_1->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			}
			object_s3_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 0)
			{
				object_s3_y1_1->SetRotation({ 0, 0, 0 });
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
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y2_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s3_y2_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s3_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_s3_y2_1->SetRotation({ 0, 0, 0 });
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
				modelIndex = 5;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y1_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s3_y1_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s3_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 5)
			{
				object_s3_y1_2->SetRotation({ 0, 0, 0 });
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
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s3_y2_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s3_y2_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s3_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_s3_y2_2->SetRotation({ 0, 180, 0 });
			}
			objects_s3_y2_2.push_back(object_s3_y2_2);
		}
	}
#pragma endregion

#pragma region マップチップ生成 ステージ8

	//ステージ　8
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i == 1 && j == 1 || i == 3 && j == 1 || i == 4 && j == 2 || i > 1 && i < 4 && j == 3 || i > 6 && i < 9 && j == 2 || i == 7 && j == 5)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 0 && i < 5 && j == 4 || i == 6 && j > 1 && j < 6 || i > 6 && i < 9 && j == 3 || i == 9 && j > 2 && j < 5)
			{
				modelIndex = 2;
			}

			if (i == 9 && j == 5)
			{
				continue;
			}

			TouchableObject* object_s6_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s6_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s6_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s6_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s6_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s6_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_s6_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s6_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 1 && i == 1 && j == 1 || i == 3 && j == 1 || i == 4 && j == 2 || i > 1 && i < 4 && j == 3 || i > 0 && i < 5 && j == 4 || i == 6 && j > 1 && j < 6
				|| i > 6 && i < 9 && j == 2 || i > 6 && i < 9 && j == 3 || i == 7 && j == 5 || i == 9 && j > 2 && j < 5)
			{
				object_s6_1->SetRotation({ 180, 90, 0 });
			}
			objects_s6_1.push_back(object_s6_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i > 0 && i < 3 && j == 4 || i == 4 && j > 1 && j < 5 || i > 5 && i < 10 && j == 5 || i == 9 && j > 0 && j < 3 || i == 8 && j == 1)
			{
				modelIndex = 1;
			}

			if (i == 0 || i == 1 && j == 1 || i == 3 && j == 1 || i == 4 && j == 2 || i > 0 && i < 3 && j == 3 || i > 5 && i < 10 && j == 4 ||
				i == 7 && j == 3 || i == 6 && j == 2)
			{
				modelIndex = 2;
			}

			if (i == 9 && j == 3 || i == 6 && j == 1 || i == 5 && j == 2 || i == 3 && j == 4)
			{
				continue;
			}

			TouchableObject* object_s6_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			object_s6_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s6_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s6_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s6_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s6_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_s6_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s6_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 1 && j == 1 || i == 3 && j == 1 || i == 4 && j == 2 || i > 0 && i < 3 && j == 3 || i > 0 && i < 3 && j == 4 || i == 4 && j > 1 && j < 5
				|| i > 5 && i < 10 && j == 5 || i > 5 && i < 10 && j == 4 || i == 7 && j == 3 || i == 6 && j == 2 || i == 9 && j > 0 && j < 3 || i == 8 && j == 1)
			{
				object_s6_2->SetRotation({ 180, 90, 0 });
			}
			objects_s6_2.push_back(object_s6_2);
		}
	}

	//黄色ブロック
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 2 && i == 1)
			{
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y1_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y1_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s6_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_s6_y1_1->SetRotation({ 0, 180, 0 });
			}
			objects_s6_y1_1.push_back(object_s6_y1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 2 && i == 3)
			{
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y2_1 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y2_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s6_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_s6_y2_1->SetRotation({ 0, 180, 0 });
			}
			objects_s6_y2_1.push_back(object_s6_y2_1);
		}
	}

	//黄色ブロック
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 4 && i == 5)
			{
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y1_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y1_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s6_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_s6_y1_2->SetRotation({ 0, -90, 0 });
			}
			objects_s6_y1_2.push_back(object_s6_y1_2);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 4 && i == 5)
			{
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y2_2 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y2_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s6_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_s6_y2_2->SetRotation({ 0, 90, 0 });
			}
			objects_s6_y2_2.push_back(object_s6_y2_2);
		}
	}

	//黄色ブロック
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 1 && i == 6)
			{
				modelIndex = 1;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y1_3 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y1_3->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s6_y1_3->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 1)
			{
				object_s6_y1_3->SetRotation({ 0, 0, 0 });
			}
			objects_s6_y1_3.push_back(object_s6_y1_3);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 2 && i == 5)
			{
				modelIndex = 5;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y2_3 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y2_3->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s6_y2_3->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 5)
			{
				object_s6_y2_3->SetRotation({ 0, 0, 0 });
			}
			objects_s6_y2_3.push_back(object_s6_y2_3);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (j == 1 && i == 6)
			{
				modelIndex = 5;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s6_y2_4 = TouchableObject::Create(modeltable_y[modelIndex]);
			object_s6_y2_4->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
			object_s6_y2_4->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, -15.0f, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 5)
			{
				object_s6_y2_4->SetRotation({ 0, 0, 0 });
			}
			objects_s6_y2_4.push_back(object_s6_y2_4);
		}
	}

#pragma endregion

#pragma region マップチップ生成 ステージ9

	// ステージ9
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6)
			{
				modelIndex = 1;
			}

			if (i == 0 || i == 4 && j == 2 || i == 4 && j == 4 || i == 7 && j == 2 || i == 7 && j == 4 || i == 4 && j == 3 || i > 4 && i < 10 && j == 3)
			{
				modelIndex = 2;
			}

			if (i == 9 && j == 2)
			{
				continue;
			}

			TouchableObject* object_s9_1 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 4 && i < 10 && j == 3)
			{
				object_s9_1->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });

			}
			else
			{
				object_s9_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			//object_s9_1->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			object_s9_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s9_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s9_1->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s9_1->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_s9_1->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s9_1->SetRotation({ 180, 90, 0 });
			}
			else if (i == 4 && j == 2 || i == 4 && j == 4 || i == 7 && j == 2 || i == 7 && j == 4 || i == 4 && j == 3)
			{
				object_s9_1->SetRotation({ 180, 90, 0 });
			}
			else if (i > 4 && i < 10 && j == 3)
			{
				object_s9_1->SetRotation({ 0, 180, 90 });
			}
			objects_s9_1.push_back(object_s9_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 10 || j == 0 || j == 6 || i == 4 && j == 4 || i == 7 && j == 2 || i == 7 && j == 4 || i == 4 && j == 3)
			{
				modelIndex = 1;
			}

			if (i == 0 || i > 4 && i < 10 && j == 3 || i == 4 && j == 2)
			{
				modelIndex = 2;
			}

			TouchableObject* object_s9_2 = TouchableObject::Create(modeltable_s2[modelIndex]);
			if (i > 4 && i < 10 && j == 3)
			{
				object_s9_2->SetScale({ LAND_SCALE * 1.5f, LAND_SCALE, LAND_SCALE });
			}
			else
			{
				object_s9_2->SetScale({ LAND_SCALE, LAND_SCALE * 1.5f, LAND_SCALE });
			}
			object_s9_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2 && i == 0)
			{
				object_s9_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && i == 10)
			{
				object_s9_2->SetRotation({ 180, 0, 0 });
			}
			else if (modelIndex == 1 && j == 0 || modelIndex == 1 && j == 6)
			{
				object_s9_2->SetRotation({ 180, 180, 0 });
			}
			if (modelIndex == 1 && i == 10 && j == 0 || modelIndex == 1 && i == 10 && j == 6)
			{
				object_s9_2->SetRotation({ 180, 90, 0 });
			}
			else if (modelIndex == 2 && i == 0 && j == 0 || modelIndex == 2 && i == 0 && j == 6)
			{
				object_s9_2->SetRotation({ 180, 90, 0 });
			}
			else if (i == 4 && j == 4 || i == 7 && j == 2 || i == 7 && j == 4 || i == 4 && j == 2 || i == 4 && j == 3)
			{
				object_s9_2->SetRotation({ 180, 90, 0 });
			}
			else if (i > 3 && i < 10 && j == 3)
			{
				object_s9_2->SetRotation({ 0, 180, 90 });
			}
			objects_s9_2.push_back(object_s9_2);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 4 && j == 1 || i == 4 && j == 5)
			{
				modelIndex = 3;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s9_y1_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s9_y1_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s9_y1_1->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 3)
			{
				object_s9_y1_1->SetRotation({ 0, 0, 0 });
			}
			objects_s9_y1_1.push_back(object_s9_y1_1);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (i == 4 && j == 1 || i == 4 && j == 5)
			{
				modelIndex = 2;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s9_y2_1 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s9_y2_1->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s9_y2_1->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 2)
			{
				object_s9_y2_1->SetRotation({ 0, 0, 0 });
			}
			objects_s9_y2_1.push_back(object_s9_y2_1);
		}
	}

	//黄色ブロックとあな
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 7; j++) // X
		{
			int modelIndex = 0;

			if (i == 7 && j == 1 || i == 7 && j == 5)
			{
				modelIndex = 5;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s9_y1_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s9_y1_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s9_y1_2->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1 - 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 5)
			{
				object_s9_y1_2->SetRotation({ 0, 0, 0 });
			}
			objects_s9_y1_2.push_back(object_s9_y1_2);
		}
	}

	//敵 Enemy
	for (int i = 0; i < 11; i++) // Y
	{
		for (int j = 0; j < 8; j++) // X
		{
			int modelIndex = 0;

			if (i == 7 && j == 1 || i == 7 && j == 5)
			{
				modelIndex = 4;
			}
			else
			{
				continue;
			}

			TouchableObject* object_s9_y2_2 = TouchableObject::Create(modeltable_y2[modelIndex]);
			object_s9_y2_2->SetScale({ LAND_SCALE * 0.5f, LAND_SCALE * 0.66f, LAND_SCALE * 0.5f });
			object_s9_y2_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE + 5, 0, (i - DIV_NUM / 2) * LAND_SCALE });
			if (modelIndex == 4)
			{
				object_s9_y2_2->SetRotation({ 0, 0, 0 });
			}
			objects_s9_y2_2.push_back(object_s9_y2_2);
		}
	}

#pragma endregion

#pragma region PositionRotationScale

	//objFighter->SetPosition({ -10, 10, 0 });
	objFighter->SetScale({ 1.0f,1.0f,1.0f }); //3.0
	//objFighter->SetCollider(new SphereCollider);
	objClone->SetScale({ 1.0f,1.0f,1.0f });
	//objClone->SetCollider(new SphereCollider);

	objFighter->SetPosition({ -20, 30, -9 }); // -20, 12
	objClone->SetPosition({ 20, 30, -9 }); // 20, 12

	objTempBullet->SetPosition({ -6.0f, 1.0f, 0 });
	objTempBulletE->SetPosition({ 6.0f, 1.0f, 0 });
	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	objMenuSelection->SetScale({ 0.25f, 0.25f, 0.25f });

	objPlayerMarker->SetScale({ 0.5f, 0.5f, 0.5f });

	objTeleporterIn1->SetPosition({ -8.0f, 0.0f, -12.0f }); // 8, -3
	objTeleporterIn1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn2->SetPosition({ 11.0f, 0.0f, 3.0f }); // 5, -12
	objTeleporterIn2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn2->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn3->SetPosition({ 17.0f, 0.0f, 3.0f }); // -11, -9
	objTeleporterIn3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn4->SetPosition({ -17.0f, 0.0f, 0.0f }); // 14, 12
	objTeleporterIn4->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn4->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn5->SetPosition({ -11.0f, 0.0f, 0.0f }); // 8, -3
	objTeleporterIn5->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn5->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut1->SetPosition({ 20.0f, 0.0f, -12.0f }); // 20, 6
	objTeleporterOut1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut2->SetPosition({ -20.0f, 0.0f, 3.0f }); // -17, 6
	objTeleporterOut2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut2->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut3->SetPosition({ -8.0f, 0.0f, 3.0f }); // 11, 0
	objTeleporterOut3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut4->SetPosition({ 11.0f, 0.0f, 12.0f }); // 8, -12
	objTeleporterOut4->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut4->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut5->SetPosition({ 17.0f, 0.0f, 12.0f }); // 20, 6
	objTeleporterOut5->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut5->SetRotation({ 0.0f, 0.0f, 0.0f });

	objButtonRed1->SetPosition({ -17, 0, 3 });
	objButtonRed2->SetPosition({ -11, 0, 3 });
	objButtonGreen1->SetPosition({ 17, 0, 0 });
	objButtonGreen2->SetPosition({ 11, 0, 0 });
	objButtonBlue->SetPosition({ 20, 0, -9 });
	objButtonBlue2->SetPosition({ 8, 0, -9 });
	objButtonYellow->SetPosition({ 8, 0, 12 });
	objButtonYellow2->SetPosition({ 20, 0, 12 });
	objButtonFloor->SetPosition({ 20, 0, -12 });
	objButtonBomb1->SetPosition({ -11, 0, 12 });

#pragma endregion

	camera->SetEye({ 0, 20, -30 });

	camera->SetTarget({ 0, 1, 0 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f }); //debug

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

#pragma region ロケーションアイテム Location Items
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
	BlueButton2 = objButtonBlue2->GetPosition();
	YellowButton = objButtonYellow->GetPosition();
	YellowButton2 = objButtonYellow2->GetPosition();
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
#pragma endregion

	if (beginStage && !falling && !pause && !Tutorial)
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
			if (TutorialNo == 0 && sceneNo != 0 && sceneNo != 1 && sceneNo != 2 && sceneNo != 3 && sceneNo != 4 && sceneNo != 8 && sceneNo != 9 && sceneNo != 10 && sceneNo != 13 && sceneNo != 14 && sceneNo != 15 && sceneNo != 16 && sceneNo != 17)
			{
				TutorialNo = 1;
				Tutorial = true;
			}
		}
	}

	if (!Tutorial)
	{
		playerMarkerPosition.x = playerPosition.x;
		playerMarkerPosition.y = playerPosition.y + 8.0f;
		playerMarkerPosition.z = playerPosition.z;
	}

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

#pragma region 歩く音 walking sound
	if (beginStage && !Tutorial && sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && !falling && !pause)
	{
		if (test2 >= 20 || playFlag == 0)
		{
			if (test3)
			{
				audio->StopWave("Walk.wav");
			}
			test2 = 0;
			test1 = false;
		}

		if (playFlag == 1)
		{
			if (test1 == false)
			{
				audio->PlayWave("Walk.wav", Volume * 50.0f, false);
				test3 = true;
				test1 = true;
			}

			test2++;
		}
	}

	if (sceneNo == 2 && test3 == true || sceneNo == 3 && test3 == true || pause && test3 == true)
	{
		audio->StopWave("Walk.wav");
	}
#pragma endregion

#pragma region ポーズ画面
	if (input->TriggerKey(DIK_ESCAPE) && sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && beginStage && !falling && !Tutorial ||
		IsButtonPush(ButtonKind::Button_RightMenu) && sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && beginStage && !falling && !Tutorial)
	{
		if (pause == false)
		{
			audio->PlayWave("Esc.wav", Volume, false);
		}

		if (!pause)
		{
			pauseMenuSelection = 0;
			pause = true;
			pauseCameraMove = cameraMove;
			camera->SetEye({ 0.0f, 25.0f, -30.0f });
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
			audio->PlayWave("MoveCursor.wav", Volume, false);
		}
		else if (input->TriggerKey(DIK_W) && pauseMenuSelection > 0 || IsButtonPush(ButtonKind::UpButton) && pauseMenuSelection > 0)
		{
			pauseMenuSelection--;
			audio->PlayWave("MoveCursor.wav", Volume, false);
		}

		switch (pauseMenuSelection)
		{
		case 0:
			pause4->SetPosition({ 670, 200 });
			break;
		case 1:
			pause4->SetPosition({ 670, 300 });
			break;
		case 2:
			pause4->SetPosition({ 670, 400 });
			break;
		}

		if (input->TriggerKey(DIK_SPACE) || IsButtonPush(ButtonKind::Button_A))
		{
			audio->PlayWave("Decision.wav", Volume, false);
			if (pauseMenuSelection == 0)
			{
				pause = false;
				pausePosition = false;
				cameraMove = pauseCameraMove;
				if (cameraMove == 1)
				{
					camera->SetEye({ 0.0f, 25.0f, -30.0f });
				}
				else if (cameraMove == 2)
				{
					camera->SetEye({ -40.0f, 25.0f, 0.0f });
				}
				else if (cameraMove == 3)
				{
					camera->SetEye({ 0.0f, 25.0f, 30.0f });
				}
				else if (cameraMove == 4)
				{
					camera->SetEye({ 40.0f, 25.0f, 0.0f });
				}
			}
			else if (pauseMenuSelection == 1)
			{
				switch (sceneNo)
				{
				case 0:
					break;
				case 1:
					Stage3Move();
					Stage3Reset();
					sceneNo = 1;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					Stage1Move();
					Stage1Reset();
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
					Stage2Move();
					Stage2Reset();
					sceneNo = 9;
					break;
				case 10:
					Stage7Move();
					Stage7Reset();
					sceneNo = 10;
					break;
				case 11:
					Tutorial4Move();
					Tutorial4Reset();
					sceneNo = 11;
					break;
				case 12:
					Tutorial5Move();
					Tutorial5Reset();
					sceneNo = 12;
					break;
				case 13:
					Stage8Move();
					Stage8Reset();
					sceneNo = 13;
					break;
				case 14:
					Stage4Move();
					Stage4Reset();
					sceneNo = 14;
					break;
				case 15:
					Stage5Move();
					Stage5Reset();
					sceneNo = 15;
					break;
				case 16:
					Stage6Move();
					Stage6Reset();
					sceneNo = 16;
					break;
				case 17:
					Stage9Move();
					Stage9Reset();
					sceneNo = 17;
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
					Stage3Move();
					sceneNo = 8;
					stageSelect = 7;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0   });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s3FirstPlayFlag = false;
					delay = true;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					Stage1Move();
					sceneNo = 8;
					stageSelect = 5;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s1FirstPlayFlag = false;
					delay = true;
					break;
				case 5:
					Tutorial1Move();
					sceneNo = 8;
					stageSelect = 0;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					t1FirstPlayFlag = false;
					delay = true;
					break;
				case 6:
					Tutorial2Move();
					sceneNo = 8;
					stageSelect = 1;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					t2FirstPlayFlag = false;
					delay = true;
					break;
				case 7:
					Tutorial3Move();
					sceneNo = 8;
					stageSelect = 2;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					t3FirstPlayFlag = false;
					delay = true;
					break;
				case 8:
					break;
				case 9:
					Stage2Move();
					sceneNo = 8;
					stageSelect = 6;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s2FirstPlayFlag = false;
					delay = true;
					break;
				case 10:
					Stage7Move();
					sceneNo = 8;
					stageSelect = 11;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s7FirstPlayFlag = false;
					delay = true;
					break;
				case 11:
					Tutorial4Move();
					sceneNo = 8;
					stageSelect = 3;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					t4FirstPlayFlag = false;
					delay = true;
					break;
				case 12:
					Tutorial5Move();
					sceneNo = 8;
					stageSelect = 4;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					t5FirstPlayFlag = false;
					delay = true;
					break;
				case 13:
					Stage8Move();
					sceneNo = 8;
					stageSelect = 12;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s8FirstPlayFlag = false;
					delay = true;
					break;
				case 14:
					Stage4Move();
					sceneNo = 8;
					stageSelect = 8;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s4FirstPlayFlag = false;
					delay = true;
					break;
				case 15:
					Stage5Move();
					sceneNo = 8;
					stageSelect = 9;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s5FirstPlayFlag = false;
					delay = true;
					break;
				case 16:
					Stage6Move();
					sceneNo = 8;
					stageSelect = 10;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s6FirstPlayFlag = false;
					delay = true;
					break;
				case 17:
					Stage9Move();
					sceneNo = 8;
					stageSelect = 13;
					audio->StopWave("Stage.wav");
					audio->PlayWave("Title.wav", Volume, true);
					camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
					camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
					menuBallRotation = { 0.0f, 0.0f, 0.0f };
					menuSelection = 0;
					s9FirstPlayFlag = false;
					delay = true;
					break;
				}

				t1BackgroundPosition = { SpriteStageSizeX + (1280 * (0 - stageSelect)), SpriteStageSizeY };
				t2BackgroundPosition = { SpriteStageSizeX + (1280 * (1 - stageSelect)), SpriteStageSizeY };
				t3BackgroundPosition = { SpriteStageSizeX + (1280 * (2 - stageSelect)), SpriteStageSizeY };
				t4BackgroundPosition = { SpriteStageSizeX + (1280 * (3 - stageSelect)), SpriteStageSizeY };
				t5BackgroundPosition = { SpriteStageSizeX + (1280 * (4 - stageSelect)), SpriteStageSizeY };
				s1BackgroundPosition = { SpriteStageSizeX + (1280 * (5 - stageSelect)), SpriteStageSizeY };
				s2BackgroundPosition = { SpriteStageSizeX + (1280 * (6 - stageSelect)), SpriteStageSizeY };
				s3BackgroundPosition = { SpriteStageSizeX + (1280 * (7 - stageSelect)), SpriteStageSizeY };
				s4BackgroundPosition = { SpriteStageSizeX + (1280 * (8 - stageSelect)), SpriteStageSizeY };
				s5BackgroundPosition = { SpriteStageSizeX + (1280 * (9 - stageSelect)), SpriteStageSizeY };
				s6BackgroundPosition = { SpriteStageSizeX + (1280 * (10 - stageSelect)), SpriteStageSizeY };
				s7BackgroundPosition = { SpriteStageSizeX + (1280 * (11 - stageSelect)), SpriteStageSizeY };
				s8BackgroundPosition = { SpriteStageSizeX + (1280 * (12 - stageSelect)), SpriteStageSizeY };
				s9BackgroundPosition = { SpriteStageSizeX + (1280 * (13 - stageSelect)), SpriteStageSizeY };

				t1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s6StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s7StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s8StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s9StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 20, (SpriteStageSizeY - 11) };

				t1NewLogPosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t2NewLogPosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t3NewLogPosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t4NewLogPosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t5NewLogPosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s1NewLogPosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s2NewLogPosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s3NewLogPosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s4NewLogPosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s5NewLogPosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s6NewLogPosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s7NewLogPosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s8NewLogPosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s9NewLogPosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 80, (SpriteStageSizeY + 60) };

				t1ClearLogPosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t2ClearLogPosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t3ClearLogPosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t4ClearLogPosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t5ClearLogPosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s1ClearLogPosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s2ClearLogPosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s3ClearLogPosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s4ClearLogPosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s5ClearLogPosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s6ClearLogPosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s7ClearLogPosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s8ClearLogPosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s9ClearLogPosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 150, (SpriteStageSizeY + 100) };

				t1Background->SetPosition(t1BackgroundPosition);
				t2Background->SetPosition(t2BackgroundPosition);
				t3Background->SetPosition(t3BackgroundPosition);
				t4Background->SetPosition(t4BackgroundPosition);
				t5Background->SetPosition(t5BackgroundPosition);
				s1Background->SetPosition(s1BackgroundPosition);
				s2Background->SetPosition(s2BackgroundPosition);
				s3Background->SetPosition(s3BackgroundPosition);
				s4Background->SetPosition(s4BackgroundPosition);
				s5Background->SetPosition(s5BackgroundPosition);
				s6Background->SetPosition(s6BackgroundPosition);
				s7Background->SetPosition(s7BackgroundPosition);
				s8Background->SetPosition(s8BackgroundPosition);
				s9Background->SetPosition(s9BackgroundPosition);

				t1StageSelectFrame->SetPosition(t1StageSelectFramePosition);
				t2StageSelectFrame->SetPosition(t2StageSelectFramePosition);
				t3StageSelectFrame->SetPosition(t3StageSelectFramePosition);
				t4StageSelectFrame->SetPosition(t4StageSelectFramePosition);
				t5StageSelectFrame->SetPosition(t5StageSelectFramePosition);
				s1StageSelectFrame->SetPosition(s1StageSelectFramePosition);
				s2StageSelectFrame->SetPosition(s2StageSelectFramePosition);
				s3StageSelectFrame->SetPosition(s3StageSelectFramePosition);
				s4StageSelectFrame->SetPosition(s4StageSelectFramePosition);
				s5StageSelectFrame->SetPosition(s5StageSelectFramePosition);
				s6StageSelectFrame->SetPosition(s6StageSelectFramePosition);
				s7StageSelectFrame->SetPosition(s7StageSelectFramePosition);
				s8StageSelectFrame->SetPosition(s8StageSelectFramePosition);
				s9StageSelectFrame->SetPosition(s9StageSelectFramePosition);

				t1NewLog->SetPosition(t1NewLogPosition);
				t2NewLog->SetPosition(t2NewLogPosition);
				t3NewLog->SetPosition(t3NewLogPosition);
				t4NewLog->SetPosition(t4NewLogPosition);
				t5NewLog->SetPosition(t4NewLogPosition);
				s1NewLog->SetPosition(s1NewLogPosition);
				s2NewLog->SetPosition(s2NewLogPosition);
				s3NewLog->SetPosition(s3NewLogPosition);
				s4NewLog->SetPosition(s4NewLogPosition);
				s5NewLog->SetPosition(s5NewLogPosition);
				s6NewLog->SetPosition(s6NewLogPosition);
				s7NewLog->SetPosition(s7NewLogPosition);
				s8NewLog->SetPosition(s8NewLogPosition);
				s9NewLog->SetPosition(s9NewLogPosition);

				t1ClearLog->SetPosition(t1ClearLogPosition);
				t2ClearLog->SetPosition(t2ClearLogPosition);
				t3ClearLog->SetPosition(t3ClearLogPosition);
				t4ClearLog->SetPosition(t4ClearLogPosition);
				t5ClearLog->SetPosition(t5ClearLogPosition);
				s1ClearLog->SetPosition(s1ClearLogPosition);
				s2ClearLog->SetPosition(s2ClearLogPosition);
				s3ClearLog->SetPosition(s3ClearLogPosition);
				s4ClearLog->SetPosition(s4ClearLogPosition);
				s5ClearLog->SetPosition(s5ClearLogPosition);
				s6ClearLog->SetPosition(s6ClearLogPosition);
				s7ClearLog->SetPosition(s7ClearLogPosition);
				s8ClearLog->SetPosition(s8ClearLogPosition);
				s9ClearLog->SetPosition(s9ClearLogPosition);

				pause = false;
				pausePosition = false;
			}
		}
		objPlayerThinking->Update();
	}
#pragma endregion

#pragma region カメラ回転 Camera Rotation
	// Camera Movement カメラ動く
	if (beginStage && sceneNo != 8 && sceneNo != 2 && sceneNo != 0 && sceneNo != 3 && !pause)
	{
		if (input->TriggerKey(DIK_Q) && cameraChange == false || input->TriggerKey(DIK_E) && cameraChange == false)
		{
			audio->PlayWave("StageRotation.wav", Volume, false);
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
		if (PlayFlag == false)
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

			audio->PlayWave("Decision.wav", Volume, false);
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

			audio->PlayWave("Decision.wav", Volume, false);
			SceneSelectionReset();
			sceneNo = 8;
			titleScene->Finalize();
			break;
		}
#pragma endregion
		break;

	case 1: // ステージ3
#pragma region case1 ステージ3
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
				SwitchFlag1 = true;
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
				Stage3Move();
				GameOverReset();
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				sceneChange = 0;

				s3FirstPlayFlag = false;
			}
			else if (intersect(playerBullet, enemyPosition, 1.0f, 1.0f, 1.0f) && playerBulletF == true)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage3Move();
				GameClearReset();
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				sceneChange = 0;

				s3FirstPlayFlag = false;
				s3ClearFlag = true;
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

			lastIntersect = intersect(playerPosition, BombButton1, 1.0f, 1.0f, 1.0f);
			lastIntersectE = intersect(enemyPosition, BombButton2, 1.0f, 1.0f, 1.0f);
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
			audio->PlayWave("Decision.wav", Volume, false);
			audio->StopWave("GameClear.wav");
			if (menuSelection == 0)
			{
				switch (lastScene)
				{
				case 0:
					break;
				case 1:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage4Reset();
					sceneNo = 14;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage2Reset();
					sceneNo = 9;
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
					sceneNo = 11;
					break;
				case 8:
					break;
				case 9:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage3Reset();
					sceneNo = 1;
					break;
				case 10:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage8Reset();
					sceneNo = 13;
					break;
				case 11:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial5Reset();
					sceneNo = 12;
					break;
				case 12:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage1Reset();
					sceneNo = 4;
					break;
				case 13:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage9Reset();
					sceneNo = 17;
					break;
				case 14:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage5Reset();
					sceneNo = 15;
					break;
				case 15:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage6Reset();
					sceneNo = 16;
					break;
				case 16:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage7Reset();
					sceneNo = 10;
					break;
				case 17:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage9Reset();
					sceneNo = 17;
					break;
				}

				break;
			}
			else if (menuSelection == 1)
			{
				audio->PlayWave("Title.wav", Volume, true);
				sceneNo = 8;
				switch (lastScene)
				{
				case 0:
					break;
				case 1:
					stageSelect = 7;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					stageSelect = 5;
					break;
				case 5:
					stageSelect = 0;
					break;
				case 6:
					stageSelect = 1;
					break;
				case 7:
					stageSelect = 2;
					break;
				case 8:
					break;
				case 9:
					stageSelect = 6;
					break;
				case 10:
					stageSelect = 11;
					break;
				case 11:
					stageSelect = 3;
					break;
				case 12:
					stageSelect = 4;
					break;
				case 13:
					stageSelect = 12;
					break;
				case 14:
					stageSelect = 8;
					break;
				case 15:
					stageSelect = 9;
					break;
				case 16:
					stageSelect = 10;
					break;
				case 17:
					stageSelect = 13;
					break;
				}

				t1BackgroundPosition = { SpriteStageSizeX + (1280 * (0 - stageSelect)), SpriteStageSizeY };
				t2BackgroundPosition = { SpriteStageSizeX + (1280 * (1 - stageSelect)), SpriteStageSizeY };
				t3BackgroundPosition = { SpriteStageSizeX + (1280 * (2 - stageSelect)), SpriteStageSizeY };
				t4BackgroundPosition = { SpriteStageSizeX + (1280 * (3 - stageSelect)), SpriteStageSizeY };
				t5BackgroundPosition = { SpriteStageSizeX + (1280 * (4 - stageSelect)), SpriteStageSizeY };
				s1BackgroundPosition = { SpriteStageSizeX + (1280 * (5 - stageSelect)), SpriteStageSizeY };
				s2BackgroundPosition = { SpriteStageSizeX + (1280 * (6 - stageSelect)), SpriteStageSizeY };
				s3BackgroundPosition = { SpriteStageSizeX + (1280 * (7 - stageSelect)), SpriteStageSizeY };
				s4BackgroundPosition = { SpriteStageSizeX + (1280 * (8 - stageSelect)), SpriteStageSizeY };
				s5BackgroundPosition = { SpriteStageSizeX + (1280 * (9 - stageSelect)), SpriteStageSizeY };
				s6BackgroundPosition = { SpriteStageSizeX + (1280 * (10 - stageSelect)), SpriteStageSizeY };
				s7BackgroundPosition = { SpriteStageSizeX + (1280 * (11 - stageSelect)), SpriteStageSizeY };
				s8BackgroundPosition = { SpriteStageSizeX + (1280 * (12 - stageSelect)), SpriteStageSizeY };
				s9BackgroundPosition = { SpriteStageSizeX + (1280 * (13 - stageSelect)), SpriteStageSizeY };

				t1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s6StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s7StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s8StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s9StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 20, (SpriteStageSizeY - 11) };

				t1NewLogPosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t2NewLogPosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t3NewLogPosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t4NewLogPosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t5NewLogPosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s1NewLogPosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s2NewLogPosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s3NewLogPosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s4NewLogPosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s5NewLogPosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s6NewLogPosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s7NewLogPosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 80, (SpriteStageSizeY + 60)};
				s8NewLogPosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s9NewLogPosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 80, (SpriteStageSizeY + 60) };

				t1ClearLogPosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t2ClearLogPosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t3ClearLogPosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t4ClearLogPosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t5ClearLogPosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s1ClearLogPosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s2ClearLogPosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s3ClearLogPosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s4ClearLogPosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s5ClearLogPosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s6ClearLogPosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s7ClearLogPosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s8ClearLogPosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s9ClearLogPosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 150, (SpriteStageSizeY + 100) };

				t1Background->SetPosition(t1BackgroundPosition);
				t2Background->SetPosition(t2BackgroundPosition);
				t3Background->SetPosition(t3BackgroundPosition);
				t4Background->SetPosition(t4BackgroundPosition);
				t5Background->SetPosition(t5BackgroundPosition);
				s1Background->SetPosition(s1BackgroundPosition);
				s2Background->SetPosition(s2BackgroundPosition);
				s3Background->SetPosition(s3BackgroundPosition);
				s4Background->SetPosition(s4BackgroundPosition);
				s5Background->SetPosition(s5BackgroundPosition);
				s6Background->SetPosition(s6BackgroundPosition);
				s7Background->SetPosition(s7BackgroundPosition);
				s8Background->SetPosition(s8BackgroundPosition);
				s9Background->SetPosition(s9BackgroundPosition);

				t1StageSelectFrame->SetPosition(t1StageSelectFramePosition);
				t2StageSelectFrame->SetPosition(t2StageSelectFramePosition);
				t3StageSelectFrame->SetPosition(t3StageSelectFramePosition);
				t4StageSelectFrame->SetPosition(t4StageSelectFramePosition);
				t5StageSelectFrame->SetPosition(t5StageSelectFramePosition);
				s1StageSelectFrame->SetPosition(s1StageSelectFramePosition);
				s2StageSelectFrame->SetPosition(s2StageSelectFramePosition);
				s3StageSelectFrame->SetPosition(s3StageSelectFramePosition);
				s4StageSelectFrame->SetPosition(s4StageSelectFramePosition);
				s5StageSelectFrame->SetPosition(s5StageSelectFramePosition);
				s6StageSelectFrame->SetPosition(s6StageSelectFramePosition);
				s7StageSelectFrame->SetPosition(s7StageSelectFramePosition);
				s8StageSelectFrame->SetPosition(s8StageSelectFramePosition);
				s9StageSelectFrame->SetPosition(s9StageSelectFramePosition);

				t1NewLog->SetPosition(t1NewLogPosition);
				t2NewLog->SetPosition(t2NewLogPosition);
				t3NewLog->SetPosition(t3NewLogPosition);
				t4NewLog->SetPosition(t4NewLogPosition);
				t5NewLog->SetPosition(t4NewLogPosition);
				s1NewLog->SetPosition(s1NewLogPosition);
				s2NewLog->SetPosition(s2NewLogPosition);
				s3NewLog->SetPosition(s3NewLogPosition);
				s4NewLog->SetPosition(s4NewLogPosition);
				s5NewLog->SetPosition(s5NewLogPosition);
				s6NewLog->SetPosition(s6NewLogPosition);
				s7NewLog->SetPosition(s7NewLogPosition);
				s8NewLog->SetPosition(s8NewLogPosition);
				s9NewLog->SetPosition(s9NewLogPosition);

				t1ClearLog->SetPosition(t1ClearLogPosition);
				t2ClearLog->SetPosition(t2ClearLogPosition);
				t3ClearLog->SetPosition(t3ClearLogPosition);
				t4ClearLog->SetPosition(t4ClearLogPosition);
				t5ClearLog->SetPosition(t5ClearLogPosition);
				s1ClearLog->SetPosition(s1ClearLogPosition);
				s2ClearLog->SetPosition(s2ClearLogPosition);
				s3ClearLog->SetPosition(s3ClearLogPosition);
				s4ClearLog->SetPosition(s4ClearLogPosition);
				s5ClearLog->SetPosition(s5ClearLogPosition);
				s6ClearLog->SetPosition(s6ClearLogPosition);
				s7ClearLog->SetPosition(s7ClearLogPosition);
				s8ClearLog->SetPosition(s8ClearLogPosition);
				s9ClearLog->SetPosition(s9ClearLogPosition);

				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
				break;
			}
		}

		if (input->PushKey(DIK_S) && menuSelection == 0 && !menuMoving || IsButtonDown(ButtonKind::DownButton) && menuSelection == 0 && !menuMoving)
		{
			audio->PlayWave("MoveCursor.wav", Volume, false);
			menuMoving = true;
		}
		else if (input->PushKey(DIK_W) && menuSelection == 1 && !menuMoving && lastScene != 17 || IsButtonDown(ButtonKind::UpButton) && menuSelection == 1 && !menuMoving && lastScene != 17)
		{
			audio->PlayWave("MoveCursor.wav", Volume, false);
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
			audio->PlayWave("Decision.wav", Volume, false);
			audio->StopWave("GameOver.wav");
			if (menuSelection == 0)
			{
				switch (lastScene)
				{
				case 0:
					break;
				case 1:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage3Reset();
					sceneNo = 1;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage1Reset();
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
					Stage2Reset();
					sceneNo = 9;
					break;
				case 10:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage7Reset();
					sceneNo = 10;
					break;
				case 11:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial4Reset();
					sceneNo = 11;
					break;
				case 12:
					audio->PlayWave("Stage.wav", Volume, true);
					Tutorial5Reset();
					sceneNo = 12;
					break;
				case 13:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage8Reset();
					sceneNo = 13;
					break;
				case 14:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage4Reset();
					sceneNo = 14;
					break;
				case 15:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage5Reset();
					sceneNo = 15;
					break;
				case 16:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage6Reset();
					sceneNo = 16;
					break;
				case 17:
					audio->PlayWave("Stage.wav", Volume, true);
					Stage9Reset();
					sceneNo = 17;
					break;
				}

				break;
			}
			else if (menuSelection == 1)
			{
				audio->PlayWave("Title.wav", Volume, true);
				sceneNo = 8;
				switch (lastScene)
				{
				case 0:
					break;
				case 1:
					stageSelect = 7;
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					stageSelect = 5;
					break;
				case 5:
					stageSelect = 0;
					break;
				case 6:
					stageSelect = 1;
					break;
				case 7:
					stageSelect = 2;
					break;
				case 8:
					break;
				case 9:
					stageSelect = 6;
					break;
				case 10:
					stageSelect = 11;
					break;
				case 11:
					stageSelect = 3;
					break;
				case 12:
					stageSelect = 4;
					break;
				case 13:
					stageSelect = 12;
					break;
				case 14:
					stageSelect = 8;
					break;
				case 15:
					stageSelect = 9;
					break;
				case 16:
					stageSelect = 10;
					break;
				case 17:
					stageSelect = 13;
					break;
				}

				t1BackgroundPosition = { SpriteStageSizeX + (1280 * (0 - stageSelect)), SpriteStageSizeY };
				t2BackgroundPosition = { SpriteStageSizeX + (1280 * (1 - stageSelect)), SpriteStageSizeY };
				t3BackgroundPosition = { SpriteStageSizeX + (1280 * (2 - stageSelect)), SpriteStageSizeY };
				t4BackgroundPosition = { SpriteStageSizeX + (1280 * (3 - stageSelect)), SpriteStageSizeY };
				t5BackgroundPosition = { SpriteStageSizeX + (1280 * (4 - stageSelect)), SpriteStageSizeY };
				s1BackgroundPosition = { SpriteStageSizeX + (1280 * (5 - stageSelect)), SpriteStageSizeY };
				s2BackgroundPosition = { SpriteStageSizeX + (1280 * (6 - stageSelect)), SpriteStageSizeY };
				s3BackgroundPosition = { SpriteStageSizeX + (1280 * (7 - stageSelect)), SpriteStageSizeY };
				s4BackgroundPosition = { SpriteStageSizeX + (1280 * (8 - stageSelect)), SpriteStageSizeY };
				s5BackgroundPosition = { SpriteStageSizeX + (1280 * (9 - stageSelect)), SpriteStageSizeY };
				s6BackgroundPosition = { SpriteStageSizeX + (1280 * (10 - stageSelect)), SpriteStageSizeY };
				s7BackgroundPosition = { SpriteStageSizeX + (1280 * (11 - stageSelect)), SpriteStageSizeY };
				s8BackgroundPosition = { SpriteStageSizeX + (1280 * (12 - stageSelect)), SpriteStageSizeY };
				s9BackgroundPosition = { SpriteStageSizeX + (1280 * (13 - stageSelect)), SpriteStageSizeY };

				t1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				t5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s6StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s7StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s8StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 20, (SpriteStageSizeY - 11) };
				s9StageSelectFramePosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 20, (SpriteStageSizeY - 11) };

				t1NewLogPosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t2NewLogPosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t3NewLogPosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t4NewLogPosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				t5NewLogPosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s1NewLogPosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s2NewLogPosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s3NewLogPosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s4NewLogPosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s5NewLogPosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s6NewLogPosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s7NewLogPosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s8NewLogPosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 80, (SpriteStageSizeY + 60) };
				s9NewLogPosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 80, (SpriteStageSizeY + 60) };

				t1ClearLogPosition = { (SpriteStageSizeX + (1280 * (0 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t2ClearLogPosition = { (SpriteStageSizeX + (1280 * (1 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t3ClearLogPosition = { (SpriteStageSizeX + (1280 * (2 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t4ClearLogPosition = { (SpriteStageSizeX + (1280 * (3 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				t5ClearLogPosition = { (SpriteStageSizeX + (1280 * (4 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s1ClearLogPosition = { (SpriteStageSizeX + (1280 * (5 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s2ClearLogPosition = { (SpriteStageSizeX + (1280 * (6 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s3ClearLogPosition = { (SpriteStageSizeX + (1280 * (7 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s4ClearLogPosition = { (SpriteStageSizeX + (1280 * (8 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s5ClearLogPosition = { (SpriteStageSizeX + (1280 * (9 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s6ClearLogPosition = { (SpriteStageSizeX + (1280 * (10 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s7ClearLogPosition = { (SpriteStageSizeX + (1280 * (11 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s8ClearLogPosition = { (SpriteStageSizeX + (1280 * (12 - stageSelect))) - 150, (SpriteStageSizeY + 100) };
				s9ClearLogPosition = { (SpriteStageSizeX + (1280 * (13 - stageSelect))) - 150, (SpriteStageSizeY + 100) };

				t1Background->SetPosition(t1BackgroundPosition);
				t2Background->SetPosition(t2BackgroundPosition);
				t3Background->SetPosition(t3BackgroundPosition);
				t4Background->SetPosition(t4BackgroundPosition);
				t5Background->SetPosition(t5BackgroundPosition);
				s1Background->SetPosition(s1BackgroundPosition);
				s2Background->SetPosition(s2BackgroundPosition);
				s3Background->SetPosition(s3BackgroundPosition);
				s4Background->SetPosition(s4BackgroundPosition);
				s5Background->SetPosition(s5BackgroundPosition);
				s6Background->SetPosition(s6BackgroundPosition);
				s7Background->SetPosition(s7BackgroundPosition);
				s8Background->SetPosition(s8BackgroundPosition);
				s9Background->SetPosition(s9BackgroundPosition);

				t1StageSelectFrame->SetPosition(t1StageSelectFramePosition);
				t2StageSelectFrame->SetPosition(t2StageSelectFramePosition);
				t3StageSelectFrame->SetPosition(t3StageSelectFramePosition);
				t4StageSelectFrame->SetPosition(t4StageSelectFramePosition);
				t5StageSelectFrame->SetPosition(t5StageSelectFramePosition);
				s1StageSelectFrame->SetPosition(s1StageSelectFramePosition);
				s2StageSelectFrame->SetPosition(s2StageSelectFramePosition);
				s3StageSelectFrame->SetPosition(s3StageSelectFramePosition);
				s4StageSelectFrame->SetPosition(s4StageSelectFramePosition);
				s5StageSelectFrame->SetPosition(s5StageSelectFramePosition);
				s6StageSelectFrame->SetPosition(s6StageSelectFramePosition);
				s7StageSelectFrame->SetPosition(s7StageSelectFramePosition);
				s8StageSelectFrame->SetPosition(s8StageSelectFramePosition);
				s9StageSelectFrame->SetPosition(s9StageSelectFramePosition);

				t1NewLog->SetPosition(t1NewLogPosition);
				t2NewLog->SetPosition(t2NewLogPosition);
				t3NewLog->SetPosition(t3NewLogPosition);
				t4NewLog->SetPosition(t4NewLogPosition);
				t5NewLog->SetPosition(t4NewLogPosition);
				s1NewLog->SetPosition(s1NewLogPosition);
				s2NewLog->SetPosition(s2NewLogPosition);
				s3NewLog->SetPosition(s3NewLogPosition);
				s4NewLog->SetPosition(s4NewLogPosition);
				s5NewLog->SetPosition(s5NewLogPosition);
				s6NewLog->SetPosition(s6NewLogPosition);
				s7NewLog->SetPosition(s7NewLogPosition);
				s8NewLog->SetPosition(s8NewLogPosition);
				s9NewLog->SetPosition(s9NewLogPosition);

				t1ClearLog->SetPosition(t1ClearLogPosition);
				t2ClearLog->SetPosition(t2ClearLogPosition);
				t3ClearLog->SetPosition(t3ClearLogPosition);
				t4ClearLog->SetPosition(t4ClearLogPosition);
				t5ClearLog->SetPosition(t5ClearLogPosition);
				s1ClearLog->SetPosition(s1ClearLogPosition);
				s2ClearLog->SetPosition(s2ClearLogPosition);
				s3ClearLog->SetPosition(s3ClearLogPosition);
				s4ClearLog->SetPosition(s4ClearLogPosition);
				s5ClearLog->SetPosition(s5ClearLogPosition);
				s6ClearLog->SetPosition(s6ClearLogPosition);
				s7ClearLog->SetPosition(s7ClearLogPosition);
				s8ClearLog->SetPosition(s8ClearLogPosition);
				s9ClearLog->SetPosition(s9ClearLogPosition);

				camera->SetEye({ (stageSelect * 100.0f), 20, -30 });
				camera->SetTarget({ (stageSelect * 100.0f), 1.0f, 0 });
				menuBallRotation = { 0.0f, 0.0f, 0.0f };
				menuSelection = 0;
				break;
			}
		}

		if (input->PushKey(DIK_S) && menuSelection == 0 && !menuMoving || IsButtonDown(ButtonKind::DownButton) && menuSelection == 0 && !menuMoving)
		{
			audio->PlayWave("MoveCursor.wav", Volume, false);
			menuMoving = true;
		}
		else if (input->PushKey(DIK_W) && menuSelection == 1 && !menuMoving || IsButtonDown(ButtonKind::UpButton) && menuSelection == 1 && !menuMoving)
		{
			audio->PlayWave("MoveCursor.wav", Volume, false);
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

	case 4: // ステージ1
#pragma region case4 ステージ1

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
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}
			if (intersect(playerPosition, FloorButton, 1.0f, 1.0f, 1.0f) && lastYellowIntersct1 == false)
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (doorOpen1)
			{
				for (auto object_s2_y : objects_s2_y) {
					XMFLOAT3 objectPosition = object_s2_y->GetPosition();
					object_s2_y->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s2_y->Update();
				}

				for (auto object_s2_y2 : objects_s2_y2) {
					XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
					object_s2_y2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s2_y2->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					stage2YellowKabe = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (doorOpen2)
			{
				for (auto object_s2_s : objects_s2_s) {
					XMFLOAT3 objectPosition = object_s2_s->GetPosition();
					object_s2_s->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s2_s->Update();
				}

				for (auto object_s2_s2 : objects_s2_s2) {
					XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
					object_s2_s2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s2_s2->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					stage2Switch = true;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage1Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s1FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage1Move();
				GameClearReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				gameClear->Initialize();

				s1FirstPlayFlag = false;
				s1ClearFlag = true;
			}

			lastYellowIntersct1 = intersect(playerPosition, yellowTrigger1, 1.0f, 1.0f, 1.0f);
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

			if (Tutorial)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = 12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = 12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = 12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = 12.0f;

				objPlayerStand->SetRotation({ 0, 140, 0 });
				objCloneStand->SetRotation({ 0, -140, 0 });

				playerMarkerPosition = { -20.0f, 0.0f, 12.0f };
				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);

				if (delay)
				{
					delayFrame++;

					if (delayFrame >= 5)
					{
						delayFrame = 0;
						delay = false;
					}
				}

				if (input->TriggerKey(DIK_SPACE) || IsButtonDown(ButtonKind::Button_A))
				{
					audio->PlayWave("Decision.wav", Volume, false);
					if (TutorialNo >= 5)
					{
						Tutorial = false;
					}
					else
					{
						TutorialNo++;
					}
				}
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
		objPlayerTalk->Update();

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

			objFighter->SetPosition({ -20, 30, -12 });
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

			if (Tutorial)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerStand->SetRotation({ 0, 320, 0 });
				objCloneStand->SetRotation({ 0, -320, 0 });

				playerMarkerPosition = { -20.0f, 0.0f, -12.0f };
				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);

				if (delay)
				{
					delayFrame++;

					if (delayFrame >= 5)
					{
						delayFrame = 0;
						delay = false;
					}
				}

				if (input->TriggerKey(DIK_SPACE) || IsButtonDown(ButtonKind::Button_A))
				{
					audio->PlayWave("Decision.wav", Volume, false);
					if (TutorialNo >= 6)
					{
						Tutorial = false;
					}
					else
					{
						TutorialNo++;
					}
				}
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
		objPlayerTalk->Update();

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

			if (Tutorial)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerStand->SetRotation({ 0, 320, 0 });
				objCloneStand->SetRotation({ 0, -320, 0 });

				playerMarkerPosition = { -20.0f, 0.0f, -12.0f };
				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);

				if (delay)
				{
					delayFrame++;

					if (delayFrame >= 5)
					{
						delayFrame = 0;
						delay = false;
					}
				}

				if (input->TriggerKey(DIK_SPACE) || IsButtonDown(ButtonKind::Button_A))
				{
					audio->PlayWave("Decision.wav", Volume, false);
					if (TutorialNo >= 6)
					{
						Tutorial = false;
					}
					else
					{
						TutorialNo++;
					}
				}
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

			if (intersect(enemyPosition, RedButton1, 1.0f, 1.0f, 1.0f) && doorOpen1 == false)
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}
			if (intersect(enemyPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}
			if (intersect(playerPosition, GreenButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, GreenButton2, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag3 == false)
				{
					SwitchFlag3 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen3 = true;
			}

			if (doorOpen1)
			{
				for (auto object_t3_y1_1 : objects_t3_y1_1) {
					XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
					object_t3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t3_y1_1->Update();
				}

				for (auto object_t3_y2_1 : objects_t3_y2_1) {
					XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
					object_t3_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t3_y2_1->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					tutorial3YellowKabe1 = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (doorOpen2)
			{
				for (auto object_t3_y1_2 : objects_t3_y1_2) {
					XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
					object_t3_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t3_y1_2->Update();
				}

				for (auto object_t3_y2_2 : objects_t3_y2_2) {
					XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
					object_t3_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t3_y2_2->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					tutorial3YellowKabe2 = false;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (doorOpen3)
			{
				for (auto object_t3_y1_3 : objects_t3_y1_3) {
					XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
					object_t3_y1_3->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t3_y1_3->Update();
				}

				for (auto object_t3_y2_3 : objects_t3_y2_3) {
					XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
					object_t3_y2_3->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t3_y2_3->Update();
				}

				doorCount3++;

				if (doorCount3 >= 15)
				{
					tutorial3YellowKabe3 = false;
					doorCount3 = 0;
					doorOpen3 = false;
				}
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
		objPlayerTalk->Update();

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

		if (input->TriggerKey(DIK_D) && stageMoveRight == false && stageMoveLeft == false && stageSelect < 13 ||
			IsButtonDown(ButtonKind::Button_RB) && stageMoveRight == false && stageMoveLeft == false && stageSelect < 13)
		{
			audio->PlayWave("StageSelection.wav", Volume, false);
			stageMoveRight = true;
			stageSelect++;
		}

		if (input->TriggerKey(DIK_A) && stageMoveRight == false && stageMoveLeft == false && stageSelect > 0 ||
			IsButtonDown(ButtonKind::Button_LB) && stageMoveRight == false && stageMoveLeft == false && stageSelect > 0)
		{
			audio->PlayWave("StageSelection.wav", Volume, false);
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
			t5BackgroundPosition.x -= 25.6f;
			s1BackgroundPosition.x -= 25.6f;
			s2BackgroundPosition.x -= 25.6f;
			s3BackgroundPosition.x -= 25.6f;
			s4BackgroundPosition.x -= 25.6f;
			s5BackgroundPosition.x -= 25.6f;
			s6BackgroundPosition.x -= 25.6f;
			s7BackgroundPosition.x -= 25.6f;
			s8BackgroundPosition.x -= 25.6f;
			s9BackgroundPosition.x -= 25.6f;

			t1StageSelectFramePosition.x -= 25.6f;
			t2StageSelectFramePosition.x -= 25.6f;
			t3StageSelectFramePosition.x -= 25.6f;
			t4StageSelectFramePosition.x -= 25.6f;
			t5StageSelectFramePosition.x -= 25.6f;
			s1StageSelectFramePosition.x -= 25.6f;
			s2StageSelectFramePosition.x -= 25.6f;
			s3StageSelectFramePosition.x -= 25.6f;
			s4StageSelectFramePosition.x -= 25.6f;
			s5StageSelectFramePosition.x -= 25.6f;
			s6StageSelectFramePosition.x -= 25.6f;
			s7StageSelectFramePosition.x -= 25.6f;
			s8StageSelectFramePosition.x -= 25.6f;
			s9StageSelectFramePosition.x -= 25.6f;

			t1NewLogPosition.x -= 25.6f;
			t2NewLogPosition.x -= 25.6f;
			t3NewLogPosition.x -= 25.6f;
			t4NewLogPosition.x -= 25.6f;
			t5NewLogPosition.x -= 25.6f;
			s1NewLogPosition.x -= 25.6f;
			s2NewLogPosition.x -= 25.6f;
			s3NewLogPosition.x -= 25.6f;
			s4NewLogPosition.x -= 25.6f;
			s5NewLogPosition.x -= 25.6f;
			s6NewLogPosition.x -= 25.6f;
			s7NewLogPosition.x -= 25.6f;
			s8NewLogPosition.x -= 25.6f;
			s9NewLogPosition.x -= 25.6f;

			t1ClearLogPosition.x -= 25.6f;
			t2ClearLogPosition.x -= 25.6f;
			t3ClearLogPosition.x -= 25.6f;
			t4ClearLogPosition.x -= 25.6f;
			t5ClearLogPosition.x -= 25.6f;
			s1ClearLogPosition.x -= 25.6f;
			s2ClearLogPosition.x -= 25.6f;
			s3ClearLogPosition.x -= 25.6f;
			s4ClearLogPosition.x -= 25.6f;
			s5ClearLogPosition.x -= 25.6f;
			s6ClearLogPosition.x -= 25.6f;
			s7ClearLogPosition.x -= 25.6f;
			s8ClearLogPosition.x -= 25.6f;
			s9ClearLogPosition.x -= 25.6f;

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
			t5BackgroundPosition.x += 25.6f;
			s1BackgroundPosition.x += 25.6f;
			s2BackgroundPosition.x += 25.6f;
			s3BackgroundPosition.x += 25.6f;
			s4BackgroundPosition.x += 25.6f;
			s5BackgroundPosition.x += 25.6f;
			s6BackgroundPosition.x += 25.6f;
			s7BackgroundPosition.x += 25.6f;
			s8BackgroundPosition.x += 25.6f;
			s9BackgroundPosition.x += 25.6f;

			t1StageSelectFramePosition.x += 25.6f;
			t2StageSelectFramePosition.x += 25.6f;
			t3StageSelectFramePosition.x += 25.6f;
			t4StageSelectFramePosition.x += 25.6f;
			t5StageSelectFramePosition.x += 25.6f;
			s1StageSelectFramePosition.x += 25.6f;
			s2StageSelectFramePosition.x += 25.6f;
			s3StageSelectFramePosition.x += 25.6f;
			s4StageSelectFramePosition.x += 25.6f;
			s5StageSelectFramePosition.x += 25.6f;
			s6StageSelectFramePosition.x += 25.6f;
			s7StageSelectFramePosition.x += 25.6f;
			s8StageSelectFramePosition.x += 25.6f;
			s9StageSelectFramePosition.x += 25.6f;

			t1NewLogPosition.x += 25.6f;
			t2NewLogPosition.x += 25.6f;
			t3NewLogPosition.x += 25.6f;
			t4NewLogPosition.x += 25.6f;
			t5NewLogPosition.x += 25.6f;
			s1NewLogPosition.x += 25.6f;
			s2NewLogPosition.x += 25.6f;
			s3NewLogPosition.x += 25.6f;
			s4NewLogPosition.x += 25.6f;
			s5NewLogPosition.x += 25.6f;
			s6NewLogPosition.x += 25.6f;
			s7NewLogPosition.x += 25.6f;
			s8NewLogPosition.x += 25.6f;
			s9NewLogPosition.x += 25.6f;

			t1ClearLogPosition.x += 25.6f;
			t2ClearLogPosition.x += 25.6f;
			t3ClearLogPosition.x += 25.6f;
			t4ClearLogPosition.x += 25.6f;
			t5ClearLogPosition.x += 25.6f;
			s1ClearLogPosition.x += 25.6f;
			s2ClearLogPosition.x += 25.6f;
			s3ClearLogPosition.x += 25.6f;
			s4ClearLogPosition.x += 25.6f;
			s5ClearLogPosition.x += 25.6f;
			s6ClearLogPosition.x += 25.6f;
			s7ClearLogPosition.x += 25.6f;
			s8ClearLogPosition.x += 25.6f;
			s9ClearLogPosition.x += 25.6f;

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

		if (stageMoveLeft == false && stageMoveRight == false && input->TriggerKey(DIK_SPACE) && !delay ||
			stageMoveLeft == false && stageMoveRight == false && IsButtonDown(ButtonKind::Button_A) && !delay)
		{
			PlayFlag = false;
			audio->PlayWave("Decision.wav", Volume, false);
			audio->StopWave("Title.wav");
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
				sceneNo = 11;

				break;
			case 4:
				Tutorial5Reset();
				sceneNo = 12;

				break;
			case 5:
				Stage1Reset();
				sceneNo = 4;

				break;
			case 6:
				Stage2Reset();
				sceneNo = 9;

				break;
			case 7:
				Stage3Reset();
				sceneNo = 1;

				break;
			case 8:
				Stage4Reset();
				sceneNo = 14;

				break;
			case 9:
				Stage5Reset();
				sceneNo = 15;

				break;
			case 10:
				Stage6Reset();
				sceneNo = 16;
				
				break;
			case 11:
				Stage7Reset();
				sceneNo = 10;

				break;
			case 12:
				Stage8Reset();
				sceneNo = 13;

				break;
			case 13:
				Stage9Reset();
				sceneNo = 17;

				break;
			}
		}

		// 移動後の座標をセット
		t1Background->SetPosition(t1BackgroundPosition);
		t2Background->SetPosition(t2BackgroundPosition);
		t3Background->SetPosition(t3BackgroundPosition);
		t4Background->SetPosition(t4BackgroundPosition);
		t5Background->SetPosition(t5BackgroundPosition);
		s1Background->SetPosition(s1BackgroundPosition);
		s2Background->SetPosition(s2BackgroundPosition);
		s3Background->SetPosition(s3BackgroundPosition);
		s4Background->SetPosition(s4BackgroundPosition);
		s5Background->SetPosition(s5BackgroundPosition);
		s6Background->SetPosition(s6BackgroundPosition);
		s7Background->SetPosition(s7BackgroundPosition);
		s8Background->SetPosition(s8BackgroundPosition);
		s9Background->SetPosition(s9BackgroundPosition);

		t1StageSelectFrame->SetPosition(t1StageSelectFramePosition);
		t2StageSelectFrame->SetPosition(t2StageSelectFramePosition);
		t3StageSelectFrame->SetPosition(t3StageSelectFramePosition);
		t4StageSelectFrame->SetPosition(t4StageSelectFramePosition);
		t5StageSelectFrame->SetPosition(t5StageSelectFramePosition);
		s1StageSelectFrame->SetPosition(s1StageSelectFramePosition);
		s2StageSelectFrame->SetPosition(s2StageSelectFramePosition);
		s3StageSelectFrame->SetPosition(s3StageSelectFramePosition);
		s4StageSelectFrame->SetPosition(s4StageSelectFramePosition);
		s5StageSelectFrame->SetPosition(s5StageSelectFramePosition);
		s6StageSelectFrame->SetPosition(s6StageSelectFramePosition);
		s7StageSelectFrame->SetPosition(s7StageSelectFramePosition);
		s8StageSelectFrame->SetPosition(s8StageSelectFramePosition);
		s9StageSelectFrame->SetPosition(s9StageSelectFramePosition);

		t1NewLog->SetPosition(t1NewLogPosition);
		t2NewLog->SetPosition(t2NewLogPosition);
		t3NewLog->SetPosition(t3NewLogPosition);
		t4NewLog->SetPosition(t4NewLogPosition);
		t5NewLog->SetPosition(t5NewLogPosition);
		s1NewLog->SetPosition(s1NewLogPosition);
		s2NewLog->SetPosition(s2NewLogPosition);
		s3NewLog->SetPosition(s3NewLogPosition);
		s4NewLog->SetPosition(s4NewLogPosition);
		s5NewLog->SetPosition(s5NewLogPosition);
		s6NewLog->SetPosition(s6NewLogPosition);
		s7NewLog->SetPosition(s7NewLogPosition);
		s8NewLog->SetPosition(s8NewLogPosition);
		s9NewLog->SetPosition(s9NewLogPosition);

		t1ClearLog->SetPosition(t1ClearLogPosition);
		t2ClearLog->SetPosition(t2ClearLogPosition);
		t3ClearLog->SetPosition(t3ClearLogPosition);
		t4ClearLog->SetPosition(t4ClearLogPosition);
		t5ClearLog->SetPosition(t5ClearLogPosition);
		s1ClearLog->SetPosition(s1ClearLogPosition);
		s2ClearLog->SetPosition(s2ClearLogPosition);
		s3ClearLog->SetPosition(s3ClearLogPosition);
		s4ClearLog->SetPosition(s4ClearLogPosition);
		s5ClearLog->SetPosition(s5ClearLogPosition);
		s6ClearLog->SetPosition(s6ClearLogPosition);
		s7ClearLog->SetPosition(s7ClearLogPosition);
		s8ClearLog->SetPosition(s8ClearLogPosition);
		s9ClearLog->SetPosition(s9ClearLogPosition);

		objSkydome->Update();

		camera->Update();
#pragma endregion
		break;

	case 9: // ステージ 2
#pragma region case9 ステージ2
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
				//tutorial4YellowKabe = false;
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_t4_y : objects_t4_y) {
					XMFLOAT3 objectPosition = object_t4_y->GetPosition();
					object_t4_y->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t4_y->Update();
				}

				for (auto object_t4_y2 : objects_t4_y2) {
					XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
					object_t4_y2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t4_y2->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					tutorial4YellowKabe = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
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
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn2->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut2->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(playerPosition, objTeleporterIn3->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objFighter->SetPosition(objTeleporterOut3->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn4->GetPosition(), 1.0f, 3.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut4->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
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
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Stage2Move();
				GameClearReset();
				sceneChange = 0;

				s2FirstPlayFlag = false;
				s2ClearFlag = true;
			}
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

	case 10: //ステージ　7
#pragma region case10 ステージ7
		if (!beginStage)
		{
			if (seventhTime)
			{
				seventhTime = false;
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
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn2->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut2->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn3->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut3->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn4->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut4->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, RedButton1, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_s3_y1_1 : objects_s3_y1_1) {
					XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
					object_s3_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s3_y1_1->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					stage3YellowKabe1 = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
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
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (doorOpen2)
			{
				for (auto object_s3_y2_1 : objects_s3_y2_1) {
					XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
					object_s3_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s3_y2_1->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					stage3YellowKabe2 = false;
					doorCount2 = 0;
					doorOpen2 = false;
				}
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
				if (SwitchFlag3 == false)
				{
					SwitchFlag3 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen3 = true;
			}

			if (doorOpen3)
			{
				for (auto object_s3_y2_2 : objects_s3_y2_2) {
					XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
					object_s3_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s3_y2_2->Update();
				}

				doorCount3++;

				if (doorCount3 >= 15)
				{
					stage3YellowKabe3 = false;
					doorCount3 = 0;
					doorOpen3 = false;
				}
			}

			if (!stage3YellowKabe3)
			{
				for (auto object_s3_y2_2 : objects_s3_y2_2) {
					XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
					object_s3_y2_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s3_y2_2->Update();
				}
			}

			if (intersect(playerPosition, YellowButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag4 == false)
				{
					SwitchFlag4 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen4 = true;
			}

			if (doorOpen4)
			{
				for (auto object_s3_y1_2 : objects_s3_y1_2) {
					XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
					object_s3_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s3_y1_2->Update();
				}

				doorCount4++;

				if (doorCount4 >= 15)
				{
					stage3YellowKabe4 = false;
					doorCount4 = 0;
					doorOpen4 = false;
				}
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
				Stage7Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s7FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage7Move();
				GameClearReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				gameClear->Initialize();

				s7FirstPlayFlag = false;
				s7ClearFlag = true;
			}
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

	case 11: //チュートリアル　４
#pragma region case11 チュートリアル４
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
				lastScene = 11;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -12 });
			objClone->SetPosition({ 20, 30, -12 });

			playerPositionTemp = { -20, 30, -12 };
			clonePositionTemp = { 20, 30, -12 };
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
				objCloneFall->SetRotation({ 0, -320, 0 });

				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);
			}

			if (Tutorial)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerStand->SetRotation({ 0, 320, 0 });
				objCloneStand->SetRotation({ 0, -320, 0 });

				playerMarkerPosition = { -20.0f, 0.0f, -12.0f };
				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);

				if (delay)
				{
					delayFrame++;

					if (delayFrame >= 5)
					{
						delayFrame = 0;
						delay = false;
					}
				}

				if (input->TriggerKey(DIK_SPACE) || IsButtonDown(ButtonKind::Button_A))
				{
					audio->PlayWave("Decision.wav", Volume, false);
					if (TutorialNo >= 4)
					{
						Tutorial = false;
					}
					else
					{
						TutorialNo++;
					}
				}
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

			if (intersect(enemyPosition, objTeleporterIn1->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut1->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
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
		}

		for (auto object_at4_1 : objects_at4_1) {
			object_at4_1->Update();
		}

		for (auto object_at4_2 : objects_at4_2) {
			object_at4_2->Update();
		}

		XMFLOAT3 teleRotationIn_1 = objTeleporterIn1->GetRotation();
		teleRotationIn_1.y += WarpRotate;
		objTeleporterIn1->SetRotation(teleRotationIn_1);

		XMFLOAT3 teleRotationOut_1 = objTeleporterOut1->GetRotation();
		teleRotationOut_1.y += WarpRotate;
		objTeleporterOut1->SetRotation(teleRotationOut_1);

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();
		objPlayerFall->Update();
		objPlayerTalk->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objTeleporterIn1->Update();
		objTeleporterOut1->Update();

		objSkydome->Update();

		camera->Update();

		collisionManager->CheckAllCollisions();
#pragma endregion
		break;
	
	case 12: //チュートリアル　５
#pragma region case12 チュートリアル5
		if (!beginStage)
		{
			if (t5Time)
			{
				t5Time = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 12;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -12 });
			objClone->SetPosition({ 20, 30, -12 });

			playerPositionTemp = { -20, 30, -12 };
			clonePositionTemp = { 20, 30, -12 };
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

			if (Tutorial)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -12.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -12.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -12.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -12.0f;

				objPlayerStand->SetRotation({ 0, 320, 0 });
				objCloneStand->SetRotation({ 0, -320, 0 });

				playerMarkerPosition = { -20.0f, 0.0f, -12.0f };
				objFighter->SetPosition(playerPositionTemp);
				objClone->SetPosition(clonePositionTemp);

				if (delay)
				{
					delayFrame++;

					if (delayFrame >= 5)
					{
						delayFrame = 0;
						delay = false;
					}
				}

				if (input->TriggerKey(DIK_SPACE) || IsButtonDown(ButtonKind::Button_A))
				{
					audio->PlayWave("Decision.wav", Volume, false);
					if (TutorialNo >= 5)
					{
						Tutorial = false;
					}
					else
					{
						TutorialNo++;
					}
				}
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

			if (intersect(playerPosition, FloorButton, 1.0f, 1.0f, 1.0f))
			{
				//tutorial5Switch = true;
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_t5_s2 : objects_t5_s2) {
					XMFLOAT3 objectPosition = object_t5_s2->GetPosition();
					object_t5_s2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_t5_s2->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					tutorial5Switch = true;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (tutorial5Switch)
			{
				for (auto object_t5_s2 : objects_t5_s2) {
					XMFLOAT3 objectPosition = object_t5_s2->GetPosition();
					object_t5_s2->SetPosition({ objectPosition.x, -150.0f, objectPosition.z });
					object_t5_s2->Update();
				}
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Tutorial5Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				t5FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Tutorial5Move();
				GameClearReset();
				sceneChange = 0;

				t5FirstPlayFlag = false;
				t5ClearFlag = true;
			}
		}

		for (auto object_t5_1 : objects_t5_1) {
			object_t5_1->Update();
		}

		for (auto object_t5_2 : objects_t5_2) {
			object_t5_2->Update();
		}

		for (auto object_t5_s2 : objects_t5_s2) {
			object_t5_s2->Update();
		}

		objFighter->Update();
		objClone->Update();

		objPlayerRun->Update();
		objPlayerStand->Update();
		objPlayerFall->Update();
		objPlayerTalk->Update();

		objCloneRun->Update();
		objCloneStand->Update();
		objCloneFall->Update();

		objButtonFloor->Update();

		objSkydome->Update();

		camera->Update();

		collisionManager->CheckAllCollisions();
#pragma endregion
		break;

	case 13: //ステージ 8
#pragma region case13 ステージ8
		if (!beginStage)
		{
			if (eigthTime)
			{
				eigthTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 13;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -9 });
			objClone->SetPosition({ 20, 30, -9 });

			playerPositionTemp = { -20, 30, -9 };
			clonePositionTemp = { 20, 30, -9 };
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -20.0f;
				playerPosition.z = -9.0f;
				playerPositionTemp.x = -20.0f;
				playerPositionTemp.z = -9.0f;

				enemyPosition.x = 20.0f;
				enemyPosition.z = -9.0f;
				clonePositionTemp.x = 20.0f;
				clonePositionTemp.z = -9.0f;

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

			if (intersect(playerPosition, objTeleporterIn1->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objFighter->SetPosition(objTeleporterOut3->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn3->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut1->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(enemyPosition, objTeleporterIn2->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				objClone->SetPosition(objTeleporterOut2->GetPosition());
				audio->PlayWave("Warp.wav", Volume, false);
			}

			if (intersect(playerPosition, RedButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, RedButton2, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (intersect(playerPosition, GreenButton2, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, GreenButton1, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (intersect(playerPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag3 == false)
				{
					SwitchFlag3 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen3 = true;
			}

			if (doorOpen1)
			{
				for (auto object_s6_y1_1 : objects_s6_y1_1) {
					XMFLOAT3 objectPosition = object_s6_y1_1->GetPosition();
					object_s6_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s6_y1_1->Update();
				}

				for (auto object_s6_y2_1 : objects_s6_y2_1) {
					XMFLOAT3 objectPosition = object_s6_y2_1->GetPosition();
					object_s6_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s6_y2_1->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					stage6YellowKabe1 = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (doorOpen2)
			{
				for (auto object_s6_y1_2 : objects_s6_y1_2) {
					XMFLOAT3 objectPosition = object_s6_y1_2->GetPosition();
					object_s6_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s6_y1_2->Update();
				}

				for (auto object_s6_y2_2 : objects_s6_y2_2) {
					XMFLOAT3 objectPosition = object_s6_y2_2->GetPosition();
					object_s6_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s6_y2_2->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					stage6YellowKabe2 = false;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (doorOpen3)
			{
				for (auto object_s6_y1_3 : objects_s6_y1_3) {
					XMFLOAT3 objectPosition = object_s6_y1_3->GetPosition();
					object_s6_y1_3->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s6_y1_3->Update();
				}

				for (auto object_s6_y2_3 : objects_s6_y2_3) {
					XMFLOAT3 objectPosition = object_s6_y2_3->GetPosition();
					object_s6_y2_3->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s6_y2_3->Update();
				}

				for (auto object_s6_y2_4 : objects_s6_y2_4) {
					XMFLOAT3 objectPosition = object_s6_y2_4->GetPosition();
					object_s6_y2_4->SetPosition({ objectPosition.x, objectPosition.y + 1.0f, objectPosition.z });
					object_s6_y2_4->Update();
				}

				doorCount3++;

				if (doorCount3 >= 15)
				{
					stage6YellowKabe3 = false;
					doorCount3 = 0;
					doorOpen3 = false;
				}
			}

			if (!stage6YellowKabe1)
			{
				for (auto object_s6_y1_1 : objects_s6_y1_1) {
					XMFLOAT3 objectPosition = object_s6_y1_1->GetPosition();
					object_s6_y1_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s6_y1_1->Update();
				}

				for (auto object_s6_y2_1 : objects_s6_y2_1) {
					XMFLOAT3 objectPosition = object_s6_y2_1->GetPosition();
					object_s6_y2_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s6_y2_1->Update();
				}
			}

			if (!stage6YellowKabe2)
			{
				for (auto object_s6_y1_2 : objects_s6_y1_2) {
					XMFLOAT3 objectPosition = object_s6_y1_2->GetPosition();
					object_s6_y1_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s6_y1_2->Update();
				}

				for (auto object_s6_y2_2 : objects_s6_y2_2) {
					XMFLOAT3 objectPosition = object_s6_y2_2->GetPosition();
					object_s6_y2_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s6_y2_2->Update();
				}
			}

			if (!stage6YellowKabe3)
			{
				for (auto object_s6_y1_3 : objects_s6_y1_3) {
					XMFLOAT3 objectPosition = object_s6_y1_3->GetPosition();
					object_s6_y1_3->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s6_y1_3->Update();
				}

				for (auto object_s6_y2_3 : objects_s6_y2_3) {
					XMFLOAT3 objectPosition = object_s6_y2_3->GetPosition();
					object_s6_y2_3->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s6_y2_3->Update();
				}

				for (auto object_s6_y2_4 : objects_s6_y2_4) {
					XMFLOAT3 objectPosition = object_s6_y2_4->GetPosition();
					object_s6_y2_4->SetPosition({ objectPosition.x, 0, objectPosition.z });
					object_s6_y2_4->Update();
				}
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage8Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s8FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Stage8Move();
				GameClearReset();
				sceneChange = 0;

				s8FirstPlayFlag = false;
				s8ClearFlag = true;
			}
		}

		for (auto object_s6_1 : objects_s6_1) {
			object_s6_1->Update();
		}

		for (auto object_s6_2 : objects_s6_2) {
			object_s6_2->Update();
		}

		for (auto object_s6_y1_1 : objects_s6_y1_1) {
			object_s6_y1_1->Update();
		}

		for (auto object_s6_y2_1 : objects_s6_y2_1) {
			object_s6_y2_1->Update();
		}

		for (auto object_s6_y1_2 : objects_s6_y1_2) {
			object_s6_y1_2->Update();
		}

		for (auto object_s6_y2_2 : objects_s6_y2_2) {
			object_s6_y2_2->Update();
		}

		for (auto object_s6_y1_3 : objects_s6_y1_3) {
			object_s6_y1_3->Update();
		}

		for (auto object_s6_y2_3 : objects_s6_y2_3) {
			object_s6_y2_3->Update();
		}

		for (auto object_s6_y2_4 : objects_s6_y2_4) {
			object_s6_y2_4->Update();
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
		objButtonGreen1->Update();
		objButtonGreen2->Update();
		objButtonBlue->Update();

		objTeleporterIn1->Update();
		objTeleporterOut1->Update();
		objTeleporterIn2->Update();
		objTeleporterOut2->Update();
		objTeleporterIn3->Update();
		objTeleporterOut3->Update();

		objSkydome->Update();

		camera->Update();

		collisionManager->CheckAllCollisions();
#pragma endregion
		break;

	case 14: //ステージ 4
#pragma region case14 ステージ4
		if (!beginStage)
		{
			if (fourthTime)
			{
				fourthTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 14;
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

			if (intersect(playerPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_s4_y2 : objects_s4_y2) {
					XMFLOAT3 objectPosition = object_s4_y2->GetPosition();
					object_s4_y2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s4_y2->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					stage4YellowKabe = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (!stage4YellowKabe)
			{
				for (auto object_s4_y2 : objects_s4_y2) {
					XMFLOAT3 objectPosition = object_s4_y2->GetPosition();
					object_s4_y2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s4_y2->Update();
				}
			}

			if (intersect(enemyPosition, FloorButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					//audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (doorOpen2)
			{
				for (auto object_s4_y : objects_s4_y) {
					XMFLOAT3 objectPosition = object_s4_y->GetPosition();
					object_s4_y->SetPosition({ objectPosition.x, objectPosition.y + 1.0f, objectPosition.z });
					object_s4_y->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					stage4Switch = true;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (stage4Switch)
			{
				for (auto object_s4_y : objects_s4_y) {
					XMFLOAT3 objectPosition = object_s4_y->GetPosition();
					object_s4_y->SetPosition({ objectPosition.x, 0, objectPosition.z });
					object_s4_y->Update();
				}
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage4Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s4FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Stage4Move();
				GameClearReset();
				sceneChange = 0;

				s4FirstPlayFlag = false;
				s4ClearFlag = true;
			}
		}

		for (auto object_s4_1 : objects_s4_1)
		{
			object_s4_1->Update();
		}

		for (auto object_s4_2 : objects_s4_2)
		{
			object_s4_2->Update();
		}

		for (auto object_s4_y : objects_s4_y)
		{
			object_s4_y->Update();
		}

		for (auto object_s4_y2 : objects_s4_y2)
		{
			object_s4_y2->Update();
		}

		objButtonBlue->Update();
		objButtonFloor->Update();

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

	case 15: //ステージ5
#pragma region case15 ステージ5
		if (!beginStage)
		{
			if (fifthTime)
			{
				fifthTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 15;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -20, 30, -12 });
			objClone->SetPosition({ 8, 30, 12 });

			playerPositionTemp = { -20, 30, -12 };
			clonePositionTemp = { 8, 30, 12 };

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

				enemyPosition.x = 8.0f;
				enemyPosition.z = 12.0f;
				clonePositionTemp.x = 8.0f;
				clonePositionTemp.z = 12.0f;

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

			if (intersect(playerPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_s5_y2_1 : objects_s5_y2_1) {
					XMFLOAT3 objectPosition = object_s5_y2_1->GetPosition();
					object_s5_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s5_y2_1->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					stage5YellowKabe1 = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (!stage5YellowKabe1)
			{
				for (auto object_s5_y2_1 : objects_s5_y2_1) {
					XMFLOAT3 objectPosition = object_s5_y2_1->GetPosition();
					object_s5_y2_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s5_y2_1->Update();
				}
			}

			if (intersect(enemyPosition, FloorButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					//audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (doorOpen2)
			{
				for (auto object_s5_y1_1 : objects_s5_y1_1) {
					XMFLOAT3 objectPosition = object_s5_y1_1->GetPosition();
					object_s5_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s5_y1_1->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					stage5Switch = true;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (stage5Switch)
			{
				for (auto object_s5_y1_1 : objects_s5_y1_1) {
					XMFLOAT3 objectPosition = object_s5_y1_1->GetPosition();
					object_s5_y1_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s5_y1_1->Update();
				}
			}

			if (intersect(playerPosition, GreenButton1, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag3 == false)
				{
					SwitchFlag3 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen3 = true;
			}

			if (doorOpen3)
			{
				for (auto object_s5_y2_2 : objects_s5_y2_2) {
					XMFLOAT3 objectPosition = object_s5_y2_2->GetPosition();
					object_s5_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s5_y2_2->Update();
				}

				doorCount3++;

				if (doorCount3 >= 15)
				{
					stage5YellowKabe2 = false;
					doorCount3 = 0;
					doorOpen3 = false;
				}
			}

			if (!stage5YellowKabe2)
			{
				for (auto object_s5_y2_2 : objects_s5_y2_2) {
					XMFLOAT3 objectPosition = object_s5_y2_2->GetPosition();
					object_s5_y2_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s5_y2_2->Update();
				}
			}

			if (intersect(enemyPosition, RedButton1, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag4 == false)
				{
					SwitchFlag4 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen4 = true;
			}

			if (doorOpen4)
			{
				for (auto object_s5_y1_2 : objects_s5_y1_2) {
					XMFLOAT3 objectPosition = object_s5_y1_2->GetPosition();
					object_s5_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s5_y1_2->Update();
				}

				doorCount4++;

				if (doorCount4 >= 15)
				{
					stage5YellowKabe3 = false;
					doorCount4 = 0;
					doorOpen4 = false;
				}
			}

			if (!stage5YellowKabe3)
			{
				for (auto object_s5_y1_2 : objects_s5_y1_2) {
					XMFLOAT3 objectPosition = object_s5_y1_2->GetPosition();
					object_s5_y1_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s5_y1_2->Update();
				}
			}

			if (intersect(playerPosition, YellowButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag5 == false)
				{
					SwitchFlag5 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen5 = true;
			}

			if (doorOpen5)
			{
				for (auto object_s5_y2_3 : objects_s5_y2_3) {
					XMFLOAT3 objectPosition = object_s5_y2_3->GetPosition();
					object_s5_y2_3->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s5_y2_3->Update();
				}

				doorCount5++;

				if (doorCount5 >= 15)
				{
					stage5YellowKabe4 = false;
					doorCount5 = 0;
					doorOpen5 = false;
				}
			}

			if (!stage5YellowKabe4)
			{
				for (auto object_s5_y2_3 : objects_s5_y2_3) {
					XMFLOAT3 objectPosition = object_s5_y2_3->GetPosition();
					object_s5_y2_3->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s5_y2_3->Update();
				}
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage5Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s5FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Stage5Move();
				GameClearReset();
				sceneChange = 0;

				s5FirstPlayFlag = false;
				s5ClearFlag = true;
			}
		}

		for (auto object_s5_1 : objects_s5_1)
		{
			object_s5_1->Update();
		}

		for (auto object_s5_2 : objects_s5_2)
		{
			object_s5_2->Update();
		}

		for (auto object_s5_y1_1 : objects_s5_y1_1)
		{
			object_s5_y1_1->Update();
		}

		for (auto object_s5_y2_1 : objects_s5_y2_1)
		{
			object_s5_y2_1->Update();
		}

		for (auto object_s5_y1_2 : objects_s5_y1_2)
		{
			object_s5_y1_2->Update();
		}

		for (auto object_s5_y2_2 : objects_s5_y2_2)
		{
			object_s5_y2_2->Update();
		}

		for (auto object_s5_y2_3 : objects_s5_y2_3)
		{
			object_s5_y2_3->Update();
		}

		objButtonBlue->Update();
		objButtonFloor->Update();
		objButtonRed1->Update();
		objButtonGreen1->Update();
		objButtonYellow->Update();

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

	case 16: //ステージ６
#pragma region case16 ステージ6
		if (!beginStage)
		{
			if (sixthTime)
			{
				sixthTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 16;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -14, 30, 0 });
			objClone->SetPosition({ 14, 30, 0 });

			playerPositionTemp = { -14, 30, 0 };
			clonePositionTemp = { 14, 30, 0 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -14.0f;
				playerPosition.z = 0.0f;
				playerPositionTemp.x = -14.0f;
				playerPositionTemp.z = 0.0f;

				enemyPosition.x = 14.0f;
				enemyPosition.z = 0.0f;
				clonePositionTemp.x = 14.0f;
				clonePositionTemp.z = 0.0f;

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

			if (intersect(enemyPosition, BlueButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_as6_y1_1 : objects_as6_y1_1) {
					XMFLOAT3 objectPosition = object_as6_y1_1->GetPosition();
					object_as6_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_as6_y1_1->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					newstage6YellowKabe1 = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (!newstage6YellowKabe1)
			{
				for (auto object_as6_y1_1 : objects_as6_y1_1) {
					XMFLOAT3 objectPosition = object_as6_y1_1->GetPosition();
					object_as6_y1_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_as6_y1_1->Update();
				}
			}

			if (intersect(playerPosition, GreenButton1, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (doorOpen2)
			{
				for (auto object_as6_y2_1 : objects_as6_y2_1) {
					XMFLOAT3 objectPosition = object_as6_y2_1->GetPosition();
					object_as6_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_as6_y2_1->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					newstage6YellowKabe2 = false;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (!newstage6YellowKabe2)
			{
				for (auto object_as6_y2_1 : objects_as6_y2_1) {
					XMFLOAT3 objectPosition = object_as6_y2_1->GetPosition();
					object_as6_y2_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_as6_y2_1->Update();
				}
			}

			if (intersect(enemyPosition, FloorButton, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag3 == false)
				{
					SwitchFlag3 = true;
					audio->PlayWave("Switch.wav", Volume, false);
				}
				doorOpen3 = true;
			}

			if (doorOpen3)
			{
				for (auto object_as6_y1_2 : objects_as6_y1_2) {
					XMFLOAT3 objectPosition = object_as6_y1_2->GetPosition();
					object_as6_y1_2->SetPosition({ objectPosition.x, objectPosition.y + 1.0f, objectPosition.z });
					object_as6_y1_2->Update();
				}

				doorCount3++;

				if (doorCount3 >= 15)
				{
					newStage6switch = true;
					doorCount3 = 0;
					doorOpen3 = false;
				}
			}

			if (newStage6switch)
			{
				for (auto object_as6_y1_2 : objects_as6_y1_2) {
					XMFLOAT3 objectPosition = object_as6_y1_2->GetPosition();
					object_as6_y1_2->SetPosition({ objectPosition.x, 0, objectPosition.z });
					object_as6_y1_2->Update();
				}
			}

			if (intersect(playerPosition, BombButton1, 1.0f, 1.0f, 1.0f) && lastIntersect == false)
			{
				playerBulletF = true;
			}

			if (playerBullet.x > 14.0f)
			{
				playerBulletF = false;
			}

			if (playerBulletF)
			{
				playerBullet.x += 1.0f;
				objTempBullet->SetPosition(playerBullet);
			}
			else
			{
				playerBullet.x = -8.0f;
				playerBullet.z = 12.0f;
				objTempBullet->SetPosition(playerBullet);
			}

			if (intersect(playerBullet, enemyPosition, 1.0f, 1.0f, 1.0f) && playerBulletF == true)
			{
				enemyAlive = false;
				sceneNo = 2;
				Stage6Move();
				GameClearReset();
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				sceneChange = 0;

				s6FirstPlayFlag = false;
				s6ClearFlag = true;
			}

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage6Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s6FirstPlayFlag = false;
			}

			lastIntersect = intersect(playerPosition, BombButton1, 1.0f, 1.0f, 1.0f);
		}

		for (auto object_as6_1 : objects_as6_1)
		{
			object_as6_1->Update();
		}

		for (auto object_as6_2 : objects_as6_2)
		{
			object_as6_2->Update();
		}

		for (auto object_as6_y1_1 : objects_as6_y1_1)
		{
			object_as6_y1_1->Update();
		}

		for (auto object_as6_y2_1 : objects_as6_y2_1)
		{
			object_as6_y2_1->Update();
		}

		for (auto object_as6_y1_2 : objects_as6_y1_2)
		{
			object_as6_y1_2->Update();
		}

		objButtonBlue->Update();
		objButtonFloor->Update();
		objButtonGreen1->Update();
		objButtonBomb1->Update();
		
		objTempBullet->Update();

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

	case 17: //ステージ9
#pragma region case17 ステージ9
		if (!beginStage)
		{
			if (ninthTime)
			{
				ninthTime = false;
			}
			camera->MoveEyeVector({ -1.0f, -1.0f, -1.0f });
			camera->Update();
			currentFrame++;

			if (currentFrame >= 100)
			{
				currentFrame = 0;
				lastScene = 17;
				beginStage = true;
				falling = true;
			}

			objFighter->SetPosition({ -14, 30, -9 });
			objClone->SetPosition({ 14, 30, -9 });

			playerPositionTemp = { -14, 30, -9 };
			clonePositionTemp = { 14, 30, -9 };

			objFighter->Update();
			objClone->Update();
		}
		if (beginStage)
		{
			if (falling)
			{
				playerPosition.x = -14.0f;
				playerPosition.z = -9.0f;
				playerPositionTemp.x = -14.0f;
				playerPositionTemp.z = -9.0f;

				enemyPosition.x = 14.0f;
				enemyPosition.z = -9.0f;
				clonePositionTemp.x = 14.0f;
				clonePositionTemp.z = -9.0f;

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

			if (intersect(playerPosition, BlueButton, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, BlueButton2, 1.0f, 1.0f, 1.0f) ||
				intersect(enemyPosition, BlueButton, 1.0f, 1.0f, 1.0f) && intersect(playerPosition, BlueButton2, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag1 == false)
				{
					SwitchFlag1 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen1 = true;
			}

			if (doorOpen1)
			{
				for (auto object_s9_y2_1 : objects_s9_y2_1) {
					XMFLOAT3 objectPosition = object_s9_y2_1->GetPosition();
					object_s9_y2_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s9_y2_1->Update();
				}

				doorCount1++;

				if (doorCount1 >= 15)
				{
					stage9YellowKabe1 = false;
					doorCount1 = 0;
					doorOpen1 = false;
				}
			}

			if (!stage9YellowKabe1)
			{
				for (auto object_s9_y2_1 : objects_s9_y2_1) {
					XMFLOAT3 objectPosition = object_s9_y2_1->GetPosition();
					object_s9_y2_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s9_y2_1->Update();
				}
			}

			if (intersect(enemyPosition, GreenButton1, 1.0f, 1.0f, 1.0f) && intersect(playerPosition, GreenButton2, 1.0f, 1.0f, 1.0f) ||
				intersect(playerPosition, GreenButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, GreenButton2, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag2 == false)
				{
					SwitchFlag2 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen2 = true;
			}

			if (doorOpen2)
			{
				for (auto object_s9_y1_1 : objects_s9_y1_1) {
					XMFLOAT3 objectPosition = object_s9_y1_1->GetPosition();
					object_s9_y1_1->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s9_y1_1->Update();
				}

				doorCount2++;

				if (doorCount2 >= 15)
				{
					stage9YellowKabe2 = false;
					doorCount2 = 0;
					doorOpen2 = false;
				}
			}

			if (!stage9YellowKabe2)
			{
				for (auto object_s9_y1_1 : objects_s9_y1_1) {
					XMFLOAT3 objectPosition = object_s9_y1_1->GetPosition();
					object_s9_y1_1->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s9_y1_1->Update();
				}
			}

			if (intersect(playerPosition, RedButton1, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, RedButton2, 1.0f, 1.0f, 1.0f) ||
				intersect(enemyPosition, RedButton1, 1.0f, 1.0f, 1.0f) && intersect(playerPosition, RedButton2, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag3 == false)
				{
					SwitchFlag3 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen3 = true;
			}

			if (doorOpen3)
			{
				for (auto object_s9_y2_2 : objects_s9_y2_2) {
					XMFLOAT3 objectPosition = object_s9_y2_2->GetPosition();
					object_s9_y2_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s9_y2_2->Update();
				}

				doorCount3++;

				if (doorCount3 >= 15)
				{
					stage9YellowKabe3 = false;
					doorCount3 = 0;
					doorOpen3 = false;
				}
			}

			if (!stage9YellowKabe3)
			{
				for (auto object_s9_y2_2 : objects_s9_y2_2) {
					XMFLOAT3 objectPosition = object_s9_y2_2->GetPosition();
					object_s9_y2_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s9_y2_2->Update();
				}
			}

			if (intersect(enemyPosition, YellowButton, 1.0f, 1.0f, 1.0f) && intersect(playerPosition, YellowButton2, 1.0f, 1.0f, 1.0f) ||
				intersect(playerPosition, YellowButton, 1.0f, 1.0f, 1.0f) && intersect(enemyPosition, YellowButton2, 1.0f, 1.0f, 1.0f))
			{
				if (SwitchFlag4 == false)
				{
					SwitchFlag4 = true;
					audio->PlayWave("Switch.wav", Volume, false);
					audio->PlayWave("OpenDoor.wav", Volume, false);
				}
				doorOpen4 = true;
			}

			if (doorOpen4)
			{
				for (auto object_s9_y1_2 : objects_s9_y1_2) {
					XMFLOAT3 objectPosition = object_s9_y1_2->GetPosition();
					object_s9_y1_2->SetPosition({ objectPosition.x, objectPosition.y - 0.25f, objectPosition.z });
					object_s9_y1_2->Update();
				}

				doorCount4++;

				if (doorCount4 >= 15)
				{
					stage9YellowKabe4 = false;
					doorCount4 = 0;
					doorOpen4 = false;
				}
			}

			if (!stage9YellowKabe4)
			{
				for (auto object_s9_y1_2 : objects_s9_y1_2) {
					XMFLOAT3 objectPosition = object_s9_y1_2->GetPosition();
					object_s9_y1_2->SetPosition({ objectPosition.x, -100, objectPosition.z });
					object_s9_y1_2->Update();
				}
			}

			if (intersect(playerPosition, objTeleporterIn1->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objFighter->SetPosition(objTeleporterOut1->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn1->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objClone->SetPosition(objTeleporterOut1->GetPosition());
			}

			if (intersect(playerPosition, objTeleporterIn2->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objFighter->SetPosition(objTeleporterOut2->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn2->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objClone->SetPosition(objTeleporterOut2->GetPosition());
			}

			if (intersect(playerPosition, objTeleporterIn3->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objFighter->SetPosition(objTeleporterOut3->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn3->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objClone->SetPosition(objTeleporterOut3->GetPosition());
			}

			if (intersect(playerPosition, objTeleporterIn4->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objFighter->SetPosition(objTeleporterOut4->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn4->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objClone->SetPosition(objTeleporterOut4->GetPosition());
			}

			if (intersect(playerPosition, objTeleporterIn5->GetPosition(), 1.0f, 1.0f, 1.0f))
			{
				audio->PlayWave("Warp.wav", Volume, false);
				objFighter->SetPosition(objTeleporterOut5->GetPosition());
			}

			if (intersect(enemyPosition, objTeleporterIn5->GetPosition(), 1.0f, 1.0f, 1.0f))
			{

				objClone->SetPosition(objTeleporterOut5->GetPosition());
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

			XMFLOAT3 teleRotationIn_5 = objTeleporterIn5->GetRotation();
			teleRotationIn_5.y += WarpRotate;
			objTeleporterIn5->SetRotation(teleRotationIn_5);
			objTeleporterIn5->Update();

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

			XMFLOAT3 teleRotationOut_5 = objTeleporterOut5->GetRotation();
			teleRotationOut_5.y += WarpRotate;
			objTeleporterOut5->SetRotation(teleRotationOut_5);
			objTeleporterOut5->Update();

			if (playerPosition.y <= -10.0f)
			{
				playerAlive = false;
				sceneNo = 3;
				Stage9Move();
				GameOverReset();
				sceneChange = 0;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameOver.wav", Volume, true);
				gameOver->Initialize();

				s9FirstPlayFlag = false;
			}
			else if (enemyPosition.y <= -10.0f)
			{
				enemyAlive = false;
				sceneNo = 2;
				audio->StopWave("Stage.wav");
				audio->PlayWave("GameClear.wav", Volume, true);
				Stage9Move();
				GameClearReset();
				sceneChange = 0;

				s9FirstPlayFlag = false;
				s9ClearFlag = true;
			}
		}

		for (auto object_s9_1 : objects_s9_1)
		{
			object_s9_1->Update();
		}

		for (auto object_s9_2 : objects_s9_2)
		{
			object_s9_2->Update();
		}

		for (auto object_s9_y1_1 : objects_s9_y1_1)
		{
			object_s9_y1_1->Update();
		}

		for (auto object_s9_y2_1 : objects_s9_y2_1)
		{
			object_s9_y2_1->Update();
		}

		for (auto object_s9_y1_2 : objects_s9_y1_2)
		{
			object_s9_y1_2->Update();
		}

		for (auto object_s9_y2_2 : objects_s9_y2_2)
		{
			object_s9_y2_2->Update();
		}

		objButtonBlue->Update();
		objButtonBlue2->Update();
		objButtonRed1->Update();
		objButtonRed2->Update();
		objButtonGreen1->Update();
		objButtonGreen2->Update();
		objButtonYellow->Update();
		objButtonYellow2->Update();

		objTeleporterIn1->Update();
		objTeleporterIn2->Update();
		objTeleporterIn3->Update();
		objTeleporterIn4->Update();
		objTeleporterIn5->Update();

		objTeleporterOut1->Update();
		objTeleporterOut2->Update();
		objTeleporterOut3->Update();
		objTeleporterOut4->Update();
		objTeleporterOut5->Update();

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
	case 8:
		spriteBG->Draw();
		break;
	case 9:
		spriteBG->Draw();
		break;
	case 10:
		spriteBG->Draw();
		break;
	case 11:
		spriteBG->Draw();
		break;
	case 12:
		spriteBG->Draw();
		break;
	case 13:
		spriteBG->Draw();
		break;
	case 14:
		spriteBG->Draw();
		break;
	case 15:
		spriteBG->Draw();
		break;
	case 16:
		spriteBG->Draw();
		break;
	case 17:
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

	if (sceneNo != 0 && sceneNo != 2 && sceneNo != 3 && sceneNo != 8 && beginStage && marker)
	{
		objPlayerMarker->Draw();
	}

	if (pause)
	{
		objPlayerThinking->Draw(cmdList);
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

		// チュートリアル
		if (Tutorial)
		{
			objPlayerTalk->Draw(cmdList);
		}

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

		// チュートリアル
		if (Tutorial)
		{
			objPlayerTalk->Draw(cmdList);
		}

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

		// チュートリアル
		if (Tutorial)
		{
			objPlayerTalk->Draw(cmdList);
		}

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
	case 11:
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

		for (auto object_at4_1 : objects_at4_1) {
			object_at4_1->Draw();
		}

		for (auto object_at4_2 : objects_at4_2) {
			object_at4_2->Draw();
		}

		objTeleporterIn1->Draw();
		objTeleporterOut1->Draw();

		objSkydome->Draw();

		// チュートリアル
		if (Tutorial)
		{
			objPlayerTalk->Draw(cmdList);
		}

		break;
	case 12:
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

		for (auto object_t5_1 : objects_t5_1) {
			object_t5_1->Draw();
		}

		for (auto object_t5_2 : objects_t5_2) {
			object_t5_2->Draw();
		}

		if (!tutorial5Switch)
		{
			objButtonFloor->Draw();

			for (auto object_t5_s2 : objects_t5_s2) {
				object_t5_s2->Draw();
			}
		}

		objSkydome->Draw();

		// チュートリアル
		if (Tutorial)
		{
			objPlayerTalk->Draw(cmdList);
		}

		break;
	case 13:
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

		for (auto object_s6_1 : objects_s6_1) {
			object_s6_1->Draw();
		}

		for (auto object_s6_2 : objects_s6_2) {
			object_s6_2->Draw();
		}

		if (stage6YellowKabe1)
		{
			objButtonRed1->Draw();
			objButtonRed2->Draw();

			for (auto object_s6_y1_1 : objects_s6_y1_1) {
				object_s6_y1_1->Draw();
			}

			for (auto object_s6_y2_1 : objects_s6_y2_1) {
				object_s6_y2_1->Draw();
			}
		}

		if (stage6YellowKabe2)
		{
			objButtonGreen1->Draw();
			objButtonGreen2->Draw();

			for (auto object_s6_y1_2 : objects_s6_y1_2) {
				object_s6_y1_2->Draw();
			}

			for (auto object_s6_y2_2 : objects_s6_y2_2) {
				object_s6_y2_2->Draw();
			}
		}

		if (stage6YellowKabe3)
		{
			objButtonBlue->Draw();

			for (auto object_s6_y1_3 : objects_s6_y1_3) {
				object_s6_y1_3->Draw();
			}

			for (auto object_s6_y2_3 : objects_s6_y2_3) {
				object_s6_y2_3->Draw();
			}
		}

		if (!stage6YellowKabe3 || doorOpen3)
		{
			for (auto object_s6_y2_4 : objects_s6_y2_4) {
				object_s6_y2_4->Draw();
			}
		}

		objTeleporterIn1->Draw();
		objTeleporterOut1->Draw();
		objTeleporterIn2->Draw();
		objTeleporterOut2->Draw();
		objTeleporterIn3->Draw();
		objTeleporterOut3->Draw();

		objSkydome->Draw();

		break;
	case 14:
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

		for (auto object_s4_1 : objects_s4_1)
		{
			object_s4_1->Draw();
		}

		for (auto object_s4_2 : objects_s4_2)
		{
			object_s4_2->Draw();
		}

		if (stage4YellowKabe)
		{
			objButtonBlue->Draw();

			for (auto object_s4_y2 : objects_s4_y2)
			{
				object_s4_y2->Draw();
			}
		}

		if (doorOpen2 || stage4Switch)
		{
			for (auto object_s4_y : objects_s4_y)
			{
				object_s4_y->Draw();
			}
		}

		if (!stage4Switch)
		{
			objButtonFloor->Draw();
		}

		objSkydome->Draw();
		break;
	case 15:
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

		for (auto object_s5_1 : objects_s5_1)
		{
			object_s5_1->Draw();
		}

		for (auto object_s5_2 : objects_s5_2)
		{
			object_s5_2->Draw();
		}

		if (!stage5Switch)
		{
			objButtonFloor->Draw();
		}

		if (doorOpen2 || !stage5Switch)
		{
			for (auto object_s5_y1_1 : objects_s5_y1_1)
			{
				object_s5_y1_1->Draw();
			}
		}

		if (stage5YellowKabe1)
		{
			objButtonBlue->Draw();

			for (auto object_s5_y2_1 : objects_s5_y2_1)
			{
				object_s5_y2_1->Draw();
			}
		}

		if (stage5YellowKabe2)
		{
			objButtonGreen1->Draw();

			for (auto object_s5_y2_2 : objects_s5_y2_2)
			{
				object_s5_y2_2->Draw();
			}
		}

		if (stage5YellowKabe3)
		{
			objButtonRed1->Draw();

			for (auto object_s5_y1_2 : objects_s5_y1_2)
			{
				object_s5_y1_2->Draw();
			}
		}

		if (stage5YellowKabe4)
		{
			objButtonYellow->Draw();

			for (auto object_s5_y2_3 : objects_s5_y2_3)
			{
				object_s5_y2_3->Draw();
			}
		}

		objSkydome->Draw();
		break;
	case 16:
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

		objButtonBomb1->Draw();

		if (beginStage)
		{
			if (playerBulletF == true)
			{
				objTempBullet->Draw();
			}
		}

		for (auto object_as6_1 : objects_as6_1)
		{
			object_as6_1->Draw();
		}

		for (auto object_as6_2 : objects_as6_2)
		{
			object_as6_2->Draw();
		}

		if (!newStage6switch)
		{
			objButtonFloor->Draw();
		}

		if (doorOpen3 || newStage6switch)
		{
			for (auto object_as6_y1_2 : objects_as6_y1_2)
			{
				object_as6_y1_2->Draw();
			}
		}

		if (newstage6YellowKabe1)
		{
			objButtonBlue->Draw();

			for (auto object_as6_y1_1 : objects_as6_y1_1)
			{
				object_as6_y1_1->Draw();
			}
		}

		if (newstage6YellowKabe2)
		{
			objButtonGreen1->Draw();

			for (auto object_as6_y2_1 : objects_as6_y2_1)
			{
				object_as6_y2_1->Draw();
			}
		}

		objSkydome->Draw();
		break;
	case 17:
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

		for (auto object_s9_1 : objects_s9_1)
		{
			object_s9_1->Draw();
		}

		for (auto object_s9_2 : objects_s9_2)
		{
			object_s9_2->Draw();
		}

		if (stage9YellowKabe2)
		{
			objButtonGreen1->Draw();
			objButtonGreen2->Draw();

			for (auto object_s9_y1_1 : objects_s9_y1_1)
			{
				object_s9_y1_1->Draw();
			}
		}

		if (stage9YellowKabe4)
		{
			objButtonYellow->Draw();
			objButtonYellow2->Draw();

			for (auto object_s9_y1_2 : objects_s9_y1_2)
			{
				object_s9_y1_2->Draw();
			}
		}

		if (stage9YellowKabe1)
		{
			objButtonBlue->Draw();
			objButtonBlue2->Draw();

			for (auto object_s9_y2_1 : objects_s9_y2_1)
			{
				object_s9_y2_1->Draw();
			}
		}

		if (stage9YellowKabe3)
		{
			objButtonRed1->Draw();
			objButtonRed2->Draw();

			for (auto object_s9_y2_2 : objects_s9_y2_2)
			{
				object_s9_y2_2->Draw();
			}
		}

		objTeleporterIn1->Draw();
		objTeleporterIn2->Draw();
		objTeleporterIn3->Draw();
		objTeleporterIn4->Draw();
		objTeleporterIn5->Draw();

		objTeleporterOut1->Draw();
		objTeleporterOut2->Draw();
		objTeleporterOut3->Draw();
		objTeleporterOut4->Draw();
		objTeleporterOut5->Draw();

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
		TitleLog->Draw();
		Press_A->Draw();
		break;
	case 1:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_1->Draw();
		}
		break;
	case 2:
		StageClearLog->Draw();
		GameOverGameClearSelectBar->Draw();
		if (lastScene != 17)
		{
			NextStageLog->Draw();
		}
		StageClearStageSelectLog->Draw();
		break;
	case 3:
		GameOverLog->Draw();
		GameOverGameClearSelectBar->Draw();
		ReStateLog->Draw();
		GameOverStageSelectLog->Draw();
		break;
	case 4:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	case 5:
		if (!Tutorial && !falling && beginStage)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}

		// チュートリアル
		if (Tutorial)
		{
			TutorialBG->Draw();
			SpeechBubble->Draw();
			switch (TutorialNo)
			{
			case 0:
				break;
			case 1:
				T1Chat1->Draw();
				break;
			case 2:
				T1Chat2->Draw();
				break;
			case 3:
				T1Chat3->Draw();
				break;
			case 4:
				T1Chat4->Draw();
				break;
			case 5:
				T1Chat5->Draw();
				break;
			}
		}

		break;
	case 6:
		if (!Tutorial && !falling && beginStage)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		
		// チュートリアル
		if (Tutorial)
		{
			TutorialBG->Draw();
			SpeechBubble->Draw();
			switch (TutorialNo)
			{
			case 0:
				break;
			case 1:
				T2Chat1->Draw();
				break;
			case 2:
				T2Chat2->Draw();
				break;
			case 3:
				T2Chat3->Draw();
				break;
			case 4:
				T2Chat4->Draw();
				break;
			case 5:
				T2Chat5->Draw();
				break;
			case 6:
				T2Chat6->Draw();
				break;
			}
		}

		break;
	case 7:
		if (!Tutorial && !falling && beginStage)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}

		// チュートリアル
		if (Tutorial)
		{
			TutorialBG->Draw();
			SpeechBubble->Draw();
			switch (TutorialNo)
			{
			case 0:
				break;
			case 1:
				T3Chat1->Draw();
				break;
			case 2:
				T3Chat2->Draw();
				break;
			case 3:
				T3Chat3->Draw();
				break;
			case 4:
				T3Chat4->Draw();
				break;
			case 5:
				T3Chat5->Draw();
				break;
			case 6:
				T3Chat6->Draw();
				break;
			}
		}

		break;
	case 8:
		StageSelectLog->Draw();

		t1Background->Draw();
		t2Background->Draw();
		t3Background->Draw();
		t4Background->Draw();
		t5Background->Draw();
		s1Background->Draw();
		s2Background->Draw();
		s3Background->Draw();
		s4Background->Draw();
		s5Background->Draw();
		s6Background->Draw();
		s7Background->Draw();
		s8Background->Draw();
		s9Background->Draw();

		t1StageSelectFrame->Draw();
		t2StageSelectFrame->Draw();
		t3StageSelectFrame->Draw();
		t4StageSelectFrame->Draw();
		t5StageSelectFrame->Draw();
		s1StageSelectFrame->Draw();
		s2StageSelectFrame->Draw();
		s3StageSelectFrame->Draw();
		s4StageSelectFrame->Draw();
		s5StageSelectFrame->Draw();
		s6StageSelectFrame->Draw();
		s7StageSelectFrame->Draw();
		s8StageSelectFrame->Draw();
		s9StageSelectFrame->Draw();

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

		if (t5FirstPlayFlag == true)
		{
			t5NewLog->Draw();
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

		if (s4FirstPlayFlag == true)
		{
			s4NewLog->Draw();
		}

		if (s5FirstPlayFlag == true)
		{
			s5NewLog->Draw();
		}

		if (s6FirstPlayFlag == true)
		{
			s6NewLog->Draw();
		}

		if (s7FirstPlayFlag == true)
		{
			s7NewLog->Draw();
		}

		if (s8FirstPlayFlag == true)
		{
			s8NewLog->Draw();
		}

		if (s9FirstPlayFlag == true)
		{
			s9NewLog->Draw();
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

		if (t5ClearFlag == true)
		{
			t5ClearLog->Draw();
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

		if (s4ClearFlag == true)
		{
			s4ClearLog->Draw();
		}

		if (s5ClearFlag == true)
		{
			s5ClearLog->Draw();
		}

		if (s6ClearFlag == true)
		{
			s6ClearLog->Draw();
		}

		if (s7ClearFlag == true)
		{
			s7ClearLog->Draw();
		}

		if (s8ClearFlag == true)
		{
			s8ClearLog->Draw();
		}

		if (s9ClearFlag == true)
		{
			s9ClearLog->Draw();
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
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 7:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 8:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 9:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 10:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 11:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 12:
			StageSelectRB->Draw();
			StageSelectLB->Draw();
			break;

		case 13:
			StageSelectLB->Draw();
			break;
		}
		break;
	case 9:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	case 10:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	case 11:
		if (!Tutorial && !falling && beginStage)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}

		// チュートリアル
		if (Tutorial)
		{
			TutorialBG->Draw();
			SpeechBubble->Draw();
			switch (TutorialNo)
			{
			case 0:
				break;
			case 1:
				T4Chat1->Draw();
				break;
			case 2:
				T4Chat2->Draw();
				break;
			case 3:
				T4Chat3->Draw();
				break;
			case 4:
				T4Chat4->Draw();
				break;
			}
		}
		break;
	case 12:
		if (!Tutorial && !falling && beginStage)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}

		// チュートリアル
		if (Tutorial)
		{
			TutorialBG->Draw();
			SpeechBubble->Draw();
			switch (TutorialNo)
			{
			case 0:
				break;
			case 1:
				T5Chat1->Draw();
				break;
			case 2:
				T5Chat2->Draw();
				break;
			case 3:
				T5Chat3->Draw();
				break;
			case 4:
				T5Chat4->Draw();
				break;
			case 5:
				T5Chat5->Draw();
			}
		}
		break;
	case  13:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	case 14:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	case 15:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	case 16:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_1->Draw();
		}
		break;
	case 17:
		if (beginStage && !falling)
		{
			GuideR->Draw();
			Guide_LRB->Draw();
			GuideState->Draw();
			Order_2->Draw();
		}
		break;
	}

	if (pause)
	{
		PauseBG->Draw();
		PauseLog->Draw();
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
			object_t1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t1_1->Update();
		}

		for (auto object_t1_2 : objects_t1_2) {
			XMFLOAT3 objectPosition = object_t1_2->GetPosition();
			object_t1_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
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

	TutorialNo = 0;
	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Tutorial1Move()
{
	for (auto object_t1_1 : objects_t1_1) {
		XMFLOAT3 objectPosition = object_t1_1->GetPosition();
		object_t1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t1_1->Update();
	}

	for (auto object_t1_2 : objects_t1_2) {
		XMFLOAT3 objectPosition = object_t1_2->GetPosition();
		object_t1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t1_2->Update();
	}
}

void GameScene::Tutorial2Reset()
{
	if (!t2Time)
	{
		for (auto object_t2_1 : objects_t2_1) {
			XMFLOAT3 objectPosition = object_t2_1->GetPosition();
			object_t2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t2_1->Update();
		}

		for (auto object_t2_2 : objects_t2_2) {
			XMFLOAT3 objectPosition = object_t2_2->GetPosition();
			object_t2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t2_2->Update();
		}
	}

	objFighter->SetPosition({ -20, 30, -12 });
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

	TutorialNo = 0;
	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Tutorial2Move()
{
	for (auto object_t2_1 : objects_t2_1) {
		XMFLOAT3 objectPosition = object_t2_1->GetPosition();
		object_t2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t2_1->Update();
	}

	for (auto object_t2_2 : objects_t2_2) {
		XMFLOAT3 objectPosition = object_t2_2->GetPosition();
		object_t2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t2_2->Update();
	}
}

void GameScene::Tutorial3Reset()
{
	if (!t3Time)
	{
		for (auto object_t3_1 : objects_t3_1) {
			XMFLOAT3 objectPosition = object_t3_1->GetPosition();
			object_t3_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_1->Update();
		}

		for (auto object_t3_2 : objects_t3_2) {
			XMFLOAT3 objectPosition = object_t3_2->GetPosition();
			object_t3_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_2->Update();
		}

		for (auto object_t3_y1_1 : objects_t3_y1_1) {
			XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
			object_t3_y1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1) {
			XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
			object_t3_y2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_y2_1->Update();
		}

		for (auto object_t3_y1_2 : objects_t3_y1_2) {
			XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
			object_t3_y1_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_y1_2->Update();
		}

		for (auto object_t3_y2_2 : objects_t3_y2_2) {
			XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
			object_t3_y2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_y2_2->Update();
		}

		for (auto object_t3_y1_3 : objects_t3_y1_3) {
			XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
			object_t3_y1_3->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t3_y1_3->Update();
		}

		for (auto object_t3_y2_3 : objects_t3_y2_3) {
			XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
			object_t3_y2_3->SetPosition({ objectPosition.x, 0.0f , objectPosition.z });
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

	doorCount1 = 0;
	doorOpen1 = false;
	doorCount2 = 0;
	doorOpen2 = false;
	doorCount3 = 0;
	doorOpen3 = false;

	SwitchFlag1 = false;
	SwitchFlag2 = false;
	SwitchFlag3 = false;

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

	TutorialNo = 0;
	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Tutorial3Move()
{
	for (auto object_t3_1 : objects_t3_1) {
		XMFLOAT3 objectPosition = object_t3_1->GetPosition();
		object_t3_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t3_1->Update();
	}

	for (auto object_t3_2 : objects_t3_2) {
		XMFLOAT3 objectPosition = object_t3_2->GetPosition();
		object_t3_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t3_2->Update();
	}

	if (tutorial3YellowKabe1)
	{
		for (auto object_t3_y1_1 : objects_t3_y1_1) {
			XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
			object_t3_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1) {
			XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
			object_t3_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y2_1->Update();
		}
	}
	else
	{
		for (auto object_t3_y1_1 : objects_t3_y1_1) {
			XMFLOAT3 objectPosition = object_t3_y1_1->GetPosition();
			object_t3_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y1_1->Update();
		}

		for (auto object_t3_y2_1 : objects_t3_y2_1) {
			XMFLOAT3 objectPosition = object_t3_y2_1->GetPosition();
			object_t3_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y2_1->Update();
		}
	}

	if (tutorial3YellowKabe2)
	{
		for (auto object_t3_y1_2 : objects_t3_y1_2) {
			XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
			object_t3_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y1_2->Update();
		}

		for (auto object_t3_y2_2 : objects_t3_y2_2) {
			XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
			object_t3_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y2_2->Update();
		}
	}
	else
	{
		for (auto object_t3_y1_2 : objects_t3_y1_2) {
			XMFLOAT3 objectPosition = object_t3_y1_2->GetPosition();
			object_t3_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y1_2->Update();
		}

		for (auto object_t3_y2_2 : objects_t3_y2_2) {
			XMFLOAT3 objectPosition = object_t3_y2_2->GetPosition();
			object_t3_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y2_2->Update();
		}
	}

	if (tutorial3YellowKabe3)
	{
		for (auto object_t3_y1_3 : objects_t3_y1_3) {
			XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
			object_t3_y1_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y1_3->Update();
		}

		for (auto object_t3_y2_3 : objects_t3_y2_3) {
			XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
			object_t3_y2_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y2_3->Update();
		}
	}
	else
	{
		for (auto object_t3_y1_3 : objects_t3_y1_3) {
			XMFLOAT3 objectPosition = object_t3_y1_3->GetPosition();
			object_t3_y1_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y1_3->Update();
		}

		for (auto object_t3_y2_3 : objects_t3_y2_3) {
			XMFLOAT3 objectPosition = object_t3_y2_3->GetPosition();
			object_t3_y2_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t3_y2_3->Update();
		}
	}
}

void GameScene::Tutorial4Reset()
{
	if (!t4Time)
	{
		for (auto object_at4_1 : objects_at4_1) {
			XMFLOAT3 objectPosition = object_at4_1->GetPosition();
			object_at4_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_at4_1->Update();
		}

		for (auto object_at4_2 : objects_at4_2) {
			XMFLOAT3 objectPosition = object_at4_2->GetPosition();
			object_at4_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_at4_2->Update();
		}
	}

	objFighter->SetPosition({ -20,30,-12 });
	objClone->SetPosition({ 20,30,-12 });

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objTeleporterIn1->SetPosition({ 8.0f, 0.0f, -3.0f });
	objTeleporterIn1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut1->SetPosition({ 20.0f, 0.0f, 6.0f });
	objTeleporterOut1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	TutorialNo = 0;
	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Tutorial4Move()
{
	for (auto object_at4_1 : objects_at4_1) {
		XMFLOAT3 objectPosition = object_at4_1->GetPosition();
		object_at4_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_at4_1->Update();
	}

	for (auto object_at4_2 : objects_at4_2) {
		XMFLOAT3 objectPosition = object_at4_2->GetPosition();
		object_at4_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_at4_2->Update();
	}
}

void GameScene::Tutorial5Reset()
{
	if (!t5Time)
	{
		for (auto object_t5_1 : objects_t5_1) {
			XMFLOAT3 objectPosition = object_t5_1->GetPosition();
			object_t5_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t5_1->Update();
		}

		for (auto object_t5_2 : objects_t5_2) {
			XMFLOAT3 objectPosition = object_t5_2->GetPosition();
			object_t5_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t5_2->Update();
		}

		for (auto object_t5_s2 : objects_t5_s2) {
			XMFLOAT3 objectPosition = object_t5_s2->GetPosition();
			object_t5_s2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t5_s2->Update();
		}
	}

	objFighter->SetPosition({ -20,30,-12 });
	objClone->SetPosition({ 20,30,-12 });

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	SwitchFlag1 = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	objButtonFloor->SetPosition({ -14.0f, 0.0f, 15.0f });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	tutorial5Switch = false;

	doorOpen1 = false;
	doorCount1 = 0;
	TutorialNo = 0;
	cameraMove = 1;
	cameraChange = false;

	beginStage = false;
}

void GameScene::Tutorial5Move()
{
	for (auto object_t5_1 : objects_t5_1) {
		XMFLOAT3 objectPosition = object_t5_1->GetPosition();
		object_t5_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t5_1->Update();
	}

	for (auto object_t5_2 : objects_t5_2) {
		XMFLOAT3 objectPosition = object_t5_2->GetPosition();
		object_t5_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t5_2->Update();
	}

	for (auto object_t5_s2 : objects_t5_s2) {
		XMFLOAT3 objectPosition = object_t5_s2->GetPosition();
		object_t5_s2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t5_s2->Update();
	}
}

void GameScene::Stage1Reset()
{
	if (!firstTime)
	{
		for (auto object_s2_1 : objects_s2_1) {
			XMFLOAT3 objectPosition = object_s2_1->GetPosition();
			object_s2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s2_1->Update();
		}

		for (auto object_s2_2 : objects_s2_2) {
			XMFLOAT3 objectPosition = object_s2_2->GetPosition();
			object_s2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
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

	doorOpen1 = false;
	doorOpen2 = false;
	doorCount1 = 0;
	doorCount2 = 0;
	cameraMove = 1;
	cameraChange = false;

	stage2YellowKabe = true;
	stage2Switch = false;

	SwitchFlag1 = false;
	SwitchFlag2 = false;

	beginStage = false;
	
}

void GameScene::Stage1Move()
{
	for (auto object_s2_1 : objects_s2_1) {
		XMFLOAT3 objectPosition = object_s2_1->GetPosition();
		object_s2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s2_1->Update();
	}

	for (auto object_s2_2 : objects_s2_2) {
		XMFLOAT3 objectPosition = object_s2_2->GetPosition();
		object_s2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s2_2->Update();
	}

	if (!stage2YellowKabe)
	{
		for (auto object_s2_y : objects_s2_y) {
			XMFLOAT3 objectPosition = object_s2_y->GetPosition();
			object_s2_y->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_y->Update();
		}

		for (auto object_s2_y2 : objects_s2_y2) {
			XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
			object_s2_y2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_y2->Update();
		}
	}
	else
	{
		for (auto object_s2_y : objects_s2_y) {
			XMFLOAT3 objectPosition = object_s2_y->GetPosition();
			object_s2_y->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_y->Update();
		}

		for (auto object_s2_y2 : objects_s2_y2) {
			XMFLOAT3 objectPosition = object_s2_y2->GetPosition();
			object_s2_y2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_y2->Update();
		}
	}

	if (stage2Switch)
	{
		for (auto object_s2_s : objects_s2_s) {
			XMFLOAT3 objectPosition = object_s2_s->GetPosition();
			object_s2_s->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_s->Update();
		}

		for (auto object_s2_s2 : objects_s2_s2) {
			XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
			object_s2_s2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_s2->Update();
		}
	}
	else
	{
		for (auto object_s2_s : objects_s2_s) {
			XMFLOAT3 objectPosition = object_s2_s->GetPosition();
			object_s2_s->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_s->Update();
		}

		for (auto object_s2_s2 : objects_s2_s2) {
			XMFLOAT3 objectPosition = object_s2_s2->GetPosition();
			object_s2_s2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s2_s2->Update();
		}
	}
}

void GameScene::Stage2Reset()
{
	if (!secondTime)
	{
		for (auto object_t4_1 : objects_t4_1) {
			XMFLOAT3 objectPosition = object_t4_1->GetPosition();
			object_t4_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t4_1->Update();
		}

		for (auto object_t4_2 : objects_t4_2) {
			XMFLOAT3 objectPosition = object_t4_2->GetPosition();
			object_t4_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t4_2->Update();
		}

		for (auto object_t4_y : objects_t4_y) {
			XMFLOAT3 objectPosition = object_t4_y->GetPosition();
			object_t4_y->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2) {
			XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
			object_t4_y2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
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

	doorOpen1 = false;
	doorCount1 = 0;
	cameraMove = 1;
	cameraChange = false;

	SwitchFlag1 = false;

	beginStage = false;
}

void GameScene::Stage2Move()
{
	for (auto object_t4_1 : objects_t4_1) {
		XMFLOAT3 objectPosition = object_t4_1->GetPosition();
		object_t4_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t4_1->Update();
	}

	for (auto object_t4_2 : objects_t4_2) {
		XMFLOAT3 objectPosition = object_t4_2->GetPosition();
		object_t4_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_t4_2->Update();
	}

	if (!tutorial4YellowKabe)
	{
		for (auto object_t4_y : objects_t4_y) {
			XMFLOAT3 objectPosition = object_t4_y->GetPosition();
			object_t4_y->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2) {
			XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
			object_t4_y2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t4_y2->Update();
		}
	}
	else
	{
		for (auto object_t4_y : objects_t4_y) {
			XMFLOAT3 objectPosition = object_t4_y->GetPosition();
			object_t4_y->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t4_y->Update();
		}

		for (auto object_t4_y2 : objects_t4_y2) {
			XMFLOAT3 objectPosition = object_t4_y2->GetPosition();
			object_t4_y2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_t4_y2->Update();
		}
	}
}

void GameScene::Stage3Reset()
{
	if (!thirdTime)
	{
		for (auto object : objects)
		{
			XMFLOAT3 objectPosition = object->GetPosition();
			object->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object->Update();
		}
		for (auto object_2 : objects_2)
		{
			XMFLOAT3 objectPosition = object_2->GetPosition();
			object_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
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

	SwitchFlag1 = false;
	SwitchFlag2 = false;

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

void GameScene::Stage3Move()
{
	for (auto object : objects)
	{
		XMFLOAT3 objectPosition = object->GetPosition();
		object->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object->Update();
	}
	for (auto object_2 : objects_2)
	{
		XMFLOAT3 objectPosition = object_2->GetPosition();
		object_2->SetPosition({ objectPosition.x, 100.0f, objectPosition.z });
		object_2->Update();
	}
}

void GameScene::Stage4Reset()
{
	if (!fourthTime)
	{
		for (auto object_s4_1 : objects_s4_1) {
			XMFLOAT3 objectPosition = object_s4_1->GetPosition();
			object_s4_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s4_1->Update();
		}

		for (auto object_s4_2 : objects_s4_2) {
			XMFLOAT3 objectPosition = object_s4_2->GetPosition();
			object_s4_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s4_2->Update();
		}

		for (auto object_s4_y : objects_s4_y) {
			XMFLOAT3 objectPosition = object_s4_y->GetPosition();
			object_s4_y->SetPosition({ objectPosition.x, -15.0f, objectPosition.z });
			object_s4_y->Update();
		}

		for (auto object_s4_y2 : objects_s4_y2) {
			XMFLOAT3 objectPosition = object_s4_y2->GetPosition();
			object_s4_y2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s4_y2->Update();
		}
	}

	objFighter->SetPosition({ -20,30,-9 });
	objClone->SetPosition({ 20,30,-9 });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	cameraMove = 1;
	cameraChange = false;

	doorOpen1 = false;
	doorOpen2 = false;
	doorCount1 = 0;
	doorCount2 = 0;
	stage4YellowKabe = true;
	stage4Switch = false;

	SwitchFlag1 = false;
	SwitchFlag2 = false;

	objButtonBlue->SetPosition({ -8, 0, 9 });
	objButtonFloor->SetPosition({ 14, 0, -6 });

	beginStage = false;
}

void GameScene::Stage4Move()
{
	for (auto object_s4_1 : objects_s4_1) {
		XMFLOAT3 objectPosition = object_s4_1->GetPosition();
		object_s4_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s4_1->Update();
	}

	for (auto object_s4_2 : objects_s4_2) {
		XMFLOAT3 objectPosition = object_s4_2->GetPosition();
		object_s4_2->SetPosition({ objectPosition.x, 100.0f, objectPosition.z });
		object_s4_2->Update();
	}

	for (auto object_s4_y : objects_s4_y) {
		XMFLOAT3 objectPosition = object_s4_y->GetPosition();
		object_s4_y->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s4_y->Update();
	}

	for (auto object_s4_y2 : objects_s4_y2) {
		XMFLOAT3 objectPosition = object_s4_y2->GetPosition();
		object_s4_y2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s4_y2->Update();
	}
}

void GameScene::Stage5Reset()
{
	if (!fifthTime)
	{
		for (auto object_s5_1 : objects_s5_1) {
			XMFLOAT3 objectPosition = object_s5_1->GetPosition();
			object_s5_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_1->Update();
		}

		for (auto object_s5_2 : objects_s5_2) {
			XMFLOAT3 objectPosition = object_s5_2->GetPosition();
			object_s5_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_2->Update();
		}

		for (auto object_s5_y1_1 : objects_s5_y1_1) {
			XMFLOAT3 objectPosition = object_s5_y1_1->GetPosition();
			object_s5_y1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_y1_1->Update();
		}

		for (auto object_s5_y2_1 : objects_s5_y2_1) {
			XMFLOAT3 objectPosition = object_s5_y2_1->GetPosition();
			object_s5_y2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_y2_1->Update();
		}

		for (auto object_s5_y1_2 : objects_s5_y1_2) {
			XMFLOAT3 objectPosition = object_s5_y1_2->GetPosition();
			object_s5_y1_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_y1_2->Update();
		}

		for (auto object_s5_y2_2 : objects_s5_y2_2) {
			XMFLOAT3 objectPosition = object_s5_y2_2->GetPosition();
			object_s5_y2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_y2_2->Update();
		}

		for (auto object_s5_y2_3 : objects_s5_y2_3) {
			XMFLOAT3 objectPosition = object_s5_y2_3->GetPosition();
			object_s5_y2_3->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s5_y2_3->Update();
		}
	}

	objFighter->SetPosition({ -20, 30, -12 });
	objClone->SetPosition({ 8, 30, 12 });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	cameraMove = 1;
	cameraChange = false;

	doorOpen1 = false;
	doorOpen2 = false;
	doorOpen3 = false;
	doorOpen4 = false;
	doorOpen5 = false;
	doorCount1 = 0;
	doorCount2 = 0;
	doorCount3 = 0;
	doorCount4 = 0;
	doorCount5 = 0;
	stage5YellowKabe1 = true;
	stage5YellowKabe2 = true;
	stage5YellowKabe3 = true;
	stage5YellowKabe4 = true;
	stage5Switch = false;

	objButtonRed1->SetPosition({ 11, 0, -3 });
	objButtonGreen1->SetPosition({ -20, 0, 12 });
	objButtonBlue->SetPosition({ -8, 0, -12 });
	objButtonYellow->SetPosition({ -17, 0, 3 });
	objButtonFloor->SetPosition({ 8, 0, -12 });

	SwitchFlag1 = false;
	SwitchFlag2 = false;
	SwitchFlag3 = false;
	SwitchFlag4 = false;
	SwitchFlag5 = false;

	beginStage = false;
}

void GameScene::Stage5Move()
{
	for (auto object_s5_1 : objects_s5_1) {
		XMFLOAT3 objectPosition = object_s5_1->GetPosition();
		object_s5_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_1->Update();
	}

	for (auto object_s5_2 : objects_s5_2) {
		XMFLOAT3 objectPosition = object_s5_2->GetPosition();
		object_s5_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_2->Update();
	}

	for (auto object_s5_y1_1 : objects_s5_y1_1) {
		XMFLOAT3 objectPosition = object_s5_y1_1->GetPosition();
		object_s5_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_y1_1->Update();
	}

	for (auto object_s5_y2_1 : objects_s5_y2_1) {
		XMFLOAT3 objectPosition = object_s5_y2_1->GetPosition();
		object_s5_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_y2_1->Update();
	}

	for (auto object_s5_y1_2 : objects_s5_y1_2) {
		XMFLOAT3 objectPosition = object_s5_y1_2->GetPosition();
		object_s5_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_y1_2->Update();
	}

	for (auto object_s5_y2_2 : objects_s5_y2_2) {
		XMFLOAT3 objectPosition = object_s5_y2_2->GetPosition();
		object_s5_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_y2_2->Update();
	}

	for (auto object_s5_y2_3 : objects_s5_y2_3) {
		XMFLOAT3 objectPosition = object_s5_y2_3->GetPosition();
		object_s5_y2_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s5_y2_3->Update();
	}
}

void GameScene::Stage6Reset()
{
	if (!sixthTime)
	{
		for (auto object_as6_1 : objects_as6_1) {
			XMFLOAT3 objectPosition = object_as6_1->GetPosition();
			object_as6_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_as6_1->Update();
		}

		for (auto object_as6_2 : objects_as6_2) {
			XMFLOAT3 objectPosition = object_as6_2->GetPosition();
			object_as6_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_as6_2->Update();
		}

		for (auto object_as6_y1_1 : objects_as6_y1_1) {
			XMFLOAT3 objectPosition = object_as6_y1_1->GetPosition();
			object_as6_y1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_as6_y1_1->Update();
		}

		for (auto object_as6_y2_1 : objects_as6_y2_1) {
			XMFLOAT3 objectPosition = object_as6_y2_1->GetPosition();
			object_as6_y2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_as6_y2_1->Update();
		}

		for (auto object_as6_y1_2 : objects_as6_y1_2) {
			XMFLOAT3 objectPosition = object_as6_y1_2->GetPosition();
			object_as6_y1_2->SetPosition({ objectPosition.x, -15.0f, objectPosition.z });
			object_as6_y1_2->Update();
		}
	}

	objFighter->SetPosition({ -14, 30, 0 });
	objClone->SetPosition({ 14, 30, 0 });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	cameraMove = 1;
	cameraChange = false;

	doorOpen1 = false;
	doorOpen2 = false;
	doorOpen3 = false;
	doorCount1 = 0;
	doorCount2 = 0;
	doorCount3 = 0;
	newstage6YellowKabe1 = true;
	newstage6YellowKabe2 = true;
	newStage6switch = false;

	objButtonGreen1->SetPosition({ -8, 0, -12 });
	objButtonBlue->SetPosition({ 11, 0, 12 });
	objButtonFloor->SetPosition({ 20, 0, -12 });
	objButtonBomb1->SetPosition({ -11, 0, 12 });

	playerBullet.x = -8.0f;
	playerBullet.z = 12.0f;
	objTempBullet->SetPosition(playerBullet);

	SwitchFlag1 = false;
	SwitchFlag2 = false;

	beginStage = false;
}

void GameScene::Stage6Move()
{
	for (auto object_as6_1 : objects_as6_1) {
		XMFLOAT3 objectPosition = object_as6_1->GetPosition();
		object_as6_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_as6_1->Update();
	}

	for (auto object_as6_2 : objects_as6_2) {
		XMFLOAT3 objectPosition = object_as6_2->GetPosition();
		object_as6_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_as6_2->Update();
	}

	for (auto object_as6_y1_1 : objects_as6_y1_1) {
		XMFLOAT3 objectPosition = object_as6_y1_1->GetPosition();
		object_as6_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_as6_y1_1->Update();
	}

	for (auto object_as6_y2_1 : objects_as6_y2_1) {
		XMFLOAT3 objectPosition = object_as6_y2_1->GetPosition();
		object_as6_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_as6_y2_1->Update();
	}

	for (auto object_as6_y1_2 : objects_as6_y1_2) {
		XMFLOAT3 objectPosition = object_as6_y1_2->GetPosition();
		object_as6_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_as6_y1_2->Update();
	}
}

void GameScene::Stage7Reset()
{
	if (!seventhTime)
	{
		for (auto object_s3_1 : objects_s3_1) {
			XMFLOAT3 objectPosition = object_s3_1->GetPosition();
			object_s3_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s3_1->Update();
		}

		for (auto object_s3_2 : objects_s3_2) {
			XMFLOAT3 objectPosition = object_s3_2->GetPosition();
			object_s3_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s3_2->Update();
		}

		for (auto object_s3_y1_1 : objects_s3_y1_1) {
			XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
			object_s3_y1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s3_y1_1->Update();
		}

		for (auto object_s3_y2_1 : objects_s3_y2_1) {
			XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
			object_s3_y2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s3_y2_1->Update();
		}

		for (auto object_s3_y1_2 : objects_s3_y1_2) {
			XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
			object_s3_y1_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s3_y1_2->Update();
		}

		for (auto object_s3_y2_2 : objects_s3_y2_2) {
			XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
			object_s3_y2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
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

	doorOpen1 = false;
	doorOpen2 = false;
	doorOpen3 = false;
	doorOpen4 = false;
	doorCount1 = 0;
	doorCount2 = 0;
	doorCount3 = 0;
	doorCount4 = 0;
	cameraMove = 1;
	cameraChange = false;

	SwitchFlag1 = false;
	SwitchFlag2 = false;
	SwitchFlag3 = false;

	beginStage = false;
}

void GameScene::Stage7Move()
{
	for (auto object_s3_1 : objects_s3_1) {
		XMFLOAT3 objectPosition = object_s3_1->GetPosition();
		object_s3_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s3_1->Update();
	}

	for (auto object_s3_2 : objects_s3_2) {
		XMFLOAT3 objectPosition = object_s3_2->GetPosition();
		object_s3_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s3_2->Update();
	}

	if (!stage3YellowKabe1)
	{
		for (auto object_s3_y1_1 : objects_s3_y1_1) {
			XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
			object_s3_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y1_1->Update();
		}
	}
	else
	{
		for (auto object_s3_y1_1 : objects_s3_y1_1) {
			XMFLOAT3 objectPosition = object_s3_y1_1->GetPosition();
			object_s3_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y1_1->Update();
		}
	}

	if (!stage3YellowKabe2)
	{
		for (auto object_s3_y2_1 : objects_s3_y2_1) {
			XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
			object_s3_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y2_1->Update();
		}
	}
	else
	{
		for (auto object_s3_y2_1 : objects_s3_y2_1) {
			XMFLOAT3 objectPosition = object_s3_y2_1->GetPosition();
			object_s3_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y2_1->Update();
		}
	}

	if (!stage3YellowKabe3)
	{
		for (auto object_s3_y2_2 : objects_s3_y2_2) {
			XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
			object_s3_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y2_2->Update();
		}
	}
	else
	{
		for (auto object_s3_y2_2 : objects_s3_y2_2) {
			XMFLOAT3 objectPosition = object_s3_y2_2->GetPosition();
			object_s3_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y2_2->Update();
		}
	}

	if (!stage3YellowKabe4)
	{
		for (auto object_s3_y1_2 : objects_s3_y1_2) {
			XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
			object_s3_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y1_2->Update();
		}
	}
	else
	{
		for (auto object_s3_y1_2 : objects_s3_y1_2) {
			XMFLOAT3 objectPosition = object_s3_y1_2->GetPosition();
			object_s3_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
			object_s3_y1_2->Update();
		}
	}
}

void GameScene::Stage8Reset()
{
	if (!eigthTime)
	{
		for (auto object_s6_1 : objects_s6_1) {
			XMFLOAT3 objectPosition = object_s6_1->GetPosition();
			object_s6_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_1->Update();
		}

		for (auto object_s6_2 : objects_s6_2) {
			XMFLOAT3 objectPosition = object_s6_2->GetPosition();
			object_s6_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_2->Update();
		}

		for (auto object_s6_y1_1 : objects_s6_y1_1) {
			XMFLOAT3 objectPosition = object_s6_y1_1->GetPosition();
			object_s6_y1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_y1_1->Update();
		}

		for (auto object_s6_y2_1 : objects_s6_y2_1) {
			XMFLOAT3 objectPosition = object_s6_y2_1->GetPosition();
			object_s6_y2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_y2_1->Update();
		}

		for (auto object_s6_y1_2 : objects_s6_y1_2) {
			XMFLOAT3 objectPosition = object_s6_y1_2->GetPosition();
			object_s6_y1_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_y1_2->Update();
		}

		for (auto object_s6_y2_2 : objects_s6_y2_2) {
			XMFLOAT3 objectPosition = object_s6_y2_2->GetPosition();
			object_s6_y2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_y2_2->Update();
		}

		for (auto object_s6_y1_3 : objects_s6_y1_3) {
			XMFLOAT3 objectPosition = object_s6_y1_3->GetPosition();
			object_s6_y1_3->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_y1_3->Update();
		}

		for (auto object_s6_y2_3 : objects_s6_y2_3) {
			XMFLOAT3 objectPosition = object_s6_y2_3->GetPosition();
			object_s6_y2_3->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s6_y2_3->Update();
		}

		for (auto object_s6_y2_4 : objects_s6_y2_4) {
			XMFLOAT3 objectPosition = object_s6_y2_4->GetPosition();
			object_s6_y2_4->SetPosition({ objectPosition.x, -15.0f, objectPosition.z });
			object_s6_y2_4->Update();
		}
	}

	objFighter->SetPosition({ -20,30,-9 });
	objClone->SetPosition({ 20,30,-9 });

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

	objTeleporterIn1->SetPosition({ -14.0f, 0.0f, -12.0f }); // 8, -3
	objTeleporterIn1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn2->SetPosition({ -17.0f, 0.0f, 12.0f }); // 5, -12
	objTeleporterIn2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn2->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn3->SetPosition({ 14.0f, 0.0f, -6.0f }); // -11, -9
	objTeleporterIn3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut1->SetPosition({ -20.0f, 0.0f, -3.0f }); // 20, 6
	objTeleporterOut1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut2->SetPosition({ -11.0f, 0.0f, 6.0f }); // -17, 6
	objTeleporterOut2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut2->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut3->SetPosition({ 20.0f, 0.0f, -3.0f }); // 11, 0
	objTeleporterOut3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objButtonRed1->SetPosition({ -17, 0, -6 });
	objButtonRed2->SetPosition({ 17, 0, -12 });
	objButtonGreen1->SetPosition({ -14, 0, -3 });
	objButtonGreen2->SetPosition({ 14, 0, 3 });
	objButtonBlue->SetPosition({ 8, 0, -12 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	stage6YellowKabe1 = true;
	stage6YellowKabe2 = true;
	stage6YellowKabe3 = true;

	doorOpen1 = false;
	doorOpen2 = false;
	doorOpen3 = false;

	doorCount1 = 0;
	doorCount2 = 0;
	doorCount3 = 0;

	cameraMove = 1;
	cameraChange = false;

	SwitchFlag1 = false;
	SwitchFlag2 = false;
	SwitchFlag3 = false;
	SwitchFlag4 = false;
	SwitchFlag5 = false;

	beginStage = false;
}

void GameScene::Stage8Move()
{
	for (auto object_s6_1 : objects_s6_1) {
		XMFLOAT3 objectPosition = object_s6_1->GetPosition();
		object_s6_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_1->Update();
	}

	for (auto object_s6_2 : objects_s6_2) {
		XMFLOAT3 objectPosition = object_s6_2->GetPosition();
		object_s6_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_2->Update();
	}

	for (auto object_s6_y1_1 : objects_s6_y1_1) {
		XMFLOAT3 objectPosition = object_s6_y1_1->GetPosition();
		object_s6_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y1_1->Update();
	}

	for (auto object_s6_y2_1 : objects_s6_y2_1) {
		XMFLOAT3 objectPosition = object_s6_y2_1->GetPosition();
		object_s6_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y2_1->Update();
	}

	for (auto object_s6_y1_2 : objects_s6_y1_2) {
		XMFLOAT3 objectPosition = object_s6_y1_2->GetPosition();
		object_s6_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y1_2->Update();
	}

	for (auto object_s6_y2_2 : objects_s6_y2_2) {
		XMFLOAT3 objectPosition = object_s6_y2_2->GetPosition();
		object_s6_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y2_2->Update();
	}

	for (auto object_s6_y1_3 : objects_s6_y1_3) {
		XMFLOAT3 objectPosition = object_s6_y1_3->GetPosition();
		object_s6_y1_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y1_3->Update();
	}

	for (auto object_s6_y2_3 : objects_s6_y2_3) {
		XMFLOAT3 objectPosition = object_s6_y2_3->GetPosition();
		object_s6_y2_3->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y2_3->Update();
	}

	for (auto object_s6_y2_4 : objects_s6_y2_4) {
		XMFLOAT3 objectPosition = object_s6_y2_4->GetPosition();
		object_s6_y2_4->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s6_y2_4->Update();
	}
}

void GameScene::Stage9Reset()
{
	if (!ninthTime)
	{
		for (auto object_s9_1 : objects_s9_1) {
			XMFLOAT3 objectPosition = object_s9_1->GetPosition();
			object_s9_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s9_1->Update();
		}

		for (auto object_s9_2 : objects_s9_2) {
			XMFLOAT3 objectPosition = object_s9_2->GetPosition();
			object_s9_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s9_2->Update();
		}

		for (auto object_s9_y1_1 : objects_s9_y1_1) {
			XMFLOAT3 objectPosition = object_s9_y1_1->GetPosition();
			object_s9_y1_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s9_y1_1->Update();
		}

		for (auto object_s9_y2_1 : objects_s9_y2_1) {
			XMFLOAT3 objectPosition = object_s9_y2_1->GetPosition();
			object_s9_y2_1->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s9_y2_1->Update();
		}

		for (auto object_s9_y1_2 : objects_s9_y1_2) {
			XMFLOAT3 objectPosition = object_s9_y1_2->GetPosition();
			object_s9_y1_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s9_y1_2->Update();
		}

		for (auto object_s9_y2_2 : objects_s9_y2_2) {
			XMFLOAT3 objectPosition = object_s9_y2_2->GetPosition();
			object_s9_y2_2->SetPosition({ objectPosition.x, 0.0f, objectPosition.z });
			object_s9_y2_2->Update();
		}
	}

	objFighter->SetPosition({ -14, 30, -9 });
	objClone->SetPosition({ 14, 30, -9 });

	objTeleporterIn1->SetPosition({ -8.0f, 0.0f, -12.0f }); // 8, -3
	objTeleporterIn1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn2->SetPosition({ 11.0f, 0.0f, 3.0f }); // 5, -12
	objTeleporterIn2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn2->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn3->SetPosition({ 17.0f, 0.0f, 3.0f }); // -11, -9
	objTeleporterIn3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn4->SetPosition({ -17.0f, 0.0f, 0.0f }); // 14, 12
	objTeleporterIn4->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn4->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterIn5->SetPosition({ -11.0f, 0.0f, 0.0f }); // 8, -3
	objTeleporterIn5->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterIn5->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut1->SetPosition({ 20.0f, 0.0f, -12.0f }); // 20, 6
	objTeleporterOut1->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut1->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut2->SetPosition({ -20.0f, 0.0f, 3.0f }); // -17, 6
	objTeleporterOut2->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut2->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut3->SetPosition({ -8.0f, 0.0f, 3.0f }); // 11, 0
	objTeleporterOut3->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut3->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut4->SetPosition({ 11.0f, 0.0f, 12.0f }); // 8, -12
	objTeleporterOut4->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut4->SetRotation({ 0.0f, 0.0f, 0.0f });

	objTeleporterOut5->SetPosition({ 17.0f, 0.0f, 12.0f }); // 20, 6
	objTeleporterOut5->SetScale({ WarpSize, WarpSize, WarpSize });
	objTeleporterOut5->SetRotation({ 0.0f, 0.0f, 0.0f });

	falling = false;

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;

	stage9YellowKabe1 = true;
	stage9YellowKabe2 = true;
	stage9YellowKabe3 = true;
	stage9YellowKabe4 = true;

	camera->SetTarget({ 0, 1, 0 });
	camera->SetEye({ 0, 20, -30 });
	camera->MoveEyeVector({ +100.0f, +105.0f, +100.0f });

	objFighter->SetRotation({ 0,0,0 });
	objClone->SetRotation({ 0,0,0 });

	playerRotationTemp = { 0,0,0 };
	cloneRotationTemp = { 0,0,0 };

	objButtonRed1->SetPosition({ -17, 0, 3 });
	objButtonRed2->SetPosition({ -11, 0, 3 });
	objButtonGreen1->SetPosition({ 17, 0, 0 });
	objButtonGreen2->SetPosition({ 11, 0, 0 });
	objButtonBlue->SetPosition({ 20, 0, -9 });
	objButtonBlue2->SetPosition({ 8, 0, -9 });
	objButtonYellow->SetPosition({ 8, 0, 12 });
	objButtonYellow2->SetPosition({ 20, 0, 12 });

	doorOpen1 = false;
	doorOpen2 = false;
	doorOpen3 = false;
	doorOpen4 = false;
	cameraMove = 1;
	cameraChange = false;

	SwitchFlag1 = false;
	SwitchFlag2 = false;
	SwitchFlag3 = false;
	SwitchFlag4 = false;

	beginStage = false;
}

void GameScene::Stage9Move()
{
	for (auto object_s9_1 : objects_s9_1) {
		XMFLOAT3 objectPosition = object_s9_1->GetPosition();
		object_s9_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s9_1->Update();
	}

	for (auto object_s9_2 : objects_s9_2) {
		XMFLOAT3 objectPosition = object_s9_2->GetPosition();
		object_s9_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s9_2->Update();
	}

	for (auto object_s9_y1_1 : objects_s9_y1_1) {
		XMFLOAT3 objectPosition = object_s9_y1_1->GetPosition();
		object_s9_y1_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s9_y1_1->Update();
	}

	for (auto object_s9_y2_1 : objects_s9_y2_1) {
		XMFLOAT3 objectPosition = object_s9_y2_1->GetPosition();
		object_s9_y2_1->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s9_y2_1->Update();
	}

	for (auto object_s9_y1_2 : objects_s9_y1_2) {
		XMFLOAT3 objectPosition = object_s9_y1_2->GetPosition();
		object_s9_y1_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s9_y1_2->Update();
	}

	for (auto object_s9_y2_2 : objects_s9_y2_2) {
		XMFLOAT3 objectPosition = object_s9_y2_2->GetPosition();
		object_s9_y2_2->SetPosition({ objectPosition.x, -100.0f, objectPosition.z });
		object_s9_y2_2->Update();
	}
}

void GameScene::GameClearReset()
{
	if (lastScene != 17)
	{
		GameOverGameClearSelectBar->SetPosition({ -250.0f,350.0f });
	}
	else
	{
		GameOverGameClearSelectBar->SetPosition({ -250.0f,500.0f });
	}
	if (lastScene != 17)
	{
		menuSelection = 0;
	}
	else
	{
		menuSelection = 1;
	}
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