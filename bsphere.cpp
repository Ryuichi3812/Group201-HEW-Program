//=============================================================================
//
// ���E���\�� [bsphere.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "bsphere.h"
#include "mesh.h"
#include "Texture.h"
#include "Light.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define M_DIFFUSE		XMFLOAT4(0.5f,1.0f,0.5f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BSHPERE		(5000)

//*****************************************************************************
// �\����
//*****************************************************************************
struct TBsphere {
	bool		use;		// �g�p���Ă��邩�ǂ���
	XMFLOAT3	pos;		// ���S���W(���f�����W�n)
	XMFLOAT4X4	world;		// ���[���h �}�g���b�N�X
	float		radius;		// ���a
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static MESH		g_mesh;					// ���b�V���\����
static MATERIAL	g_material;				// �}�e���A��
static TBsphere	g_bsphere[MAX_BSHPERE];	// ���E�����
static bool		g_visible;				// �\��ON/OFF

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static HRESULT MakeVertexBSphere(ID3D11Device* pDevice);

//=============================================================================
// ������
//=============================================================================
HRESULT InitBSphere(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// �}�e���A���̏����ݒ�
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// ���_���̍쐬
	hr = MakeVertexBSphere(pDevice);

	// �\��ON
	g_visible = true;

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBSphere(void)
{
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// �X�V
//=============================================================================
void UpdateBSphere(void)
{
	if (GetKeyTrigger(VK_B))
		g_visible = !g_visible;

	PrintDebugProc("*** �������� ***\n");
	PrintDebugProc("ON/OFF : B\n");
	PrintDebugProc("\n");
}

//=============================================================================
// �`��
//=============================================================================
void DrawBSphere(void)
{
	if (!g_visible) return;
	ID3D11DeviceContext* pDC = GetDeviceContext();
	CLight::Get()->SetDisable();
	SetBlendState(BS_ADDITIVE);	// ���Z����
	SetZWrite(false);	// �������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)
	TBsphere* pBSphere = g_bsphere;
	for (int i = 0; i < MAX_BSHPERE; ++i, ++pBSphere) {
		if (!pBSphere->use) continue;
		// ���S���W���ړ�
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, XMVector3TransformCoord(
			XMLoadFloat3(&pBSphere->pos),
			XMLoadFloat4x4(&pBSphere->world)));
		// �T�C�Y�𔽉f
		g_mesh.mtxWorld._11 = g_mesh.mtxWorld._22 =
			g_mesh.mtxWorld._33 = pBSphere->radius;
		// �ʒu�𔽉f
		g_mesh.mtxWorld._41 = pos.x;
		g_mesh.mtxWorld._42 = pos.y;
		g_mesh.mtxWorld._43 = pos.z;
		// �`��
		DrawMesh(pDC, &g_mesh);
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

//=============================================================================
// ����
//=============================================================================
int CreateBSphere(XMFLOAT3 pos, float radius, XMFLOAT4X4 world)
{
	TBsphere* pBSphere = g_bsphere;
	for (int i = 0; i < MAX_BSHPERE; ++i, ++pBSphere) {
		if (pBSphere->use) continue;
		pBSphere->pos = pos;
		pBSphere->radius = radius;
		pBSphere->world = world;
		pBSphere->use = true;
		return i;
	}
	return -1;
}

//=============================================================================
// �ړ�
//=============================================================================
void MoveBSphere(int nBSphere, XMFLOAT4X4 world)
{
	if (nBSphere < 0 || nBSphere >= MAX_BSHPERE)
		return;
	g_bsphere[nBSphere].world = world;
}

//=============================================================================
// ���
//=============================================================================
void ReleaseBSphere(int nBSphere)
{
	if (nBSphere < 0 || nBSphere >= MAX_BSHPERE)
		return;
	g_bsphere[nBSphere].use = false;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexBSphere(ID3D11Device* pDevice)
{
	// �v���~�e�B�u�̎w��
	g_mesh.primitiveType = PT_LINE;

	// �I�u�W�F�N�g�̒��_�z��𐶐�
	g_mesh.nNumVertex = 384 * 3;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// ���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	for (int axis = 0; axis < 3; ++axis) {
		for (int i = 0; i < 384; ++i, ++pVtx) {
			// ���_���W�̐ݒ�
			switch (axis) {
			case 0:
				pVtx->vtx.x = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.y = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.z = 0.0f;
				break;
			case 1:
				pVtx->vtx.y = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.z = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.x = 0.0f;
				break;
			case 2:
				pVtx->vtx.z = sinf(i / 384.0f * XM_2PI);
				pVtx->vtx.x = cosf(i / 384.0f * XM_2PI);
				pVtx->vtx.y = 0.0f;
				break;
			}

			// �@���̐ݒ�
			pVtx->nor = pVtx->vtx;

			// ���ˌ��̐ݒ�
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex = XMFLOAT2(0.0f, 0.0f);
		}
	}

	// �C���f�b�N�X�z��𐶐�
	g_mesh.nNumIndex = (384 * 2) * 3;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// �C���f�b�N�X�z��̒��g�𖄂߂�
	for (int axis = 0; axis < 3; ++axis) {
		int* pIdx = &pIndexWk[axis * (384 * 2)];
		for (int i = 0; i < 384; ++i) {
			*pIdx++ = axis * 384 + i;
			*pIdx++ = axis * 384 + (i + 1) % 384;
		}
	}

	// ���_�o�b�t�@����
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// �ꎞ�z��̉��
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// �S�̕\��ON/OFF
//=============================================================================
void SetBSphereVisible(bool visible)
{
	g_visible = visible;
}
