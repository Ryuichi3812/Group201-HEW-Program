//================================================================================
// フィールドベース
//--------------------------------------------------------------------------------
// FieldBase.cpp
//--------------------------------------------------------------------------------
// 履歴
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "FieldBase.h"


#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(50.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)


//================================================================================
// コンストラクタ
//================================================================================
FieldBase::FieldBase()
{

}

//================================================================================
// デストラクタ
//================================================================================
FieldBase::~FieldBase()
{

}

//================================================================================
// 初期化
//================================================================================
HRESULT FieldBase::Init(int nNumBlockX, int nNumBlockZ,
	float fSizeBlockX, float fSizeBlockZ, float fTexSizeX, float fTexSizeZ)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// 位置・回転の初期設定
	m_mesh.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_mesh.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// マテリアルの初期設定
	m_material.Diffuse = M_DIFFUSE;
	m_material.Ambient = M_AMBIENT;
	m_material.Specular = M_SPECULAR;
	m_material.Power = M_POWER;
	m_material.Emissive = M_EMISSIVE;
	m_mesh.pMaterial = &m_material;

	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());

	// 頂点情報の作成
	hr = MakeVertexField(pDevice, nNumBlockX, nNumBlockZ,
		fSizeBlockX, fSizeBlockZ, fTexSizeX, fTexSizeZ);

	return hr;
}


//================================================================================
// 終了
//================================================================================
void FieldBase::Uninit()
{

}

//================================================================================
// 更新
//================================================================================
void FieldBase::Update()
{

}


//================================================================================
// 描画
//================================================================================
void FieldBase::Draw()
{

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT  FieldBase::MakeVertexField(ID3D11Device* pDevice,
	int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
	float fTexSizeX, float fTexSizeZ)
{
	// プリミティブ種別設定
	m_mesh.primitiveType = PT_TRIANGLESTRIP;

	// 頂点数の設定
	m_mesh.nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// インデックス数の設定(縮退ポリゴン用を考慮する)
	m_mesh.nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// 頂点配列の作成
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh.nNumVertex];

	// インデックス配列の作成
	int* pIndexWk = new int[m_mesh.nNumIndex];

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
	HRESULT hr = MakeMeshVertex(pDevice, &m_mesh, pVertexWk, pIndexWk);

	// 一時配列の解放
	delete[] pVertexWk;
	delete[] pIndexWk;

	return hr;
}