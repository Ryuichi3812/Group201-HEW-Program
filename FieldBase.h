//================================================================================
// �t�B�[���h
//--------------------------------------------------------------------------------
// Field.h
//--------------------------------------------------------------------------------
// ����
//================================================================================
#pragma once


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "main.h"
#include "Shader.h"
#include "Texture.h"
#include "input.h"
#include "mesh.h"

//********************************************************************************
// �N���X�E�\���̒�`
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
	ID3D11ShaderResourceView*	m_pTexture;				// �e�N�X�`��
	MESH						m_mesh;					// �\����
	MATERIAL					m_material;				// �}�e���A��

};

