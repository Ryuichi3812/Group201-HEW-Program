//=============================================================================
//
// メッシュ共通処理 [mesh.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define M_DIFFUSE		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR		XMFLOAT4(0.1f,0.1f,0.1f,1.0f)
#define M_POWER			(1.0f)
#define M_AMBIENT		XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE		XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// シェーダに渡す値
struct SHADER_GLOBAL {
	XMMATRIX	mWVP;		// ワールド×ビュー×射影行列(転置行列)
	XMMATRIX	mW;			// ワールド行列(転置行列)
	XMMATRIX	mTex;		// テクスチャ行列(転置行列)
};
struct SHADER_GLOBAL2 {
	XMVECTOR	vEye;		// 視点座標
	// 光源
	XMVECTOR	vLightDir;	// 光源方向
	XMVECTOR	vLa;		// 光源色(アンビエント)
	XMVECTOR	vLd;		// 光源色(ディフューズ)
	XMVECTOR	vLs;		// 光源色(スペキュラ)
	// マテリアル
	XMVECTOR	vAmbient;	// アンビエント色(+テクスチャ有無)
	XMVECTOR	vDiffuse;	// ディフューズ色
	XMVECTOR	vSpecular;	// スペキュラ色(+スペキュラ強度)
	XMVECTOR	vEmissive;	// エミッシブ色
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer*				g_pConstantBuffer[2];	// 定数バッファ
static ID3D11SamplerState*			g_pSamplerState;		// テクスチャ サンプラ
static ID3D11VertexShader*			g_pVertexShader;		// 頂点シェーダ
static ID3D11InputLayout*			g_pInputLayout;			// 頂点フォーマット
static ID3D11PixelShader*			g_pPixelShader;			// ピクセルシェーダ

static MATERIAL						g_material;				// マテリアル

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMesh(void)
{
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = LoadShader("Vertex", "Pixel", &g_pVertexShader, &g_pInputLayout, &g_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[0]);
	if (FAILED(hr)) return hr;
	bd.ByteWidth = sizeof(SHADER_GLOBAL2);
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer[1]);
	if (FAILED(hr)) return hr;

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &g_pSamplerState);
	if (FAILED(hr)) {
		return hr;
	}

	// マテリアルの初期設定
	g_material.Diffuse = M_DIFFUSE;
	g_material.Ambient = M_AMBIENT;
	g_material.Specular = M_SPECULAR;
	g_material.Power = M_POWER;
	g_material.Emissive = M_EMISSIVE;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMesh(void)
{
	// テクスチャ サンプラの開放
	SAFE_RELEASE(g_pSamplerState);
	// 定数バッファの解放
	for (int i = 0; i < _countof(g_pConstantBuffer); ++i) {
		SAFE_RELEASE(g_pConstantBuffer[i]);
	}
	// ピクセルシェーダ解放
	SAFE_RELEASE(g_pPixelShader);
	// 頂点フォーマット解放
	SAFE_RELEASE(g_pInputLayout);
	// 頂点シェーダ解放
	SAFE_RELEASE(g_pVertexShader);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMesh(MESH* pMesh)
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	if (!pMesh) return;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pMesh->rot.x),
		XMConvertToRadians(pMesh->rot.y), XMConvertToRadians(pMesh->rot.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	XMStoreFloat4x4(&pMesh->mtxWorld, mtxWorld);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMesh(ID3D11DeviceContext* pDeviceContext, MESH* pMesh, int nTranslucntType)
{
	MATERIAL* pMaterial = pMesh->pMaterial;
	if (!pMaterial) pMaterial = &g_material;
	switch (nTranslucntType) {
	case TT_OPACITYONLY:		// 不透明のみ
		if (pMaterial->Diffuse.w < 1.0f || pMesh->fAlpha < 1.0f) {
			return;
		}
		break;
	case TT_TRANSLUCENTONLY:	// 半透明のみ
		if (pMaterial->Diffuse.w >= 1.0f && pMesh->fAlpha >= 1.0f) {
			return;
		}
		break;
	default:					// 不透明度で区別しない
		break;
	}
	// シェーダ設定
	pDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(g_pInputLayout);

	// 頂点バッファをセット
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pMesh->pVertexBuffer, &stride, &offset);
	// インデックスバッファをセット
	pDeviceContext->IASetIndexBuffer(pMesh->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &pMesh->pTexture);

	SHADER_GLOBAL cb;
	XMMATRIX mtxWorld = XMLoadFloat4x4(&pMesh->mtxWorld);
	CCamera* pCamera = CCamera::Get();
	cb.mWVP = XMMatrixTranspose(mtxWorld * XMLoadFloat4x4(&pCamera->GetViewMatrix()) * XMLoadFloat4x4(&pCamera->GetProjMatrix()));
	cb.mW = XMMatrixTranspose(mtxWorld);
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&pMesh->mtxTexture));
	pDeviceContext->UpdateSubresource(g_pConstantBuffer[0], 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer[0]);
	SHADER_GLOBAL2 cb2;
	cb2.vEye = XMLoadFloat3(&pCamera->GetPos());
	CLight* pLight = CLight::Get();
	cb2.vLightDir = XMLoadFloat3(&pLight->GetDir());
	cb2.vLa = XMLoadFloat4(&pLight->GetAmbient());
	cb2.vLd = XMLoadFloat4(&pLight->GetDiffuse());
	cb2.vLs = XMLoadFloat4(&pLight->GetSpecular());
	cb2.vDiffuse = XMLoadFloat4(&pMaterial->Diffuse);
	cb2.vAmbient = XMVectorSet(pMaterial->Ambient.x, pMaterial->Ambient.y, pMaterial->Ambient.z, (pMesh->pTexture != nullptr) ? 1.f : 0.f);
	cb2.vSpecular = XMVectorSet(pMaterial->Specular.x, pMaterial->Specular.y, pMaterial->Specular.z, pMaterial->Power);
	cb2.vEmissive = XMLoadFloat4(&pMaterial->Emissive);
	pDeviceContext->UpdateSubresource(g_pConstantBuffer[1], 0, nullptr, &cb2, 0, 0);
	pDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffer[1]);

	// プリミティブ形状をセット
	static const D3D11_PRIMITIVE_TOPOLOGY pt[] = {
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,	// 0なら三角形ストリップ
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	};
	pDeviceContext->IASetPrimitiveTopology(pt[pMesh->primitiveType]);

	// ポリゴンの描画
	pDeviceContext->DrawIndexed(pMesh->nNumIndex, 0, 0);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeMeshVertex(ID3D11Device* pDevice, MESH* pMesh, VERTEX_3D vertexWk[], int indexWk[])
{
	pMesh->fAlpha = 1.0f;
	for (int i = 0; i < pMesh->nNumVertex; ++i) {
		if (pMesh->fAlpha > vertexWk[i].diffuse.w) {
			pMesh->fAlpha = vertexWk[i].diffuse.w;
		}
	}

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(VERTEX_3D) * pMesh->nNumVertex;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertexWk;
	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &pMesh->pVertexBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	CD3D11_BUFFER_DESC ibd(pMesh->nNumIndex * sizeof(int), D3D11_BIND_INDEX_BUFFER);
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = indexWk;
	hr = pDevice->CreateBuffer(&ibd, &initData, &pMesh->pIndexBuffer);

	return hr;
}

//=============================================================================
// メッシュ解放
//=============================================================================
void ReleaseMesh(MESH* pMesh)
{
	if (!pMesh) return;
	// テクスチャ解放
	SAFE_RELEASE(pMesh->pTexture);
	// 頂点バッファ解放
	SAFE_RELEASE(pMesh->pVertexBuffer);
	// インデックス バッファ解放
	SAFE_RELEASE(pMesh->pIndexBuffer);
}

//=============================================================================
// 頂点バッファ更新
//=============================================================================
HRESULT UpdateMeshVertex(ID3D11DeviceContext* pDeviceContext, MESH* pMesh, VERTEX_3D vertex[])
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	D3D11_MAPPED_SUBRESOURCE msr;
	HRESULT hr = pDeviceContext->Map(pMesh->pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(hr)) {
		// 頂点データを上書き
		rsize_t len = sizeof(VERTEX_3D) * pMesh->nNumVertex;
		memcpy_s(msr.pData, len, vertex, len);
		// 頂点データをアンロックする
		pDeviceContext->Unmap(pMesh->pVertexBuffer, 0);
	}
	return hr;
}
