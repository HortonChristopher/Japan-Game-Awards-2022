#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "CollisionPrimitive.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "CinematicCamera.h"

#include <vector>

class CollisionManager;
class Player;
class Enemy;
class TouchableObject;
class TitleScene;
class GameOver;
class GameClear;

// ゲームシーン Game scene
class GameScene
{
private: // エイリアス alias
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数 Static member variables
	static const int debugTextTexNumber = 0;

public:

public: // メンバ関数 Member function

	// コンストクラタ Constructor
	GameScene();

	// デストラクタ Destructor
	~GameScene();

	// 初期化 Initialization
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	// 毎フレーム処理 Every frame processing
	void Update();

	// 描画 drawing
	void Draw();

	// カメラ移動 Camera movement
	void MoveCamera();

	// パーティクル生成 Particle generation
	void CreateParticles();

	int intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH);

	void SceneSelectionReset();
	void Tutorial1Reset();
	void Tutorial1Move();
	void Tutorial2Reset();
	void Tutorial2Move();
	void Tutorial3Reset();
	void Tutorial3Move();
	void newTutorial4Reset();
	void newTutorial4Move();
	void Tutorial5Reset();
	void Tutorial5Move();
	void Tutorial4Reset();
	void Tutorial4Move();
	void Stage1Reset();
	void Stage1Move();
	void Stage2Reset();
	void Stage2Move();
	void Stage5Reset();
	void Stage5Move();
	void Stage6Reset();
	void Stage6Move();

	void GameClearReset();
	void GameOverReset();

	void CinematicCamera();

private: // メンバ変数 Member variables
	bool lastIntersect = false;
	bool lastIntersectE = false;
	bool lastYellowIntersct1 = false;

	bool enemyAlive = true;
	bool playerAlive = true;

	bool playerBulletF = false;
	bool enemyBulletF = false;

	bool tutorial3YellowKabe1 = true;
	bool tutorial3YellowKabe2 = true;
	bool tutorial3YellowKabe3 = true;
	bool tutorial4YellowKabe = true;
	bool tutorial5Switch = false;
	bool stage2YellowKabe = true;
	bool stage2Switch = false;
	bool stage3YellowKabe1 = true;
	bool stage3YellowKabe2 = true;
	bool stage3YellowKabe3 = true;
	bool stage3YellowKabe4 = true;
	bool stage6YellowKabe1 = true;
	bool stage6YellowKabe2 = true;
	bool stage6YellowKabe3 = true;

	bool stageMoveRight = false;
	bool stageMoveLeft = false;

	//初めプレイですか？
	bool t1Time = true;
	bool t2Time = true;
	bool t3Time = true;
	bool at4Time = true;
	bool t5Time = true;
	bool t4Time = true;
	bool firstTime = true;
	bool secondTime = true;
	bool thirdTime = true;
	bool sixthTime = true;

	bool temp = true;

	//音楽用
	bool once = true;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// ゲームシーン用 For game scenes
	Camera* camera = nullptr;

	bool falling = false;
	bool marker = true;
	bool pause = false;
	bool pausePosition = false;

	bool doorOpen1 = false;
	bool doorOpen2 = false;
	bool doorOpen3 = false;
	bool doorOpen4 = false;
	bool doorOpen5 = false;

	int doorCount1 = 0;
	int doorCount2 = 0;
	int doorCount3 = 0;
	int doorCount4 = 0;
	int doorCount5 = 0;

	// ステージセレクト
	bool t1FirstPlayFlag = true;
	bool t2FirstPlayFlag = true;
	bool t3FirstPlayFlag = true;
	bool at4FirstPlayFlag = true;
	bool t5FirstPlayFlag = true;
	bool t4FirstPlayFlag = true;
	bool s1FirstPlayFlag = true;
	bool s2FirstPlayFlag = true;
	bool s3FirstPlayFlag = true;
	bool s4FirstPlayFlag = true;
	bool s6FirstPlayFlag = true;

	bool t1ClearFlag = false;
	bool t2ClearFlag = false;
	bool t3ClearFlag = false;
	bool at4ClearFlag = false;
	bool t5ClearFlag = false;
	bool t4ClearFlag = false;
	bool s1ClearFlag = false;
	bool s2ClearFlag = false;
	bool s3ClearFlag = false;
	bool s4ClearFlag = false;
	bool s6ClearFlag = false;

	//スイッチSEフラグ
	bool SwitchFlag1 = false;
	bool SwitchFlag2 = false;
	bool SwitchFlag3 = false;

#pragma region Sprite
	Sprite* spriteBG = nullptr;
	Sprite* GuideR = nullptr;
	Sprite* Guide_LRB = nullptr;
	Sprite* Order_1 = nullptr;
	Sprite* Order_2 = nullptr;
	Sprite* Press_A = nullptr;
	Sprite* GameOverLog = nullptr;
	Sprite* ReStateLog = nullptr;
	Sprite* GameOverStageSelectLog = nullptr;
	Sprite* StageClearStageSelectLog = nullptr;
	Sprite* StageClearLog = nullptr;
	Sprite* TitleLog = nullptr;
	Sprite* NextStageLog = nullptr;
	Sprite* StageSelectRB = nullptr;
	Sprite* StageSelectLB = nullptr;
	Sprite* StageSelectLog = nullptr;
	Sprite* GuideA = nullptr;
	Sprite* GuideB = nullptr;
	Sprite* GameOverGameClearSelectBar = nullptr;

	// チュートリアル
	Sprite* TutorialBG = nullptr;
	Sprite* SpeechBubble = nullptr;

	// チュートリアル1
	bool Tutorial = false;
	int TutorialNo = 0;
	Sprite* T1Chat1 = nullptr;
	Sprite* T1Chat2 = nullptr;
	Sprite* T1Chat3 = nullptr;
	Sprite* T1Chat4 = nullptr;
	Sprite* T1Chat5 = nullptr;

	// チュートリアル2
	Sprite* T2Chat1 = nullptr;
	Sprite* T2Chat2 = nullptr;
	Sprite* T2Chat3 = nullptr;
	Sprite* T2Chat4 = nullptr;
	Sprite* T2Chat5 = nullptr;
	Sprite* T2Chat6 = nullptr;

	// チュートリアル3
	Sprite* T3Chat1 = nullptr;
	Sprite* T3Chat2 = nullptr;
	Sprite* T3Chat3 = nullptr;
	Sprite* T3Chat4 = nullptr;
	Sprite* T3Chat5 = nullptr;
	Sprite* T3Chat6 = nullptr;

	// チュートリアル4
	Sprite* T4Chat1 = nullptr;
	Sprite* T4Chat2 = nullptr;
	Sprite* T4Chat3 = nullptr;
	Sprite* T4Chat4 = nullptr;

	// チュートリアル5
	Sprite* T5Chat1 = nullptr;
	Sprite* T5Chat2 = nullptr;
	Sprite* T5Chat3 = nullptr;
	Sprite* T5Chat4 = nullptr;
	Sprite* T5Chat5 = nullptr;

	Sprite* t1Background = nullptr;
	Sprite* t2Background = nullptr;
	Sprite* t3Background = nullptr;
	Sprite* t4Background = nullptr;
	Sprite* t5Background = nullptr;
	Sprite* s1Background = nullptr;
	Sprite* s2Background = nullptr;
	Sprite* s3Background = nullptr;
	Sprite* s4Background = nullptr;
	Sprite* s5Background = nullptr;
	Sprite* s6Background = nullptr;

	Sprite* t1StageSelectFrame = nullptr;
	Sprite* t2StageSelectFrame = nullptr;
	Sprite* t3StageSelectFrame = nullptr;
	Sprite* t4StageSelectFrame = nullptr;
	Sprite* t5StageSelectFrame = nullptr;
	Sprite* s1StageSelectFrame = nullptr;
	Sprite* s2StageSelectFrame = nullptr;
	Sprite* s3StageSelectFrame = nullptr;
	Sprite* s4StageSelectFrame = nullptr;
	Sprite* s5StageSelectFrame = nullptr;
	Sprite* s6StageSelectFrame = nullptr;

	Sprite* t1NewLog = nullptr;
	Sprite* t2NewLog = nullptr;
	Sprite* t3NewLog = nullptr;
	Sprite* t4NewLog = nullptr;
	Sprite* t5NewLog = nullptr;
	Sprite* s1NewLog = nullptr;
	Sprite* s2NewLog = nullptr;
	Sprite* s3NewLog = nullptr;
	Sprite* s4NewLog = nullptr;
	Sprite* s5NewLog = nullptr;
	Sprite* s6NewLog = nullptr;

	Sprite* t1ClearLog = nullptr;
	Sprite* t2ClearLog = nullptr;
	Sprite* t3ClearLog = nullptr;
	Sprite* t4ClearLog = nullptr;
	Sprite* t5ClearLog = nullptr;
	Sprite* s1ClearLog = nullptr;
	Sprite* s2ClearLog = nullptr;
	Sprite* s3ClearLog = nullptr;
	Sprite* s4ClearLog = nullptr;
	Sprite* s5ClearLog = nullptr;
	Sprite* s6ClearLog = nullptr;
	
	//pauseメニュー
	Sprite* pause1 = nullptr;
	Sprite* pause2 = nullptr;
	Sprite* pause3 = nullptr;
	Sprite* pause4 = nullptr;
#pragma endregion

	ParticleManager* particleMan = nullptr;

#pragma region Model
	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelPlane = nullptr;
	Model* modelTempWall = nullptr;
	Model* modelTempWall2 = nullptr;
	Model* modelYellowWall = nullptr;
	Model* modelTempBullet = nullptr;
	Model* modelTeleporterIn_1= nullptr;
	Model* modelTeleporterOut_1 = nullptr;
	Model* modelPlayerMarker = nullptr;

	// スイッチ
	Model* modelButtonRed = nullptr;
	Model* modelButtonBlue = nullptr;
	Model* modelButtonGreen = nullptr;
	Model* modelButtonYellow = nullptr;
	Model* modelButtonBomb = nullptr;
	Model* modelButtonFloor = nullptr;

	// 扉
	Model* modelDoorBase = nullptr;
	Model* modelDoorRed = nullptr;
	Model* modelDoorBlue = nullptr;
	Model* modelDoorGreen = nullptr;
	Model* modelDoorYellow = nullptr;

	Model* modelTESTONLY = nullptr;
#pragma endregion

#pragma region FbxModel
	// プレイヤー
	FbxModel* modelPlayerRun = nullptr;
	FbxModel* modelPlayerStand = nullptr;
	FbxModel* modelPlayerFight = nullptr;
	FbxModel* modelPlayerWin = nullptr;
	FbxModel* modelPlayerFall = nullptr;
	FbxModel* modelPlayerLose = nullptr;
	FbxModel* modelPlayerTalk = nullptr;

	// クローン
	FbxModel* modelCloneRun = nullptr;
	FbxModel* modelCloneStand = nullptr;
	FbxModel* modelCloneFight = nullptr;
	FbxModel* modelCloneFall = nullptr;
#pragma endregion
	
#pragma region Object3d
	//ステージセレクト
	Object3d* objT1 = nullptr;
	Object3d* objT2 = nullptr;
	Object3d* objT3 = nullptr;
	Object3d* objT4 = nullptr;
	Object3d* objS1 = nullptr;
	Object3d* objS2 = nullptr;
	Object3d* objS3 = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objTempBullet = nullptr;
	Object3d* objTempBulletE = nullptr;
	Object3d* objMenuSelection = nullptr;
	Object3d* objTempWall = nullptr;
	Object3d* objPlayerMarker = nullptr;

	// スイッチ
	Object3d* objButtonRed1 = nullptr;
	Object3d* objButtonRed2 = nullptr;
	Object3d* objButtonBlue = nullptr;
	Object3d* objButtonGreen1 = nullptr;
	Object3d* objButtonGreen2 = nullptr;
	Object3d* objButtonYellow = nullptr;
	Object3d* objButtonBomb1 = nullptr;
	Object3d* objButtonBomb2 = nullptr;
	Object3d* objButtonFloor = nullptr;

	// ワープ
	Object3d* objTeleporterIn1 = nullptr;
	Object3d* objTeleporterOut1 = nullptr;
	Object3d* objTeleporterIn2 = nullptr;
	Object3d* objTeleporterOut2 = nullptr;
	Object3d* objTeleporterIn3 = nullptr;
	Object3d* objTeleporterOut3 = nullptr;
	Object3d* objTeleporterIn4 = nullptr;
	Object3d* objTeleporterOut4 = nullptr;

	// 扉
	Object3d* objDoorBase = nullptr;
	Object3d* objDoorRed = nullptr;
	Object3d* objDoorBlue = nullptr;
	Object3d* objDoorGreen = nullptr;
	Object3d* objDoorYellow = nullptr;
#pragma endregion


	TouchableObject* objGround = nullptr;

#pragma region FbxObject3d
	// プレイヤー
	FbxObject3d* objPlayerRun = nullptr;
	FbxObject3d* objPlayerStand = nullptr;
	FbxObject3d* objPlayerFight = nullptr;
	FbxObject3d* objPlayerWin = nullptr;
	FbxObject3d* objPlayerLose = nullptr;
	FbxObject3d* objPlayerFall = nullptr;
	FbxObject3d* objPlayerTalk = nullptr;

	// クローン
	FbxObject3d* objCloneRun = nullptr;
	FbxObject3d* objCloneStand = nullptr;
	FbxObject3d* objCloneFight = nullptr;
	FbxObject3d* objCloneFall = nullptr;
#pragma endregion

	Player* objFighter = nullptr;

	Enemy* objClone = nullptr;

	FbxObject3d* fbxobject1 = nullptr;

	//球の初期位置変数
	float InitBulletPos_PX = -9.0f;

	//敵側の球の初期位置
	float InitBulletPos_EX = 9.0f;

	float rate = 1.0f;	//移動倍率係数
	float Speed = 0.4f;	//移動スピード

	float move;

	XMFLOAT2 SpriteStagSize = { 760,428 };

	float SpriteStageSizeX = 260.0f;
	float SpriteStageSizeY = 180.0f;

	XMFLOAT2 t1BackgroundPosition = { SpriteStageSizeX + (1280 * 0), SpriteStageSizeY };
	XMFLOAT2 t2BackgroundPosition = { SpriteStageSizeX + (1280 * 1), SpriteStageSizeY };
	XMFLOAT2 t3BackgroundPosition = { SpriteStageSizeX + (1280 * 2), SpriteStageSizeY };
	XMFLOAT2 t4BackgroundPosition = { SpriteStageSizeX + (1280 * 3), SpriteStageSizeY };
	XMFLOAT2 t5BackgroundPosition = { SpriteStageSizeX + (1280 * 4), SpriteStageSizeY };
	XMFLOAT2 s1BackgroundPosition = { SpriteStageSizeX + (1280 * 5), SpriteStageSizeY };
	XMFLOAT2 s2BackgroundPosition = { SpriteStageSizeX + (1280 * 6), SpriteStageSizeY };
	XMFLOAT2 s3BackgroundPosition = { SpriteStageSizeX + (1280 * 7), SpriteStageSizeY };
	XMFLOAT2 s4BackgroundPosition = { SpriteStageSizeX + (1280 * 8), SpriteStageSizeY };
	XMFLOAT2 s5BackgroundPosition = { SpriteStageSizeX + (1280 * 9), SpriteStageSizeY };
	XMFLOAT2 s6BackgroundPosition = { SpriteStageSizeX + (1280 * 10), SpriteStageSizeY };

	XMFLOAT2 t1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 0)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 t2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 1)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 t3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 2)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 t4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 3)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 t5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 4)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 s1StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 5)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 s2StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 6)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 s3StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 7)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 s4StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 8)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 s5StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 9)) - 20, (SpriteStageSizeY - 11) };
	XMFLOAT2 s6StageSelectFramePosition = { (SpriteStageSizeX + (1280 * 10)) - 20, (SpriteStageSizeY - 11) };

	XMFLOAT2 t1NewLogPosition = { (SpriteStageSizeX + (1280 * 0)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 t2NewLogPosition = { (SpriteStageSizeX + (1280 * 1)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 t3NewLogPosition = { (SpriteStageSizeX + (1280 * 2)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 t4NewLogPosition = { (SpriteStageSizeX + (1280 * 3)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 t5NewLogPosition = { (SpriteStageSizeX + (1280 * 4)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 s1NewLogPosition = { (SpriteStageSizeX + (1280 * 5)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 s2NewLogPosition = { (SpriteStageSizeX + (1280 * 6)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 s3NewLogPosition = { (SpriteStageSizeX + (1280 * 7)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 s4NewLogPosition = { (SpriteStageSizeX + (1280 * 8)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 s5NewLogPosition = { (SpriteStageSizeX + (1280 * 9)) - 80, (SpriteStageSizeY + 60) };
	XMFLOAT2 s6NewLogPosition = { (SpriteStageSizeX + (1280 * 10)) - 80, (SpriteStageSizeY + 60) };

	XMFLOAT2 t1ClearLogPosition = { (SpriteStageSizeX + (1280 * 0)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 t2ClearLogPosition = { (SpriteStageSizeX + (1280 * 1)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 t3ClearLogPosition = { (SpriteStageSizeX + (1280 * 2)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 t4ClearLogPosition = { (SpriteStageSizeX + (1280 * 3)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 t5ClearLogPosition = { (SpriteStageSizeX + (1280 * 4)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 s1ClearLogPosition = { (SpriteStageSizeX + (1280 * 5)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 s2ClearLogPosition = { (SpriteStageSizeX + (1280 * 6)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 s3ClearLogPosition = { (SpriteStageSizeX + (1280 * 7)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 s4ClearLogPosition = { (SpriteStageSizeX + (1280 * 8)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 s5ClearLogPosition = { (SpriteStageSizeX + (1280 * 9)) - 150, (SpriteStageSizeY + 100) };
	XMFLOAT2 s6ClearLogPosition = { (SpriteStageSizeX + (1280 * 10)) - 150, (SpriteStageSizeY + 100) };

	CollisionManager* collisionManager;

	//マップチップ mapchip
	std::vector<Object3d*> objects_t1_1; //チュートリアル　１
	std::vector<Object3d*> objects_t1_2; //チュートリアル　１
	std::vector<Object3d*> objects_t2_1; //チュートリアル　２
	std::vector<Object3d*> objects_t2_2; //チュートリアル　２
	std::vector<Object3d*> objects_t3_1; //チュートリアル　３
	std::vector<Object3d*> objects_t3_2; //チュートリアル　３
	std::vector<Object3d*> objects_t3_y1_1; //チュートリアル　３
	std::vector<Object3d*> objects_t3_y1_2; //チュートリアル　３
	std::vector<Object3d*> objects_t3_y1_3; //チュートリアル　３
	std::vector<Object3d*> objects_t3_y2_1; //チュートリアル　３
	std::vector<Object3d*> objects_t3_y2_2; //チュートリアル　３
	std::vector<Object3d*> objects_t3_y2_3; //チュートリアル　３
	std::vector<Object3d*> objects_at4_1; //チュートリアル　４
	std::vector<Object3d*> objects_at4_2; //チュートリアル　４
	std::vector<Object3d*> objects_t5_1; //チュートリアル　5
	std::vector<Object3d*> objects_t5_2; //チュートリアル　5
	std::vector<Object3d*> objects_t5_s2; //チュートリアル　5
	std::vector<Object3d*> objects; //ステージ　１
	std::vector<Object3d*> objects_2; //ステージ　１
	std::vector<Object3d*> objects_s2_1; //ステージ　２
	std::vector<Object3d*> objects_s2_2; //ステージ　２
	std::vector<Object3d*> objects_s2_y; //ステージ　２
	std::vector<Object3d*> objects_s2_y2; //ステージ　２
	std::vector<Object3d*> objects_s2_s; //ステージ　２
	std::vector<Object3d*> objects_s2_s2; //ステージ　２
	std::vector<Object3d*> objects_t4_1; //新しいステージ3
	std::vector<Object3d*> objects_t4_2; //新しいステージ３
	std::vector<Object3d*> objects_t4_y; //新しいステージ３
	std::vector<Object3d*> objects_t4_y2; //新しいステージ３
	std::vector<Object3d*> objects_s3_1; //ステージ　5
	std::vector<Object3d*> objects_s3_2; //ステージ　5
	std::vector<Object3d*> objects_s3_y1_1; //ステージ　5
	std::vector<Object3d*> objects_s3_y1_2; //ステージ　5
	std::vector<Object3d*> objects_s3_y2_1; //ステージ　5
	std::vector<Object3d*> objects_s3_y2_2; //ステージ　5
	std::vector<Object3d*> objects_s6_1; //ステージ　６
	std::vector<Object3d*> objects_s6_2; //ステージ　６
	std::vector<Object3d*> objects_s6_y1_1; //ステージ　６
	std::vector<Object3d*> objects_s6_y2_1; //ステージ　６
	std::vector<Object3d*> objects_s6_y1_2; //ステージ　６
	std::vector<Object3d*> objects_s6_y2_2; //ステージ　６
	std::vector<Object3d*> objects_s6_y1_3; //ステージ　６
	std::vector<Object3d*> objects_s6_y2_3; //ステージ　６
	std::vector<Object3d*> objects_s6_y2_4; //ステージ　６
	std::vector<Object3d*> objects_Wall;

	TitleScene* titleScene = nullptr;
	GameOver* gameOver = nullptr;
	GameClear* gameClear = nullptr;

	//オブジェクトのポジション、回転角度
	XMFLOAT3 playerPosition;
	XMFLOAT3 playerRotation;
	XMFLOAT3 enemyPosition;
	XMFLOAT3 enemyRotation;
	XMFLOAT3 playerTrigger;
	XMFLOAT3 enemyTrigger;
	XMFLOAT3 yellowTrigger1;
	XMFLOAT3 yellowTrigger2;
	XMFLOAT3 BlueButton;
	XMFLOAT3 YellowButton;
	XMFLOAT3 RedButton1;
	XMFLOAT3 RedButton2;
	XMFLOAT3 GreenButton1;
	XMFLOAT3 GreenButton2;
	XMFLOAT3 BombButton1;
	XMFLOAT3 BombButton2;
	XMFLOAT3 FloorButton;
	XMFLOAT3 playerBullet;
	XMFLOAT3 enemyBullet;
	XMFLOAT3 FBXplayerPosition;
	XMFLOAT3 FBXplayerRotation;
	XMFLOAT3 FBXclonePosition;
	XMFLOAT3 FBXcloneRotation;
	XMFLOAT3 menuBallRotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 playerMarkerPosition;
	XMFLOAT3 playerMarkerRotation;
	XMFLOAT3 playerPausePosition;
	XMFLOAT3 playerPauseRotation;
	XMFLOAT3 clonePausePosition;
	XMFLOAT3 clonePauseRotation;
	XMFLOAT3 playerMarkerPausePosition;
	XMFLOAT3 playerMarkerPauseRotation;

	int stage1Init = 0;
	int FBXModelChange = 0;
	int pauseMenuSelection = 0; //pause
	int menuSelection = 0; // ゲームクリアとオーバー画面
	bool menuMoving = false; // ゲームクリアとオーバー画面
	int lastScene = 0; // ゲームクリアとオーバー画面

	int delayFrame = 0;
	bool delay = false;

	//コントローラー起動フラグ
	bool ControllerFlag = false;

	//コントローラー検知用タイマー
	int ConTimer;

	//シーン番号記録(音楽再生に使う)
	bool PlayFlag = false;

	//Cinematic Camera Variables シネマティックカメラ変数
	int maximumTime, currentFrame, timeRate;
	bool beginStage = false;
	bool cameraFlag = false;
	XMFLOAT3 controlPoint, cameraStartPosition, cameraEndPosition;
	int currentCameraFrame = 0;
	bool cameraChange = false;

	XMFLOAT3 originalCamera;

	//Waveのボリューム
	const float Volume = 0.1f;

	//ワープゾーンの回転スピード
	const float WarpRotate = 3.0f;
	const float WarpSize = 1.7f;

	const int DIV_NUM = 10;
	const float LAND_SCALE = 3.0f;

	//ステージ1用外壁マップチップ
	const int WALL_NUM = 23;
};