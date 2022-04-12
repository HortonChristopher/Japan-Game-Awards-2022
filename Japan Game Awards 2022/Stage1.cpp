#include "Stage1.h"
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
#include "GameScene.h"

using namespace DirectX;
extern int sceneNo;
extern int sceneChange;
extern XMFLOAT3 playerPositionTemp;
extern XMFLOAT3 playerRotationTemp;
extern XMFLOAT3 clonePositionTemp;
extern XMFLOAT3 cloneRotationTemp;
extern DirectXCommon* dxCommon;

Stage1::Stage1()
{
}

Stage1::~Stage1()
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
	safe_delete(fbxmodel1);
	ReleaseInput();
}

void Stage1::Initialize()
{
	// nullptrチェック nullptr check
	//assert(dxCommon);
	//assert(input);
	//assert(audio);

	//this->dxCommon = dxCommon;
	//this->input = input;
	//this->audio = audio;

	gameOver = new GameOver();
	gameOver->Initialize();

	gameClear = new GameClear();
	gameOver->Initialize();

	// カメラ生成 Camera generation
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	collisionManager = CollisionManager::GetInstance();

	// カメラセット Camera set
	Object3d::SetCamera(camera);
	FbxObject3d::SetCamera(camera);

	// デバイスをセット Set the device
	//FbxObject3d::SetDevice(dxCommon->GetDevice());

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

	// テクスチャ読み込み Texture loading
	if ( !Sprite::LoadTexture( 1, L"Resources/background.png" ) ) {
		assert( 0 );
		return;
	}
	// 背景スプライト生成 Background sprite generation
	spriteBG = Sprite::Create( 1, { 0.0f,0.0f } );

	// パーティクルマネージャー
	//particleMan = ParticleManager::Create( dxCommon->GetDevice(), camera );

	// 3Dオブジェクト生成 3D object generation
	objSkydome = Object3d::Create();
	objTempTrigger = Object3d::Create();
	objTempTriggerE = Object3d::Create();
	objTempBullet = Object3d::Create();
	objTempBulletE = Object3d::Create();

	// テクスチャ2番に読み込み Load into texture # 2
	Sprite::LoadTexture( 2, L"Resources/texture.png" );

	modelSkydome = Model::CreateFromOBJ( "skydome" );
	modelGround = Model::CreateFromOBJ( "ground" );
	modelFighter = Model::CreateFromOBJ( "chr_sword" );
	modelPlane = Model::CreateFromOBJ("yuka");
	modelBox = Model::CreateFromOBJ("box1x1x1");
	//modelPyramid = Model::CreateFromOBJ("pyramid1x1");
	modelTempWall = Model::CreateFromOBJ("kabe");
	modelTempTrigger = Model::CreateFromOBJ("TempTrigger");
	modelTempBullet = Model::CreateFromOBJ("bullet2");

	objSkydome->SetModel( modelSkydome );
	//objGround->SetModel( modelGround );
	//objFighter->SetModel( modelFighter );
	objTempTrigger->SetModel(modelTempTrigger);
	objTempTriggerE->SetModel(modelTempTrigger);
	objTempBullet->SetModel(modelTempBullet);
	objTempBulletE->SetModel(modelTempBullet);	

	//objGround = TouchableObject::Create(modelGround);
	objFighter = Player::Create(modelFighter);
	objClone = Enemy::Create(modelFighter);

	//fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "bonetest" );
	//fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "cube" );
	//fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "bone" );

	// FBX3Dオブジェクト生成とモデルとセット FBX3D object generation and model set
	//fbxobject1 = new FbxObject3d;
	//fbxobject1->Initialize();
	//fbxobject1->SetModel( fbxmodel1 );


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
	camera->MoveEyeVector({ 0, 25.0f, 25.0f });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;
}

void Stage1::Update()
{
	XMFLOAT3 playerPosition = objFighter->GetPosition();
	XMFLOAT3 playerRotation = objFighter->GetRotation();
	XMFLOAT3 enemyPosition = objClone->GetPosition();
	XMFLOAT3 enemyRotation = objClone->GetRotation();
	XMFLOAT3 playerTrigger = objTempTrigger->GetPosition();
	XMFLOAT3 enemyTrigger = objTempTriggerE->GetPosition();
	XMFLOAT3 playerBullet = objTempBullet->GetPosition();
	XMFLOAT3 enemyBullet = objTempBulletE->GetPosition();
	playerPositionTemp = playerPosition;
	playerRotationTemp = playerRotation;
	clonePositionTemp = enemyPosition;
	cloneRotationTemp = enemyRotation;

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
	} else
	{
		playerBullet.x = InitBulletPos_PX;
		objTempBullet->SetPosition(playerBullet);
	}

	if (enemyBulletF)
	{
		enemyBullet.x -= 0.1f;
		objTempBulletE->SetPosition(enemyBullet);
	} else
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
		sceneNo = 2;
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

	objTempBullet->Update();
	objTempBulletE->Update();

	//fbxobject1->Update();

	//debugText.Print( "", 50, 50, 1.0f );
	//debugText.Print( "WS: move camera UpDown", 50, 70, 1.0f );
	//debugText.Print( "ARROW: move camera FrontBack", 50, 90, 1.0f );

	lastIntersect = intersect(playerPosition, playerTrigger, 1.0f, 1.0f, 1.0f);
	lastIntersectE = intersect(enemyPosition, enemyTrigger, 1.0f, 1.0f, 1.0f);

	collisionManager->CheckAllCollisions();

	//Debug Start
	char msgbuf[256];
	char msgbuf2[256];
	//char msgbuf3[256];

	sprintf_s(msgbuf, 256, "Enemy X: %f\n", enemyPosition.x);
	sprintf_s(msgbuf2, 256, "Enemy Z: %f\n", enemyPosition.z);
	//sprintf_s(msgbuf3, 256, "isTouchingGround: %f\n", isTouchingGround);
	OutputDebugStringA(msgbuf);
	OutputDebugStringA(msgbuf2);
	//OutputDebugStringA(msgbuf3);
	//Debug End
}

void Stage1::Finalize()
{
	objFighter->SetPosition({ -12,0,-12 });
	objClone->SetPosition({ 12,0,-12 });

	objTempTrigger->SetPosition({ -12.0f, 0, 0 });
	objTempTriggerE->SetPosition({ 12.0f, 0, 0 });

	objTempBullet->SetPosition({ -6.0f, 1.0f, 0 });
	objTempBulletE->SetPosition({ 6.0f, 1.0f, 0 });
	objTempBullet->SetScale({ 0.25f, 0.25f, 0.25f });
	objTempBulletE->SetScale({ 0.25f, 0.25f, 0.25f });

	//camera->SetTarget({ 0, 1, 0 });
	//camera->MoveEyeVector({ 0, 25.0f, 25.0f });

	enemyAlive = true;
	playerAlive = true;

	playerBulletF = false;
	enemyBulletF = false;
}

void Stage1::DrawBGsprite()
{
	// 背景スプライト描画 Background sprite drawing
	spriteBG->Draw();
}

void Stage1::Draw3Dobject()
{
	// 3Dオブクジェクトの描画 Drawing 3D objects
	//objSkydome->Draw();
	//objGround->Draw();
	for (auto object : objects) {
		object->Draw();
	}

	for (auto object_2 : objects_2) {
		object_2->Draw();
	}

	if (playerAlive)
	{
		//objFighter->Draw();
	}
	
	if (enemyAlive)
	{
		//objClone->Draw();
	}

	objTempTrigger->Draw();
	objTempTriggerE->Draw();

	objTempBullet->Draw();
	objTempBulletE->Draw();

	//fbxobject1->Draw( cmdList );
}

void Stage1::DrawFGsprite()
{

}

int Stage1::intersect(XMFLOAT3 player, XMFLOAT3 wall, float circleR, float rectW, float rectH)
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