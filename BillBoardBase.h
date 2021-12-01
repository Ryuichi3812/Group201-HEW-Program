//================================================================================
// �r���{�[�h�x�[�X
//--------------------------------------------------------------------------------
// BillBordBase.h
//--------------------------------------------------------------------------------
// ����
//================================================================================
#pragma once

//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "main.h"

#include "Camera.h"
#include "Texture.h"
#include "mesh.h"
#include "Light.h"


//********************************************************************************
// �萔�E�}�N����`
//********************************************************************************
#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER				(1.0f)
#define M_AMBIENT			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)


//********************************************************************************
// �N���X�E�\���̒�`
//********************************************************************************
class BillBoardBase
{
public:
	BillBoardBase();
	~BillBoardBase();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:

};
