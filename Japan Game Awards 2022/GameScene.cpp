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
#include "Stage1.h"
#include "GameOver.h"
#include "GameClear.h"

using namespace DirectX;
extern int sceneNo = 0; //タイトル Title
extern int sceneChange = 0;

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
	/*for (auto object : objects) {
		safe_delete(object);
	}

	for (auto object_2 : objects_2) {
		safe_delete(object_2);
	}

	safe_delete( spriteBG );
	safe_delete( objSkydome );
	safe_delete( objGround );
	safe_delete( objFighter );
	safe_delete( objClone );
	safe_delete(objTempTrigger);
	safe_delete(objTempTriggerE);
	safe_delete(objTempBullet);
	safe_delete(objTempBulletE);
	safe_delete( modelSkydome );
	safe_delete( modelGround );
	safe_delete( modelFighter );
	safe_delete(modelPlane);
	safe_delete(modelBox);
	safe_delete(modelPyramid);

	safe_delete( fbxobject1 );
	safe_delete( fbxmodel1 );
	ReleaseInput();*/
}

void GameScene::Initialize( DirectXCommon *dxCommon, Input *input, Audio *audio )
{
	//// nullptrチェック nullptr check
	assert( dxCommon );
	assert( input );
	assert( audio );

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	//コントローラー初期化
	InitInput();
	
	//// カメラ生成 Camera generation
	//camera = new Camera( WinApp::window_width, WinApp::window_height );

	//collisionManager = CollisionManager::GetInstance();

	//// カメラセット Camera set
	//Object3d::SetCamera( camera );
	//FbxObject3d::SetCamera( camera );

	// デバイスをセット Set the device
	FbxObject3d::SetDevice( dxCommon->GetDevice() );

	//// グラフィックスパイプライン生成 Graphics pipeline generation
	//FbxObject3d::CreateGraphicsPipeline();

	////コントローラー初期化
	//InitInput();

	//// デバッグテキスト用テクスチャ読み込み Import texture for debug text
	//if ( !Sprite::LoadTexture( debugTextTexNumber, L"Resources/debugfont.png" ) ) {
	//	assert( 0 );
	//	return;
	//}
	//// デバッグテキスト初期化 Debug text initialization
	//debugText.Initialize( debugTextTexNumber );

	//タイトルシーン初期化 Title Scene Initialization
	titleScene = new TitleScene();
	titleScene->Initialize();

	gameOver = new GameOver();
	gameOver->Initialize();

	gameClear = new GameClear();
	gameOver->Initialize();

	//ステージ初期化 Stage Initialization
	stage1 = new Stage1();
	
	//// テクスチャ読み込み Texture loading
	//if ( !Sprite::LoadTexture( 1, L"Resources/background.png" ) ) {
	//	assert( 0 );
	//	return;
	//}
	//// 背景スプライト生成 Background sprite generation
	//spriteBG = Sprite::Create( 1, { 0.0f,0.0f } );

	//// パーティクルマネージャー
	//particleMan = ParticleManager::Create( dxCommon->GetDevice(), camera );

	//// 3Dオブジェクト生成 3D object generation
	//objSkydome = Object3d::Create();
	//objTempTrigger = Object3d::Create();
	//objTempTriggerE = Object3d::Create();
	//objTempBullet = Object3d::Create();
	//objTempBulletE = Object3d::Create();

	//// テクスチャ2番に読み込み Load into texture # 2
	//Sprite::LoadTexture( 2, L"Resources/texture.png" );

	//modelSkydome = Model::CreateFromOBJ( "skydome" );
	//modelGround = Model::CreateFromOBJ( "ground" );
	//modelFighter = Model::CreateFromOBJ( "chr_sword" );
	//modelPlane = Model::CreateFromOBJ("yuka");
	//modelBox = Model::CreateFromOBJ("box1x1x1");
	////modelPyramid = Model::CreateFromOBJ("pyramid1x1");
	//modelTempWall = Model::CreateFromOBJ("TempWall");
	//modelTempTrigger = Model::CreateFromOBJ("TempTrigger");
	//modelTempBullet = Model::CreateFromOBJ("bullet2");

	//objSkydome->SetModel( modelSkydome );
	////objGround->SetModel( modelGround );
	////objFighter->SetModel( modelFighter );
	//objTempTrigger->SetModel(modelTempTrigger);
	//objTempTriggerE->SetModel(modelTempTrigger);
	//objTempBullet->SetModel(modelTempBullet);
	//objTempBulletE->SetModel(modelTempBullet);

	////objGround = TouchableObject::Create(modelGround);
	//objFighter = Player::Create(modelFighter);
	//objClone = Enemy::Create(modelFighter);

	////fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "bonetest" );
	////fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "cube" );
	////fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "bone" );

	//// モデルテーブル Model table
	//Model* modeltable[12] = {
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelTempWall,
	//};

	//Model* modeltable_2[12] = {
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelPlane,
	//	modelTempWall,
	//};

	//const int DIV_NUM = 10;
	//const float LAND_SCALE = 3.0f;

	////ステージ1用外壁マップチップ
	//const int WALL_NUM = 23;


	////自分側のマップチップ生成(Map chip generation)
	//for (int i = 0; i < DIV_NUM; i++) { // y coordinate - Bottom to Top
	//	for (int j = 0; j < 6; j++) { // x coordinate - Left to Right
	//		int modelIndex = rand() % 10;

	//		//ステージ外壁の配置
	//		//下
	//		if (i == 0 && j == j)
	//		{
	//			modelIndex = 10;
	//		}

	//		//上
	//		if (i == 9 && j == j)
	//		{
	//			modelIndex = 10;
	//		}

	//		//左
	//		if (i == i && j == 0)
	//		{
	//			modelIndex = 10;
	//		}

	//		//右
	//		if (i == i && j == 5)
	//		{
	//			modelIndex = 10;
	//		}

	//		if (i == 5 && j == 5)
	//		{
	//			modelIndex = 0;
	//		}

	//		TouchableObject* object = TouchableObject::Create(modeltable[modelIndex]);
	//		object->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE });
	//		object->SetPosition({ (j - DIV_NUM / 2) * LAND_SCALE - LAND_SCALE * 1, 0, (i - DIV_NUM / 2) * LAND_SCALE });
	//		objects.push_back(object);
	//	}
	//}

	////敵側のマップチップ生成(Enemy side map chip generation)
	//for (int i = 0; i < DIV_NUM; i++) { // y coordinate - Bottom to Top
	//	for (int j = 0; j < 6; j++) { // x coordinate - Left to Right

	//		int modelIndex = rand() % 10;

	//		//ステージ外壁の配置
	//		//下
	//		if (i == 0 && j == j)
	//		{
	//			modelIndex = 10;
	//		}

	//		//上
	//		if (i == 9 && j == j)
	//		{
	//			modelIndex = 10;
	//		}

	//		//左
	//		if (i == i && j == 0)
	//		{
	//			modelIndex = 10;
	//		}

	//		//右
	//		if (i == i && j == 5)
	//		{
	//			modelIndex = 10;
	//		}

	//		if (i == 5 && j == 5)
	//		{
	//			modelIndex = 0;
	//		}

	//		
	//		TouchableObject* object_2 = TouchableObject::Create(modeltable_2[modelIndex]);
	//		object_2->SetScale({ LAND_SCALE, LAND_SCALE, LAND_SCALE});
	//		object_2->SetPosition({ (j - DIV_NUM / 2) * 0.5f * LAND_SCALE * (-2) + LAND_SCALE, 0, (i - DIV_NUM / 2) * LAND_SCALE});
	//		objects_2.push_back(object_2);
	//	}
	//}

	//objFighter->SetScale({ 1,1,1 });
	//objClone->SetScale({ 1,1,1 });

	//objFighter->SetPosition({ -12,0,-12 });
	//objClone->SetPosition({ 12,0,-12 });

	//objTempTrigger->SetPosition({ InitBulletPos_PX, 0, 0 });
	//objTempTriggerE->SetPosition({ InitBulletPos_EX, 0, 0 });

	//objTempBullet->SetPosition({ -6.0f, 1.0f, 0 });
	//objTempBulletE->SetPosition({ 6.0f, 1.0f, 0 });
	//objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	//objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	//camera->SetTarget({ 0, 1, 0 });
}

void GameScene::Update()
{
	UpdateInput();

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
			if (stage1Init == 0)
			{
				stage1->Initialize();
				stage1Init = 1;
			}
			break;
		}

		if (IsButtonDown(ButtonKind::Button_A))
		{
			sceneNo = 1;
			titleScene->Finalize();
			if (stage1Init == 0)
			{
				stage1->Initialize();
				stage1Init = 1;
			}
			break;
		}


		break;
	case 1:
		stage1->Update();
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
			gameClear->Finalize();
			titleScene->Initialize();
			break;
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
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画 Background sprite drawing
	// 背景スプライト描画前処理 Background sprite drawing pre-processing
	Sprite::PreDraw( cmdList );
	// 背景スプライト描画 Background sprite drawing
	//spriteBG->Draw();
	switch (sceneNo)
	{
	case 0:
		titleScene->Draw();
		break;
	case 1:
		stage1->DrawBGsprite();
		break;
	case 2:
		if (sceneChange == 0)
		{
			stage1->Finalize();
			gameClear->Initialize();
			sceneChange = 1;
		}
		gameClear->Draw();
		break;
	case 3:
		if (sceneChange == 0)
		{
			stage1->Finalize();
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
	Object3d::PreDraw( cmdList );

	switch (sceneNo)
	{
	case 0:
		break;
	case 1:
		stage1->Draw3Dobject();
		break;
	case 2:
		break;
	case 3:
		break;
	}

	// 3Dオブクジェクトの描画 Drawing 3D objects
	/* objSkydome->Draw();
	 objGround->Draw();*/
	/*for (auto object : objects) {
		object->Draw();
	}

	for (auto object_2 : objects_2) {
		object_2->Draw();
	}*/

	/*if (playerAlive)
	{
		objFighter->Draw();
	}
	
	if (enemyAlive)
	{
		objClone->Draw();
	}*/

	/*objTempTrigger->Draw();
	objTempTriggerE->Draw();

	objTempBullet->Draw();
	objTempBulletE->Draw();*/

	//fbxobject1->Draw( cmdList );

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
		stage1->DrawFGsprite();
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
	if ( input->PushKey( DIK_W ) || input->PushKey( DIK_S ) || input->PushKey( DIK_D ) || input->PushKey( DIK_A ) )
	{
		if ( input->PushKey( DIK_W ) ) { camera->MoveVector( { 0.0f,+0.06f,0.0f } ); }
		else if ( input->PushKey( DIK_S ) ) { camera->MoveVector( { 0.0f,-0.06f,0.0f } ); }
		if ( input->PushKey( DIK_D ) ) { camera->MoveVector( { +0.06f,0.0f,0.0f } ); }
		else if ( input->PushKey( DIK_A ) ) { camera->MoveVector( { -0.06f,0.0f,0.0f } ); }
	}

	// カメラ移動 Camera movement
	if ( input->PushKey( DIK_UP ) || input->PushKey( DIK_DOWN ) )
	{
		if ( input->PushKey( DIK_UP ) ) { camera->MoveVector( { 0.0f,0.0f,+0.06f } ); }
		else if ( input->PushKey( DIK_DOWN ) ) { camera->MoveVector( { 0.0f,0.0f,-0.06f } ); }
	}
}

void GameScene::CreateParticles()
{
	for ( int i = 0; i < 10; i++ ) {
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