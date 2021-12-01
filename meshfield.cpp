//=============================================================================
//
// メッシュ地面処理 [meshfield.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "meshfield.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/field001.jpg"	// テクスチャファイル名

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

#define	VALUE_MOVE			(5.0f)							// 移動量
#define	VALUE_ROTATE		(0.2f)							// 回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static HRESULT MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11ShaderResourceView*	g_pTexture;				// テクスチャ
static MESH							g_mesh;					// 構造体
static MATERIAL						g_material;				// マテリアル

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ,
	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// 位置・回転の初期設定
	g_mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;

	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_mesh.pTexture);
	if (FAILED(hr))
		return hr;
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// 頂点情報の作成
	hr = MakeVertexField(pDevice, nNumBlockX, nNumBlockZ,
		fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	// メッシュ解放
	ReleaseMesh(&g_mesh);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{
	// メッシュ更新
	UpdateMesh(&g_mesh);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// メッシュの描画
	DrawMesh(pDC, &g_mesh);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ)
{
	// プリミティブ種別設定
	g_mesh.primitiveType = PT_TRIANGLESTRIP;

	// 頂点数の設定
	g_mesh.nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定(縮退ポリゴン用を考慮する)
	g_mesh.nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// 頂点配列の作成
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// インデックス配列の作成
	int* pIndexWk = new int[g_mesh.nNumIndex];

	// 頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	for (int z = 0; z < nNumBlockZ + 1; ++z) {
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			// 頂点座標の設定
			pVtx->vtx.x = x * fSizeBlockX - (nNumBlockX * 0.5f) * fSizeBlockX;
			pVtx->vtx.y = 0.0f;
			pVtx->vtx.z = -z * fSizeBlockZ + (nNumBlockZ * 0.5f) * fSizeBlockZ;

			// 法線の設定
			pVtx->nor = XMFLOAT3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//インデックス配列の中身を埋める
	int* pIdx = pIndexWk;
	for (int z = 0; z < nNumBlockZ; ++z) {
		if (z > 0) {
			// 縮退ポリゴンのためのダブりの設定
			*pIdx++ = (z + 1) * (nNumBlockX + 1);
		}
		for (int x = 0; x < nNumBlockX + 1; ++x) {
			*pIdx++ = (z + 1) * (nNumBlockX + 1) + x;
			*pIdx++ = z * (nNumBlockX + 1) + x;
		}
		if (z < nNumBlockZ - 1) {
			// 縮退ポリゴンのためのダブりの設定
			*pIdx++ = z * (nNumBlockX + 1) + nNumBlockX;
		}
	}

	// 頂点バッファ/インデックス バッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pVertexWk;
	delete[] pIndexWk;

	return hr;
}
