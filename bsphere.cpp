//=============================================================================
//
// 境界球表示 [bsphere.cpp]
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
// マクロ定義
//*****************************************************************************
#define M_DIFFUSE		XMFLOAT4(0.5f,1.0f,0.5f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define MAX_BSHPERE		(5000)

//*****************************************************************************
// 構造体
//*****************************************************************************
struct TBsphere {
	bool		use;		// 使用しているかどうか
	XMFLOAT3	pos;		// 中心座標(モデル座標系)
	XMFLOAT4X4	world;		// ワールド マトリックス
	float		radius;		// 半径
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static MESH		g_mesh;					// メッシュ構造体
static MATERIAL	g_material;				// マテリアル
static TBsphere	g_bsphere[MAX_BSHPERE];	// 境界球情報
static bool		g_visible;				// 表示ON/OFF

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeVertexBSphere(ID3D11Device* pDevice);

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitBSphere(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr = S_OK;

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// ワールドマトリックス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// 頂点情報の作成
	hr = MakeVertexBSphere(pDevice);

	// 表示ON
	g_visible = true;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBSphere(void)
{
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新
//=============================================================================
void UpdateBSphere(void)
{
	if (GetKeyTrigger(VK_B))
		g_visible = !g_visible;

	PrintDebugProc("*** ｷｮｳｶｲｷｭｳ ***\n");
	PrintDebugProc("ON/OFF : B\n");
	PrintDebugProc("\n");
}

//=============================================================================
// 描画
//=============================================================================
void DrawBSphere(void)
{
	if (!g_visible) return;
	ID3D11DeviceContext* pDC = GetDeviceContext();
	CLight::Get()->SetDisable();
	SetBlendState(BS_ADDITIVE);	// 加算合成
	SetZWrite(false);	// 半透明描画はZバッファを更新しない(Zチェックは行う)
	TBsphere* pBSphere = g_bsphere;
	for (int i = 0; i < MAX_BSHPERE; ++i, ++pBSphere) {
		if (!pBSphere->use) continue;
		// 中心座標を移動
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, XMVector3TransformCoord(
			XMLoadFloat3(&pBSphere->pos),
			XMLoadFloat4x4(&pBSphere->world)));
		// サイズを反映
		g_mesh.mtxWorld._11 = g_mesh.mtxWorld._22 =
			g_mesh.mtxWorld._33 = pBSphere->radius;
		// 位置を反映
		g_mesh.mtxWorld._41 = pos.x;
		g_mesh.mtxWorld._42 = pos.y;
		g_mesh.mtxWorld._43 = pos.z;
		// 描画
		DrawMesh(pDC, &g_mesh);
	}
	SetZWrite(true);
	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

//=============================================================================
// 生成
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
// 移動
//=============================================================================
void MoveBSphere(int nBSphere, XMFLOAT4X4 world)
{
	if (nBSphere < 0 || nBSphere >= MAX_BSHPERE)
		return;
	g_bsphere[nBSphere].world = world;
}

//=============================================================================
// 解放
//=============================================================================
void ReleaseBSphere(int nBSphere)
{
	if (nBSphere < 0 || nBSphere >= MAX_BSHPERE)
		return;
	g_bsphere[nBSphere].use = false;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBSphere(ID3D11Device* pDevice)
{
	// プリミティブの指定
	g_mesh.primitiveType = PT_LINE;

	// オブジェクトの頂点配列を生成
	g_mesh.nNumVertex = 384 * 3;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	for (int axis = 0; axis < 3; ++axis) {
		for (int i = 0; i < 384; ++i, ++pVtx) {
			// 頂点座標の設定
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

			// 法線の設定
			pVtx->nor = pVtx->vtx;

			// 反射光の設定
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx->tex = XMFLOAT2(0.0f, 0.0f);
		}
	}

	// インデックス配列を生成
	g_mesh.nNumIndex = (384 * 2) * 3;
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// インデックス配列の中身を埋める
	for (int axis = 0; axis < 3; ++axis) {
		int* pIdx = &pIndexWk[axis * (384 * 2)];
		for (int i = 0; i < 384; ++i) {
			*pIdx++ = axis * 384 + i;
			*pIdx++ = axis * 384 + (i + 1) % 384;
		}
	}

	// 頂点バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// 全体表示ON/OFF
//=============================================================================
void SetBSphereVisible(bool visible)
{
	g_visible = visible;
}
