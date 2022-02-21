#include "GameScene.h"
#include "Model.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"Camera.h"

using namespace DirectX;

//あんじょうテスト
//よっしーテスト♡

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete( spriteBG );
	safe_delete( objSkydome );
	safe_delete( objGround );
	safe_delete( objFighter );
	safe_delete( modelSkydome );
	safe_delete( modelGround );
	safe_delete( modelFighter );

	safe_delete( fbxobject1 );
	safe_delete( fbxmodel1 );
}

void GameScene::Initialize( DirectXCommon *dxCommon, Input *input, Audio *audio )
{
	// nullptrチェック nullptr check
	assert( dxCommon );
	assert( input );
	assert( audio );

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
	
	// カメラ生成 Camera generation
	camera = new Camera( WinApp::window_width, WinApp::window_height );


	// カメラセット Camera set
	Object3d::SetCamera( camera );
	FbxObject3d::SetCamera( camera );


	// デバイスをセット Set the device
	FbxObject3d::SetDevice( dxCommon->GetDevice() );

	// グラフィックスパイプライン生成 Graphics pipeline generation
	FbxObject3d::CreateGraphicsPipeline();


	// デバッグテキスト用テクスチャ読み込み Import texture for debug text
	if ( !Sprite::LoadTexture( debugTextTexNumber, L"Resources/debugfont.png" ) ) {
		assert( 0 );
		return;
	}
	// デバッグテキスト初期化 Debug text initialization
	debugText.Initialize( debugTextTexNumber );
	
	// テクスチャ読み込み Texture loading
	if ( !Sprite::LoadTexture( 1, L"Resources/background.png" ) ) {
		assert( 0 );
		return;
	}
	// 背景スプライト生成 Background sprite generation
	spriteBG = Sprite::Create( 1, { 0.0f,0.0f } );

	// パーティクルマネージャー
	particleMan = ParticleManager::Create( dxCommon->GetDevice(), camera );

	// 3Dオブジェクト生成 3D object generation
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objFighter = Object3d::Create();

	// テクスチャ2番に読み込み Load into texture # 2
	Sprite::LoadTexture( 2, L"Resources/texture.png" );

	modelSkydome = Model::CreateFromOBJ( "skydome" );
	modelGround = Model::CreateFromOBJ( "ground" );
	modelFighter = Model::CreateFromOBJ( "chr_sword" );

	objSkydome->SetModel( modelSkydome );
	objGround->SetModel( modelGround );
	objFighter->SetModel( modelFighter );

	fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "bonetest" );
	//fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "cube" );
	//fbxmodel1 = FbxLoader::GetInstance()->LoadModelFromFile( "bone" );

	// FBX3Dオブジェクト生成とモデルとセット FBX3D object generation and model set
	fbxobject1 = new FbxObject3d;
	fbxobject1->Initialize();
	fbxobject1->SetModel( fbxmodel1 );
}

void GameScene::Update()
{
	// オブジェクト移動 Move object
	if ( input->PushKey( DIK_I ) || input->PushKey( DIK_K ) || input->PushKey( DIK_J ) || input->PushKey( DIK_L ) )
	{
		// 現在の座標を取得 Get current coordinates
		XMFLOAT3 position = objFighter->GetPosition();

		// 移動後の座標を計算 Calculate the coordinates after moving
		if ( input->PushKey( DIK_I ) ) { position.y += 1.0f; }
		else if ( input->PushKey( DIK_K ) ) { position.y -= 1.0f; }
		if ( input->PushKey( DIK_L ) ) { position.x += 1.0f; }
		else if ( input->PushKey( DIK_J ) ) { position.x -= 1.0f; }

		// 座標の変更を反映 Reflect the change in coordinates
		objFighter->SetPosition( position );

		// 現在の座標を取得 Get current coordinates
		XMFLOAT3 PlayerPosition = fbxobject1->GetPosition();

		// 移動後の座標を計算 Calculate the coordinates after moving
		if ( input->PushKey( DIK_I ) ) { PlayerPosition.y += 1.0f; }
		else if ( input->PushKey( DIK_K ) ) { PlayerPosition.y -= 1.0f; }
		if ( input->PushKey( DIK_L ) ) { PlayerPosition.x += 1.0f; }
		else if ( input->PushKey( DIK_J ) ) { PlayerPosition.x -= 1.0f; }

		// 座標の変更を反映 Reflect the change in coordinates
		fbxobject1->SetPosition( PlayerPosition );
	}


	MoveCamera();
	// パーティクル生成 Particle generation
	CreateParticles();

	camera->Update();
	particleMan->Update();

	objSkydome->Update();
	objGround->Update();
	objFighter->Update();

	fbxobject1->Update();

	debugText.Print( "AD: move camera LeftRight", 50, 50, 1.0f );
	debugText.Print( "WS: move camera UpDown", 50, 70, 1.0f );
	debugText.Print( "ARROW: move camera FrontBack", 50, 90, 1.0f );
}

void GameScene::Draw()
{
	// コマンドリストの取得 Get command list
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画 Background sprite drawing
	// 背景スプライト描画前処理 Background sprite drawing pre-processing
	Sprite::PreDraw( cmdList );
	// 背景スプライト描画 Background sprite drawing
	spriteBG->Draw();
	
	// ここに背景スプライトの描画処理を追加できる You can add background sprite drawing processing here

	// スプライト描画後処理 Post-processing of sprite drawing
	Sprite::PostDraw();
	// 深度バッファクリア Depth buffer clear
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画 3D object drawing
	// 3Dオブジェクト描画前処理 3D object drawing pre-processing
	Object3d::PreDraw( cmdList );

	// 3Dオブクジェクトの描画 Drawing 3D objects
	/* objSkydome->Draw();
	 objGround->Draw();*/
	objFighter->Draw();

	fbxobject1->Draw( cmdList );

	// パーティクルの描画 Drawing particles
	particleMan->Draw( cmdList );
	
	// ここに3Dオブジェクトの描画処理を追加できる You can add 3D object drawing process here

	// 3Dオブジェクト描画後処理 Post-processing of 3D object drawing
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画 Foreground sprite drawing
	// 前景スプライト描画前処理 Foreground sprite drawing pre-processing
	Sprite::PreDraw( cmdList );

	// ここに前景スプライトの描画処理を追加できる You can add foreground sprite drawing processing here

	//// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画 Debug text drawing
	// debugText.DrawAll(cmdList);

	// スプライト描画後処理 Post-processing of sprite drawing
	Sprite::PostDraw();
#pragma endregion
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
		particleMan->Add( 60, pos, vel, acc, 1.0f, 0.0f );
	}
}
