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

#include <vector>

class CollisionManager;
class Player;
class Enemy;
class TouchableObject;
class TitleScene;
class Stage1;
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
	
	// コンストクラタ Constcrater
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

	/*bool lastIntersect = false;
	bool lastIntersectE = false;

	bool enemyAlive = true;
	bool playerAlive = true;

	bool playerBulletF = false;
	bool enemyBulletF = false;*/

private: // メンバ変数 Member variables
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;	

	// ゲームシーン用 For game scenes
	Camera *camera = nullptr;
	 
	//Sprite* spriteBG = nullptr;

	//ParticleManager *particleMan = nullptr;

	//Model* modelSkydome = nullptr;
	//Model* modelGround = nullptr;
	//Model* modelFighter = nullptr;
	//Model* modelPlane = nullptr;
	//Model* modelBox = nullptr;
	//Model* modelPyramid = nullptr;
	//Model* modelTempWall = nullptr;
	//Model* modelTempTrigger = nullptr;
	//Model* modelTempBullet = nullptr;

	//FbxModel *fbxmodel1 = nullptr;
	FbxModel* modelPlayerRun = nullptr;
	FbxModel* modelPlayerStand = nullptr;

	//Object3d* objSkydome = nullptr;
	//Object3d* objTempTrigger = nullptr;
	//Object3d* objTempTriggerE = nullptr;
	//Object3d* objTempBullet = nullptr;
	//Object3d* objTempBulletE = nullptr;

	//TouchableObject* objGround = nullptr;
	FbxObject3d* objPlayerRun = nullptr;
	FbxObject3d* objCloneRun = nullptr;
	FbxObject3d* objPlayerStand = nullptr;
	FbxObject3d* objCloneStand = nullptr;

	//Player* objFighter = nullptr;

	//Enemy* objClone = nullptr;

	//FbxObject3d *fbxobject1 = nullptr;

	//球の初期位置変数
	float InitBulletPos_PX = -9.0f;

	//敵側の球の初期位置
	float InitBulletPos_EX = 9.0f;

	float rate = 1.0f;	//移動倍率係数
	float Speed = 0.4f;	//移動スピード

	//float move;

	////XMFLOAT3 PlayerPosition = { 50.0f, 0.0f, 0.0f };
	////XMFLOAT3 position = { -50.0f, 0.0f, 0.0f };

	CollisionManager* collisionManager;

	std::vector<Object3d*> objects;
	std::vector<Object3d*> objects_2;
	std::vector<Object3d*> objects_Wall;

	TitleScene* titleScene = nullptr;
	Stage1* stage1 = nullptr;
	GameOver* gameOver = nullptr;
	GameClear* gameClear = nullptr;

	int stage1Init = 0;
	int FBXModelChange = 0;

	//コントローラー起動フラグ
	bool ControllerFlag = false;

	//コントローラー検知用タイマー
	int ConTimer;
};

