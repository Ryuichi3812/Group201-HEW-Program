//================================================================================
// ビルボードテスト
//--------------------------------------------------------------------------------
// TestBill.cpp
//--------------------------------------------------------------------------------
// 履歴
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "TestBill.h"


//********************************************************************************
// 定数・マクロ定義
//********************************************************************************
#define	TEXTURE_TESTBILL	L"data/texture/bullet001.png"	// 読み込むテクスチャファイル名
#define MAX_TESTBILL		(100)
#define TESTBILL_RADIUS		(5.0f)
#define COLISION_TESTBILL	XMFLOAT3(3.0f,3.0f,3.0f)

//********************************************************************************
// グローバル変数
//********************************************************************************
static MESH				g_mesh;					// メッシュ情報
static MATERIAL			g_material;				// マテリアル情報


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTestBill(ID3D11Device* pDevice);


//================================================================================
// コンストラクタ
//================================================================================
TestBill::TestBill()
{
}


//================================================================================
// デストラクタ
//================================================================================
TestBill::~TestBill()
{
}


//================================================================================
// 初期化
//================================================================================
HRESULT TestBill::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	//配列の大きさを設定
	m_TestBill.resize(MAX_TESTBILL);		

	// 頂点情報の作成
	hr = MakeVertexTestBill(pDevice);
	if (FAILED(hr))
	{
		return hr;
	}

	// マテリアルの設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;
	g_mesh.pMaterial = &g_material;


	// テクスチャの読み込み
	hr = CreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_TESTBILL,	// ファイルの名前
								&g_mesh.pTexture);	// 読み込むメモリ
	if (FAILED(hr))
	{
		return hr;
	}
	XMStoreFloat4x4(&g_mesh.mtxTexture, XMMatrixIdentity());

	// ワールドマトリックス初期化
	XMStoreFloat4x4(&g_mesh.mtxWorld, XMMatrixIdentity());

	// 情報初期化
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
// 終了
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
	// メッシュの開放
	ReleaseMesh(&g_mesh);
}


//================================================================================
// 更新
//================================================================================
void TestBill::Update()
{
	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		// 未使用ならスキップ
		if (m_TestBill[cntTestBill].m_use)
		{
			continue;
		}
	}
}

//================================================================================
// 描画
//================================================================================
void TestBill::Draw()
{
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxScale, mtxTranslate;

	CLight::Get()->SetDisable();	// 光源無効
	SetBlendState(BS_ALPHABLEND);	// αブレンディング有効

	// ビューマトリックスを取得
	XMFLOAT4X4& mtxView = CCamera::Get()->GetViewMatrix();

	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		// 未使用ならスキップ
		if (!m_TestBill[cntTestBill].m_use)
		{
			continue;
		}

		// ワールド マトリックス初期化
		mtxWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// 回転を反映
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

		// スケールを反映 (回転より先に反映)
		mtxScale = XMMatrixScaling(m_TestBill[cntTestBill].m_size.x, m_TestBill[cntTestBill].m_size.y, 1.0f);
		mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(m_TestBill[cntTestBill].m_pos.x, m_TestBill[cntTestBill].m_pos.y, m_TestBill[cntTestBill].m_pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		XMStoreFloat4x4(&g_mesh.mtxWorld, mtxWorld);

		// 色の設定
		g_material.Diffuse = m_TestBill[cntTestBill].m_col;

		// ポリゴンの描画
		DrawMesh(pDeviceContext, &g_mesh);
	}
	SetBlendState(BS_NONE);		// αブレンディング無効
	CLight::Get()->SetEnable();	// 光源有効
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexTestBill(ID3D11Device* pDevice)
{
	// 一時的な頂点配列を生成
	g_mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[g_mesh.nNumVertex];

	// 頂点座標の設定
	pVertexWk[0].vtx = XMFLOAT3(-TESTBILL_RADIUS, TESTBILL_RADIUS, 0.0f);
	pVertexWk[1].vtx = XMFLOAT3(TESTBILL_RADIUS, TESTBILL_RADIUS, 0.0f);
	pVertexWk[2].vtx = XMFLOAT3(-TESTBILL_RADIUS, -TESTBILL_RADIUS, 0.0f);
	pVertexWk[3].vtx = XMFLOAT3(TESTBILL_RADIUS, -TESTBILL_RADIUS, 0.0f);

	// 法線の設定
	pVertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 反射光の設定
	pVertexWk[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVertexWk[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	// 一時的なインデックス配列を生成
	g_mesh.nNumIndex = 4;
	int* pIndexWk = new int[g_mesh.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;

	// 頂点バッファ/インデックスバッファ生成
	HRESULT hr = MakeMeshVertex(pDevice, &g_mesh, pVertexWk, pIndexWk);

	// 一時的な頂点配列/インデックス配列を解放
	delete[] pIndexWk;
	delete[] pVertexWk;

	return hr;
}

//=============================================================================
// ビルボード生成
//=============================================================================
int TestBill::SetTestBill(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 col)
{
	int idxTree = -1;

	for (int cntTestBill = 0; cntTestBill < MAX_TESTBILL; ++cntTestBill)
	{
		// 使用中ならスキップ
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