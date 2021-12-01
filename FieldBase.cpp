//================================================================================
// �t�B�[���h�x�[�X
//--------------------------------------------------------------------------------
// FieldBase.cpp
//--------------------------------------------------------------------------------
// ����
//================================================================================


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "FieldBase.h"


#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)


//================================================================================
// �R���X�g���N�^
//================================================================================
FieldBase::FieldBase()
{

}

//================================================================================
// �f�X�g���N�^
//================================================================================
FieldBase::~FieldBase()
{

}

//================================================================================
// ������
//================================================================================
HRESULT FieldBase::Init(int nNumBlockX, int nNumBlockZ,
	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// �ʒu�E��]�̏����ݒ�
	m_mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// �}�e���A���̏����ݒ�
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	m_mesh.pMaterial = &m_material;

	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());

	// ���_���̍쐬
	hr = MakeVertexField(pDevice, nNumBlockX, nNumBlockZ,
		fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);

	return hr;
}


//================================================================================
// �I��
//================================================================================
void FieldBase::Uninit()
{

}

//================================================================================
// �X�V
//================================================================================
void FieldBase::Update()
{

}


//================================================================================
// �`��
//================================================================================
void FieldBase::Draw()
{

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT  FieldBase::MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ)
{
	// �v���~�e�B�u��ʐݒ�
	m_mesh.primitiveType = PT_TRIANGLESTRIP;

	// ���_���̐ݒ�
	m_mesh.nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
	m_mesh.nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// ���_�z��̍쐬
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh.nNumVertex];

	// �C���f�b�N�X�z��̍쐬
	int* pIndexWk = new int[m_mesh.nNumIndex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	for (int z = 0; z < nNumBlockZ + 1; ++z) {
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			// ���_���W�̐ݒ�
			pVtx->vtx.x = x * fSizeBlockX - (nNumBlockX * 0.5f) * fSizeBlockX;
			pVtx->vtx.y = 0.0f;
			pVtx->vtx.z = -z * fSizeBlockZ + (nNumBlockZ * 0.5f) * fSizeBlockZ;

			// �@���̐ݒ�
			pVtx->nor = XMFLOAT3(0.0f, 1.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//�C���f�b�N�X�z��̒��g�𖄂߂�
	int* pIdx = pIndexWk;
	for (int z = 0; z < nNumBlockZ; ++z) {
		if (z > 0) {
			// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			*pIdx++ = (z + 1) * (nNumBlockX + 1);
		}
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			*pIdx++ = (z + 1) * (nNumBlockX + 1) + x;
			*pIdx++ = z * (nNumBlockX + 1) + x;
		}
		if (z < nNumBlockZ - 1) {
			// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
			*pIdx++ = z * (nNumBlockX + 1) + nNumBlockX;
		}
	}

	// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &m_mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pVertexWk;
	delete[] pIndexWk;

	return hr;
}