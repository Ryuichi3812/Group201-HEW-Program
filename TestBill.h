//================================================================================
// ビルボードテスト
//--------------------------------------------------------------------------------
// TestBill.h
//--------------------------------------------------------------------------------
// 履歴
//================================================================================
#pragma once

//********************************************************************************
// インクルード部
//********************************************************************************
#include "BillBoardBase.h"
#include <vector>	//動的配列を使用

//********************************************************************************
// クラス・構造体定義
//********************************************************************************
struct TTestBill
{
	XMFLOAT2	m_size;			// サイズ
	XMFLOAT3	m_pos;			// 位置
	XMFLOAT4	m_col;			// 色
	bool		m_use;			// 使用しているかどうか
};


class TestBill:public BillBoardBase
{
public:
	TestBill();
	~TestBill();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	int SetTestBill(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

private:
	std::vector<TTestBill> m_TestBill;
};