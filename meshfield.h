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


//********************************************************************************
// クラス・構造体定義
//********************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ, float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
