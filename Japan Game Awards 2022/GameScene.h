#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
#include "DebugText.h"
#include "Audio.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"

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

private: // メンバ変数 Member variables
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;	

	// ゲームシーン用 For game scenes
	Camera *camera = nullptr;
	 
	Sprite* spriteBG = nullptr;

	ParticleManager *particleMan = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;

	FbxModel *fbxmodel1 = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;

	FbxObject3d *fbxobject1 = nullptr;
};

