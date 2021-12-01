//=============================================================================
//
// ���E���\�� [bsphere.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBSphere(void);
void UninitBSphere(void);
void UpdateBSphere(void);
void DrawBSphere(void);
int  CreateBSphere(XMFLOAT3 pos, float radius, XMFLOAT4X4 world);
void MoveBSphere(int nBSphere, XMFLOAT4X4 world);
void ReleaseBSphere(int nBSphere);
void SetBSphereVisible(bool visible = true);
