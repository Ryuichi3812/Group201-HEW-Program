//=============================================================================
//
// メッシュ共通処理 [mesh.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
enum ePrimitiveType {
	PT_UNDEFINED = 0,
	PT_POINT,
	PT_LINE,
	PT_LINESTRIP,
	PT_TRIANGLE,
	PT_TRIANGLESTRIP,

	MAX_PRIMITIVETYPE
};
enum eTranslucentType {
	TT_NOAFFECT = 0,	// 全て
	TT_OPACITYONLY,		// 不透明のみ
	TT_TRANSLUCENTONLY,	// 半透明のみ

	MAX_TRANSLUCENTTYPE
};

//**************************************
// 構造体定義
//**************************************
typedef struct {
	XMFLOAT4	Diffuse;		// Diffuse color RGBA
	XMFLOAT4	Ambient;		// Ambient color RGB
	XMFLOAT4	Specular;		// Specular 'shininess'
	XMFLOAT4	Emissive;		// Emissive color RGB
	float		Power;			// Sharpness if specular highlight
} MATERIAL;

typedef struct {
	ID3D11Buffer* pVertexBuffer;			// 頂点バッファインターフェースへのポインタ
	ID3D11Buffer* pIndexBuffer;				// インデックスバッファインターフェースへのポインタ

	XMFLOAT4X4 mtxWorld;					// ワールドマトリックス
	XMFLOAT3 pos;							// ポリゴン表示位置の中心座標
	XMFLOAT3 rot;							// ポリゴンの回転角
	int nNumVertex;							// 総頂点数	
	int nNumIndex;							// 総インデックス数

	XMFLOAT4X4 mtxTexture;					// テクスチャ マトリックス
	ID3D11ShaderResourceView* pTexture;		// テクスチャ

	ePrimitiveType primitiveType;			// プリミティブ型

	MATERIAL* pMaterial;					// マテリアル
	float fAlpha;							// 頂点カラーα最小値
} MESH;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMesh(void);
void UninitMesh(void);
void UpdateMesh(MESH* pMesh);
void DrawMesh(ID3D11DeviceContext* pDeviceContext, MESH* pMesh, int nTranslucntType = TT_NOAFFECT);
HRESULT MakeMeshVertex(ID3D11Device* pDevice, MESH* pMesh, VERTEX_3D vertexWk[], int indexWk[]);
void ReleaseMesh(MESH* pMesh);
HRESULT UpdateMeshVertex(ID3D11DeviceContext* pDeviceContext, MESH* pMesh, VERTEX_3D vertex[]);
