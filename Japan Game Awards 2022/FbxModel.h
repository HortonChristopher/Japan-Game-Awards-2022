#pragma once

#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

struct Node
{
	// ���O name
	std::string name;

	// ���[�J���X�P�[�� Local scale
	DirectX::XMVECTOR scaling = { 1,1,1,0 };

	// ���[�J����]�p Local angle of rotation
	DirectX::XMVECTOR rotation = { 0,0,0,0 };

	// ���[�J���ړ� Local move
	DirectX::XMVECTOR translation = { 0,0,0,1 };

	// ���[�J���ό`�s�� Local transformation matrix
	DirectX::XMMATRIX transform;

	// �O���[�o���ό`�s�� Global transformation matrix
	DirectX::XMMATRIX globalTransform;

	// �e�m�[�h Parent node
	Node *parent = nullptr;
};



class FbxModel
{
private: // �G�C���A�X alias
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	// std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public: // �萔 constant
// �{�[���C���f�b�N�X�̍ő吔 Maximum number of bone indexes
	static const int MAX_BONE_INDICES = 4;

public: // �T�u�N���X Subclass
// ���_�f�[�^�\���� Vertex data structure
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; // xyz���W xyz coordinates
		DirectX::XMFLOAT3 normal; // �@���x�N�g��Normal vector
		DirectX::XMFLOAT2 uv; // uv���W uv coordinates
		UINT boneIndex[MAX_BONE_INDICES]; // �{�[�� �ԍ� Bone number
		float boneWeight[MAX_BONE_INDICES]; // �{�[�� �d�� Bone weight
	};

public:
	// �t�����h�N���X Friend class
	friend class FbxLoader;

	struct Bone
	{
		// ���O name
		std::string name;

		// �����p���̋t�s�� Inverse matrix of initial posture
		DirectX::XMMATRIX invInitialPose;

		// �N���X�^�[(FBX���̃{�[�����) Cluster (bone information on the FBX side)
		FbxCluster *fbxCluster;

		// �R���X�g���N�^ constructor
		Bone( const std::string &name )
		{
			this->name = name;
		}
	};

public:
	// �`�� drawing
	void Draw( ID3D12GraphicsCommandList *cmdList );

private: // �����o�ϐ� Member variables
	// ���f���� Model name
	std::string name;

	// �m�[�h�z�� Node array
	std::vector<Node> nodes;
	// ���b�V�������m�[�h Node with mesh
	Node *meshNode = nullptr;
	// ���_�o�b�t�@Vertex buffer
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@ Index buffer
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@ Texture buffer
	ComPtr<ID3D12Resource> texBuff;
	// ���_�o�b�t�@�r���[ Vertex buffer view
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[ Index buffer view
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV�p�f�X�N���v�^�q�[�v Descriptor heap for SRV
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// ���_�f�[�^�z�� Vertex data array
	std::vector<VertexPosNormalUvSkin> vertices;
	// ���_�C���f�b�N�X�z�� Vertex index array
	std::vector<unsigned short> indices;

	// �A���r�G���g�W�� Ambient coefficient
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// �f�B�t���[�Y�W�� Diffuse coefficient
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	// �e�N�X�`�����^�f�[�^ Texture metadata
	DirectX::TexMetadata metadata = {};
	// �X�N���b�`�C���[�W Scratch image
	DirectX::ScratchImage scratchImg = {};

	// �{�[���z�� Bone array
	std::vector<Bone> bones;

	// FBX�V�[�� FBX scene
	FbxScene *fbxScene = nullptr;

public:
	// �o�b�t�@���� Buffer generation
	void CreatBuffers( ID3D12Device *device );

	// ���f���̕ό`�s��擾 Get model transformation matrix
	const XMMATRIX &GetModelTransform() { return meshNode->globalTransform; }

	// getter
	std::vector<Bone> &GetBones() { return bones; }

	// getter
	FbxScene *GetFbxScene() { return fbxScene; }

	// �f�X�g���N�^ Destructor
	~FbxModel();
};

