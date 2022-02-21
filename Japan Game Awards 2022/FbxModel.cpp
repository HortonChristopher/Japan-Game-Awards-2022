#include "FbxModel.h"

void FbxModel::Draw( ID3D12GraphicsCommandList *cmdList )
{
	// ���_�o�b�t�@���Z�b�g Set vertex buffer
	cmdList->IASetVertexBuffers( 0, 1, &vbView );
	// �C���f�b�N�X�o�b�t�@���Z�b�g Set index buffer
	cmdList->IASetIndexBuffer( &ibView );

	// �f�X�N���v�^�q�[�v�̔z�� Array of descriptor heaps
	if ( descHeapSRV ) {
		ID3D12DescriptorHeap *ppHeaps[] = { descHeapSRV.Get() };
		cmdList->SetDescriptorHeaps( _countof( ppHeaps ), ppHeaps );
	}
	// �V�F�[�_���\�[�X�r���[���Z�b�g Set shader resource view
	cmdList->SetGraphicsRootDescriptorTable( 1, descHeapSRV->GetGPUDescriptorHandleForHeapStart() );

	// �`��R�}���h Drawing command
	cmdList->DrawIndexedInstanced( (UINT)indices.size(), 1, 0, 0, 0 );
}

FbxModel::~FbxModel()
{
	// FBX�V�[���̉�� Release the FBX scene
	fbxScene->Destroy();
}

void FbxModel::CreatBuffers( ID3D12Device *device )
{
	HRESULT result;

	// ���_�f�[�^�̑S�̃T�C�Y Overall size of vertex data
	UINT sizeVB = static_cast<UINT>(sizeof( VertexPosNormalUvSkin ) * vertices.size());

	// ���_�o�b�t�@���� Vertex buffer generation
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( sizeVB ),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &vertBuff ) );

	// ���_�o�b�t�@�ւ̃f�[�^�]�� Data transfer to vertex buffer
	VertexPosNormalUvSkin *vertMap = nullptr;
	result = vertBuff->Map( 0, nullptr, (void **)&vertMap );
	if ( SUCCEEDED( result ) )
	{
		std::copy( vertices.begin(), vertices.end(), vertMap );
		vertBuff->Unmap( 0, nullptr );
	}

	// ���_�o�b�t�@�r���[(VBV)�̍쐬 Creating a vertex buffer view (VBV)
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof( vertices[0] );

	// ���_�C���f�b�N�X�S�̂̃T�C�Y Overall size of vertex index
	UINT sizeIB = static_cast<UINT>(sizeof( unsigned short ) * indices.size());

	// �C���f�b�N�X�o�b�t�@���� Index buffer generation
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( sizeIB ),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &indexBuff ) );

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]�� Data transfer to index buffer
	unsigned short *indexMap = nullptr;
	result = indexBuff->Map( 0, nullptr, (void **)&indexMap );
	if ( SUCCEEDED( result ) )
	{
		std::copy( indices.begin(), indices.end(), indexMap );
		indexBuff->Unmap( 0, nullptr );
	}

	// �C���f�b�N�X�o�b�t�@�r���[(IBV)�̍쐬 Creating an index buffer view (IBV)
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	// �e�N�X�`���摜�f�[�^ Texture image data
	const DirectX::Image *img = scratchImg.GetImage( 0, 0, 0 ); // ���f�[�^���o Raw data extraction
	assert( img ); 

	// ���\�[�X�ݒ� Resource settings
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐��� Generate buffer for texture
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0 ),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w�� Specification for texture
		nullptr,
		IID_PPV_ARGS( &texBuff ) );

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]�� Data transfer to texture buffer
	result = texBuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�ւ̃R�s�[ Copy to all areas
		img->pixels, // ���f�[�^�A�h���X Original data address
		(UINT)img->rowPitch, // 1���C���T�C�Y 1 line size
		(UINT)img->slicePitch // 1���T�C�Y 1 sheet size
	);

	// SRV�p�f�X�N���v�^�q�[�v�𐶐� Generate descriptor heap for SRV
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_���猩����悤�� As visible from the shader
	descHeapDesc.NumDescriptors = 1; // �e�N�X�`������ Number of textures
	result = device->CreateDescriptorHeap( &descHeapDesc, IID_PPV_ARGS( &descHeapSRV ) ); // ���� generation

	// �V�F�[�_���\�[�X�r���[(SRV)���� Shader resource view (SRV) generation
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\���� Setting structure
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`�� 2D texture
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView( texBuff.Get(), // �r���[�Ɗ֘A�t����o�b�t�@ Buffer associated with view
		&srvDesc,// �e�N�X�`���ݒ��� Texture setting information
		descHeapSRV->GetCPUDescriptorHandleForHeapStart() // �q�[�v�̐擪�A�h���X Heap start address
	);
}
