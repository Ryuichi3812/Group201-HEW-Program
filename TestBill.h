//================================================================================
// �r���{�[�h�e�X�g
//--------------------------------------------------------------------------------
// TestBill.h
//--------------------------------------------------------------------------------
// ����
//================================================================================
#pragma once

//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "BillBoardBase.h"
#include <vector>	//���I�z����g�p

//********************************************************************************
// �N���X�E�\���̒�`
//********************************************************************************
struct TTestBill
{
	XMFLOAT2	m_size;			// �T�C�Y
	XMFLOAT3	m_pos;			// �ʒu
	XMFLOAT4	m_col;			// �F
	bool		m_use;			// �g�p���Ă��邩�ǂ���
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