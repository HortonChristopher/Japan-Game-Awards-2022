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

class Stage1
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

public: // メンバ関数 Member function

	// コンストクラタ Constructer
	Stage1();

	// デストラクタ Destructor
	~Stage1();

	void Initialize();

	void Update();

	void DrawBGsprite();

	void Draw3Dobject();

	void DrawFGsprite();

	int intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH);

private: // メンバ変数 Member variables
	bool lastIntersect = false;
	bool lastIntersectE = false;

	bool enemyAlive = true;
	bool playerAlive = true;

	bool playerBulletF = false;
	bool enemyBulletF = false;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// ゲームシーン用 For game scenes
	Camera* camera = nullptr;

	Sprite* spriteBG = nullptr;

	ParticleManager* particleMan = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelPlane = nullptr;
	Model* modelBox = nullptr;
	Model* modelPyramid = nullptr;
	Model* modelTempWall = nullptr;
	Model* modelTempTrigger = nullptr;
	Model* modelTempBullet = nullptr;

	FbxModel* fbxmodel1 = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objTempTrigger = nullptr;
	Object3d* objTempTriggerE = nullptr;
	Object3d* objTempBullet = nullptr;
	Object3d* objTempBulletE = nullptr;

	Object3d* objTempWall = nullptr;

	TouchableObject* objGround = nullptr;

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

	//XMFLOAT3 PlayerPosition = { 50.0f, 0.0f, 0.0f };
	//XMFLOAT3 position = { -50.0f, 0.0f, 0.0f };

	CollisionManager* collisionManager;

	std::vector<Object3d*> objects;
	std::vector<Object3d*> objects_2;
	std::vector<Object3d*> objects_Wall;
};

