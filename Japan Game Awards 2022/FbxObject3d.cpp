#include "FbxObject3d.h"
#include "FbxLoader.h"

#include<d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ID3D12Device *FbxObject3d::device = nullptr;
Camera *FbxObject3d::camera = nullptr;

ComPtr<ID3D12RootSignature> FbxObject3d::rootSignature;
ComPtr<ID3D12PipelineState> FbxObject3d::pipelineState;

void FbxObject3d::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g Vertex shader object
	ComPtr<ID3DBlob> psBlob;    // �s�N�Z���V�F�[�_�I�u�W�F�N�g Pixel shader object
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g Error object

	assert( device );

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C�� Loading and compiling vertex shaders
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // �V�F�[�_�t�@�C���� Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ��� Enable to include
		"main", "vs_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w�� Entry point name, shader model specification
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ� Debug settings
		0,
		&vsBlob, &errorBlob );
	if ( FAILED( result ) ) {
		// errorBlob����G���[���e��string�^�ɃR�s�[ Copy the error content from errorBlob to string type
		std::string errstr;
		errstr.resize( errorBlob->GetBufferSize() );

		std::copy_n( (char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin() );
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\�� Display error details in output window
		OutputDebugStringA( errstr.c_str() );
		exit( 1 );
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C�� Loading and compiling pixel shaders
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // �V�F�[�_�t�@�C���� Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ��� Enable to include
		"main", "ps_5_0",    // �G���g���[�|�C���g���A�V�F�[�_�[���f���w�� Entry point name, shader model specification
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ� Debug settings
		0,
		&psBlob, &errorBlob );
	if ( FAILED( result ) ) {
		// errorBlob����G���[���e��string�^�ɃR�s�[ Copy the error content from errorBlob to string type
		std::string errstr;
		errstr.resize( errorBlob->GetBufferSize() );

		std::copy_n( (char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin() );
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\�� Display error details in output window
		OutputDebugStringA( errstr.c_str() );
		exit( 1 );
	}

	// ���_���C�A�E�g Vertex layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��) xy coordinates (it is easier to see if written in one line)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��) Normal vector (easier to see if written in one line)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��) uv coordinates (it is easier to see if written in one line)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �e�����󂯂�{�[���ԍ�(4��) Affected bone numbers (4)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �{�[���̃X�L���E�F�C�g(4��) Bone skin weights (4)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ� Set the flow of the graphics pipeline
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE( vsBlob.Get() );
	gpipeline.PS = CD3DX12_SHADER_BYTECODE( psBlob.Get() );

	// �T���v���}�X�N Sample mask
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g Rasterizer state
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC( D3D12_DEFAULT );
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g Depth stencil state
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC( D3D12_DEFAULT );

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ� Render target blend settings
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`�� RBGA Draw all channels
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	
	// �u�����h�X�e�[�g�̐ݒ� Blend state setting
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g Depth buffer format
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ� Vertex layout settings
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof( inputLayout );

	// �}�`�̌`��ݒ�i�O�p�`�j Shape shape setting (triangle)
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // �`��Ώۂ�1�� One drawing target
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA RGBA specified from 0 to 255
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O Sampling once per pixel

	// �f�X�N���v�^�����W Descriptor range
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0 ); // t0 ���W�X�^ t0 register

	// ���[�g�p�����[�^ Route parameters
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV�i���W�ϊ��s��p�j CBV (for coordinate transformation matrix)
	rootparams[0].InitAsConstantBufferView( 0, 0, D3D12_SHADER_VISIBILITY_ALL );
	// SRV�i�e�N�X�`���j SRV (texture)
	rootparams[1].InitAsDescriptorTable( 1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL );
	// CBV (�X�L�j���O�p) CBV (for skinning)
	rootparams[2].InitAsConstantBufferView( 3, 0, D3D12_SHADER_VISIBILITY_ALL );
	// �X�^�e�B�b�N�T���v���[ Static sampler
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC( 0 );

	// ���[�g�V�O�l�`���̐ݒ�Route signature settings
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0( _countof( rootparams ), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT );

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y Serialization of automatic version judgment
	result = D3DX12SerializeVersionedRootSignature( &rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob );
	// ���[�g�V�O�l�`���̐��� Route signature generation
	result = device->CreateRootSignature( 0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS( rootSignature.ReleaseAndGetAddressOf() ) );
	if ( FAILED( result ) ) { assert( 0 ); }

	gpipeline.pRootSignature = rootSignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐��� Graphics pipeline generation
	result = device->CreateGraphicsPipelineState( &gpipeline, IID_PPV_ARGS( pipelineState.ReleaseAndGetAddressOf() ) );
	if ( FAILED( result ) ) { assert( 0 ); }
}

void FbxObject3d::Initialize()
{
	HRESULT result;
	// �萔�o�b�t�@�̐��� Generate constant buffer
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ), 	// �A�b�v���[�h�\ Uploadable
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( (sizeof( ConstBufferDataTransform ) + 0xff) & ~0xff ),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &constBuffTransform ) );

	// �X�L�j���O���萔�o�b�t�@���� Skinning information constant buffer generation
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ), 	// �A�b�v���[�h�\ Uploadable
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( (sizeof( ConstBufferDataSkin ) + 0xff) & ~0xff ),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &constBuffSkin ) );

	// �萔�o�b�t�@�փf�[�^�]�� Data transfer to a constant buffer
	ConstBufferDataSkin *constMapSkin = nullptr;
	result = constBuffSkin->Map( 0, nullptr, (void **)&constMapSkin );
	for ( int i = 0; i < MAX_BONES; i++ )
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap( 0, nullptr );

	// 1�t���[�����̎��Ԃ�60FPS�Őݒ� Set the time for one frame at 60 FPS
	frameTime.SetTime( 0, 0, 0, 1, 0, FbxTime::EMode::eFrames60 );
}

void FbxObject3d::Update()
{
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z Calculation of scale, rotation, translation matrix
	matScale = XMMatrixScaling( scale.x, scale.y, scale.z );
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ( XMConvertToRadians( rotation.z ) );
	matRot *= XMMatrixRotationX( XMConvertToRadians( rotation.x ) );
	matRot *= XMMatrixRotationY( XMConvertToRadians( rotation.y ) );
	matTrans = XMMatrixTranslation( position.x, position.y, position.z );

	// ���[���h�s��̍��� World matrix composition
	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g Reset transformation
	matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f Reflect scaling in world matrix
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f Reflect the rotation in the world matrix
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f Reflect translation in world matrix

	// �r���[�v���_�N�V�����s�� View production matrix
	const XMMATRIX &matViewProjection = camera->GetViewProjectionMatrix();

	// ���f���̃��b�V���g�����X�t�H�[�� Model mesh transform
	const XMMATRIX &modelTransform = fbxmodel->GetModelTransform();

	// �J�������W Camera coordinates
	const XMFLOAT3 &cameraPos = camera->GetEye();

	HRESULT result;

	// �萔�o�b�t�@�փf�[�^�]�� Data transfer to a constant buffer
	ConstBufferDataTransform *constMap = nullptr;
	result = constBuffTransform->Map( 0, nullptr, (void **)&constMap );
	if ( SUCCEEDED( result ) )
	{
		constMap->viewproj = matViewProjection;
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap( 0, nullptr );
	}

	// �{�[���z�� Bone array
	std::vector<FbxModel::Bone> &bones = fbxmodel->GetBones();

	if ( isPlay == false )
	{
		PlayAnimation();
	}

	// �A�j���[�V���� animation
	if ( isPlay )
	{
		// 1�t���[���i�߂� Advance one frame
		currentTime += frameTime;
		// �Ō�܂ōĐ�������擪�ɖ߂� After playing to the end, return to the beginning
		if ( currentTime > endTime )
		{
			currentTime = startTime;
		}
	}


	// �萔�o�b�t�@�f�[�^�]�� Constant buffer data transfer
	ConstBufferDataSkin *constMapSkin = nullptr;
	result = constBuffSkin->Map( 0, nullptr, (void **)&constMapSkin );
	for ( int i = 0; i < bones.size(); i++ )
	{
		// ���̎p���s�� Current posture matrix
		XMMATRIX matCurrentPose;
		// ���̎p���s����擾 Get the current posture matrix
		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform( currentTime );
		// XMMATRIX�ɕϊ� Convert to XMMATRIX
		FbxLoader::ConvertMatrixFromFbx( &matCurrentPose, fbxCurrentPose );
		// �������ăX�L�j���O�s��� Synthesize into a skinning matrix
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap( 0, nullptr );
}

void FbxObject3d::Draw( ID3D12GraphicsCommandList *cmdList )
{
	// ���f���̊��蓖�Ă��Ȃ���Ε`�悵�Ȃ� Do not draw without model assignment
	if ( fbxmodel == nullptr ) {
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ� Setting the pipeline state
	cmdList->SetPipelineState( pipelineState.Get() );
	// ���[�g�V�O�l�`���̐ݒ� Route signature settings
	cmdList->SetGraphicsRootSignature( rootSignature.Get() );
	// �v���~�e�B�u�`���ݒ� Set primitive shape
	cmdList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	// �萔�o�b�t�@�r���[���Z�b�g Set constant buffer view
	cmdList->SetGraphicsRootConstantBufferView( 0, constBuffTransform->GetGPUVirtualAddress() );
	// �X�L�j���O���萔�o�b�t�@�r���[���Z�b�g Set skinning information constant buffer view
	cmdList->SetGraphicsRootConstantBufferView( 2, constBuffSkin->GetGPUVirtualAddress() );

	// ���f���`�� Model drawing
	fbxmodel->Draw( cmdList );
}

void FbxObject3d::PlayAnimation()
{
	FbxScene *fbxScene = fbxmodel->GetFbxScene();
	// 0�Ԗڂ̃A�j���[�V�����擾 Get 0th animation
	FbxAnimStack *animstack = fbxScene->GetSrcObject<FbxAnimStack>( 0 );
	// �A�j���[�V�����̖��O�擾 Get the name of the animation
	const char *animstackname = animstack->GetName();
	// �A�j���[�V�����̎��ԏ�� Animation time information
	FbxTakeInfo *takeinfo = fbxScene->GetTakeInfo( animstackname );

	// �J�n���Ԏ擾 Get start time
	startTime = takeinfo->mLocalTimeSpan.GetStart();
	// �I�����Ԏ擾 Get end time
	endTime = takeinfo->mLocalTimeSpan.GetStop();
	// �J�n���Ԃɍ��킹�� Match the start time
	currentTime = startTime;
	// �Đ�����Ԃɂ��� Put it in the playing state
	isPlay = true;
}
