//================================================================================
// �r���{�[�h�e�X�g
//--------------------------------------------------------------------------------
// TestBill.cpp
//--------------------------------------------------------------------------------
// ����
//================================================================================


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "TestBill.h"


//********************************************************************************
// �萔�E�}�N����`
//********************************************************************************
#define	TEXTURE_TESTBILL	L"data/texture/bullet001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MAX_TESTBILL		(100)
#define TESTBILL_RADIUS		(5.0f)
#define COLISION_TESTBILL	XMFLOAT3(3.0f,3.0f,3.0f)

//********************************************************************************
// �O���[�o���ϐ�
//********************************************************************************
static MESH				g_mesh;					// ���b�V�����
static MATERIAL			g_material;				// �}�e���A�����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexTestBill(ID3D11Device* pDevice);


//================================================================================
// �R���X�g���N�^
//================================================================================
TestBill::TestBill()
{
}


//================================================================================
// �f�X�g���N�^
//================================================================================
TestBill::~TestBill()
{
}


//================================================================================
// ������
//================================================================================
HRESULT TestBill::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	//�z��̑傫����ݒ�
	m_TestBill.resize(MAX_TESTBILL);		

	// ���_���̍쐬
	hr = MakeVertexTestBill(pDevice);
	if (FAILED(hr))
	{
		return hr;
	}

	// �}�e���A���̐ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;


	// �e�N�X�`���̓ǂݍ���
	hr = CreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_TESTBILL,	// �t�@�C���̖��O
								&g_mesh.pTexture);	// �ǂݍ��ރ�����
	if (FAILED(hr))
	{
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// ��񏉊���
	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		m_TestBill[cntTestBill].m_size = XMFLOAT2(0.0f, 0.0f);
		m_TestBill[cntTestBill].m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_TestBill[cntTestBill].m_col = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		m_TestBill[cntTestBill].m_use = false;
	}
	return hr;
}


//================================================================================
// �I��
//================================================================================
void TestBill::Uninit()
{
	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		if (m_TestBill[cntTestBill].m_use)
		{
			m_TestBill[cntTestBill].m_use = false;
		}
	}
	// ���b�V���̊J��
	ReleaseMesh(&g_mesh);
}


//================================================================================
// �X�V
//================================================================================
void TestBill::Update()
{
	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		// ���g�p�Ȃ�X�L�b�v
		if (m_TestBill[cntTestBill].m_use)
		{
			continue;
		}
	}
}

//================================================================================
// �`��
//================================================================================
void TestBill::Draw()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	CLight::Get()->SetDisable();	// ��������
	SetBlendState(BS_ALPHABLEND);	// ���u�����f�B���O�L��

	// �r���[�}�g���b�N�X���擾
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		// ���g�p�Ȃ�X�L�b�v
		if (!m_TestBill[cntTestBill].m_use)
		{
			continue;
		}

		// ���[���h �}�g���b�N�X������
		mtxWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// ��]�𔽉f
		g_mesh.mtxWorld._11 = mtxView._11;
		g_mesh.mtxWorld._12 = mtxView._21;
		g_mesh.mtxWorld._13 = mtxView._31;
		g_mesh.mtxWorld._21 = mtxView._12;
		g_mesh.mtxWorld._22 = mtxView._22;
		g_mesh.mtxWorld._23 = mtxView._32;
		g_mesh.mtxWorld._31 = mtxView._13;
		g_mesh.mtxWorld._32 = mtxView._23;
		g_mesh.mtxWorld._33 = mtxView._33;
		mtxWorld = XMLoadFloat4x4(&g_mesh.mtxWorld);

		// �X�P�[���𔽉f (��]����ɔ��f)
		mtxScale = XMMatrixScaling(m_TestBill[cntTestBill].m_size.x, m_TestBill[cntTestBill].m_size.y, 1.0f);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(m_TestBill[cntTestBill].m_pos.x, m_TestBill[cntTestBill].m_pos.y, m_TestBill[cntTestBill].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// �F�̐ݒ�
		g_material.Diffuse = m_TestBill[cntTestBill].m_col;

		// �|���S���̕`��
		DrawMesh(pDeviceContext, &g_mesh);
	}
	SetBlendState(BS_NONE);		// ���u�����f�B���O����
	CLight::Get()->SetEnable();	// �����L��
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexTestBill(ID3D11Device* pDevice)
{
	// �ꎞ�I�Ȓ��_�z��𐶐�
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_���W�̐ݒ�
	pVertexWk[0].vtx = XMFLOAT3(-TESTBILL_RADIUS, TESTBILL_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(TESTBILL_RADIUS, TESTBILL_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-TESTBILL_RADIUS, -TESTBILL_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(TESTBILL_RADIUS, -TESTBILL_RADIUS, 0.0f);

	// �@���̐ݒ�
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// �ꎞ�I�ȃC���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�I�Ȓ��_�z��/�C���f�b�N�X�z������
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// �r���{�[�h����
//=============================================================================
int TestBill::SetTestBill(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 col)
{
	int idxTree = -1;

	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		// �g�p���Ȃ�X�L�b�v
		if (m_TestBill[cntTestBill].m_use)
		{
			continue;
		}

		m_TestBill[cntTestBill].m_use = true;
		m_TestBill[cntTestBill].m_pos = pos;
		m_TestBill[cntTestBill].m_size = size;
		m_TestBill[cntTestBill].m_col = col;
		idxTree = cntTestBill;
		break;
	}

	return idxTree;
}