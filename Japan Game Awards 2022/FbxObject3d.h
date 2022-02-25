#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "FbxModel.h"
#include "Camera.h"

class FbxObject3d
{
protected: // �G�C���A�X alias
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �萔 constant
// �{�[���̍ő吔 Maximum number of bones
	static const int MAX_BONES = 32;

public: // �T�u�N���X Subclass
// �萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p) Data structure for constant buffer (for coordinate transformation matrix)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	// �萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O) Data structure for constant buffer (skinning)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};


public: // �ÓI�����o�֐� Static member function
// setter
	static void SetDevice( ID3D12Device *device ) { FbxObject3d::device = device; }
	static void SetCamera( Camera *camera ) { FbxObject3d::camera = camera; }

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐��� Generate graphic pipeline
	/// </summary>
	static void CreateGraphicsPipeline();

private: // �ÓI�����o�ϐ� Static member variables
	static ID3D12Device *device;

	// �J���� camera
	static Camera *camera;

	// ���[�g�V�O�l�`�� Route signature
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g Pipeline state object
	static ComPtr<ID3D12PipelineState> pipelineState;

	// �萔�o�b�t�@(�X�L��) Constant buffer (skin)
	ComPtr<ID3D12Resource> constBuffSkin;

public: // �����o�֐� Member function
	// ������ Initialization
	void Initialize();

	// ���t���[������ Every frame processing
	void Update();
	
	// ���f���̃Z�b�g Set model
	void SetModel( FbxModel *fbxmodel ) { this->fbxmodel = fbxmodel; }
	
	// �`�� drawing
	void Draw( ID3D12GraphicsCommandList *cmdList );
	
	// ���W�̎擾 Get coordinates
	const XMFLOAT3 &GetPosition() { return position; }
	
	// ���W�̐ݒ� Coordinate setting
	void SetPosition( XMFLOAT3 position ) { this->position = position; }

	void SetRotation( XMFLOAT3 rotation ) { this->rotation = rotation; }
	
	// �X�P�[���̐ݒ� Scale setting
	void SetScale( XMFLOAT3 scale ) { this->scale = scale; }
	
	// �A�j���[�V�����J�n Animation start
	void PlayAnimation();

protected: // �����o�ϐ� Member variables
// �萔�o�b�t�@ Constant buffer
	ComPtr<ID3D12Resource> constBuffTransform;

	// ���[�J���X�P�[�� Local scale
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p Local rotation angle around the X, Y, Z axes
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W Local coordinates
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s�� Local world transformation matrix
	XMMATRIX matWorld;
	// ���f�� model
	FbxModel *fbxmodel = nullptr;

	// 1�t���[�� 1 frame
	FbxTime frameTime;
	// �A�j���[�V�����J�n���� Animation start time
	FbxTime startTime;
	// �A�j���[�V�����I������ Animation end time
	FbxTime endTime;
	// ���ݎ��� Current time
	FbxTime currentTime;
	// �A�j���[�V�����Đ��� Animation is playing
	bool isPlay = false;
};

