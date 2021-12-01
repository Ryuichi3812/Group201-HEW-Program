//================================================================================
// フィールド
//--------------------------------------------------------------------------------
// Field.h
//--------------------------------------------------------------------------------
// 履歴
//================================================================================
#pragma once


//********************************************************************************
// インクルード部
//********************************************************************************
#include "main.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//********************************************************************************
// クラス・構造体定義
//********************************************************************************
class FieldBase
{
public:
	FieldBase();
	~FieldBase();

	virtual HRESULT Init(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	virtual HRESULT MakeVertexField(ID3D11Device* pDevice,
									int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
									float fTexSizeX, float fTexSizeZ);

private:
	ID3D11ShaderResourceView*	m_pTexture;				// テクスチャ
	MESH						m_mesh;					// 構造体
	MATERIAL					m_material;				// マテリアル

};

