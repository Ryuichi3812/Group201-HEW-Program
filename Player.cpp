//================================================================================
// �v���C���[
//--------------------------------------------------------------------------------
// Player.cpp
//--------------------------------------------------------------------------------
// 2021 11/29 SatoKen �R�����g�ǉ�
//================================================================================


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "Player.h"
#include "input.h"

#include "debugproc.h"


//********************************************************************************
// �}�N����`
//********************************************************************************
#define PLAYER_PLANE		"data/model/Hikouki.X"

#define	VALUE_MOVE_PLAYER	(0.50f)			// �ړ����x
#define	RATE_MOVE_PLAYER	(0.20f)			// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(9.0f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.20f)			// ��]�����W��

#define RADIUS_PLAYER		(3.0f)			// ���f�����a


//********************************************************************************
// �O���[�o���ϐ�
//********************************************************************************
static XMFLOAT4X4	g_mtxWorld;			// ���[���h�}�g���b�N�X
static XMFLOAT3		g_retPos;			// �ʒu��Ԃ��p

//================================================================================
// �R���X�g���N�^
//================================================================================
Player::Player()
{
}

//================================================================================
// �f�X�g���N�^
//================================================================================
Player::~Player()
{
}

//================================================================================
// ����������
//================================================================================
HRESULT Player::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	m_posPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_movePlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDestPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// ���f���f�[�^�̓ǂݍ���
	if (!m_modelPlayer.Load(pDevice, pDeviceContext, PLAYER_PLANE))
	{
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitPlayer", MB_OK);
		return E_FAIL;
	}

	return hr;
}


//=============================================================================
// �I������
//=============================================================================
void Player::Uninit()
{
	// ���f���̉��
	m_modelPlayer.Release();
}


//=============================================================================
// �X�V����
//=============================================================================
void Player::Update()
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (GetKeyPress(VK_LEFT))
	{
		if (GetKeyPress(VK_UP)) 
		{
			// ���O�ړ�
			m_movePlayer.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y + 135.0f;
		}
		else if (GetKeyPress(VK_DOWN))
		{
			// ����ړ�
			m_movePlayer.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y + 45.0f;
		}
		else 
		{
			// ���ړ�
			m_movePlayer.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y + 90.0f;
		}
	}
	else if (GetKeyPress(VK_RIGHT)) 
	{
		if (GetKeyPress(VK_UP))
		{
			// �E�O�ړ�
			m_movePlayer.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y - 135.0f;
		}
		else if (GetKeyPress(VK_DOWN))
		{
			// �E��ړ�
			m_movePlayer.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y - 45.0f;
		}
		else
		{
			// �E�ړ�
			m_movePlayer.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y - 90.0f;
		}
	}
	else if (GetKeyPress(VK_UP))
	{
		// �O�ړ�
		m_movePlayer.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
		m_movePlayer.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;

		m_rotDestPlayer.y = 180.0f + rotCamera.y;
	}
	else if (GetKeyPress(VK_DOWN)) 
	{
		// ��ړ�
		m_movePlayer.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
		m_movePlayer.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;

		m_rotDestPlayer.y = rotCamera.y;
	}

	if (GetKeyPress(VK_I))
	{
		m_movePlayer.y += VALUE_MOVE_PLAYER;
	}
	if (GetKeyPress(VK_K))
	{
		m_movePlayer.y -= VALUE_MOVE_PLAYER;
	}

	if (GetKeyPress(VK_LSHIFT))
	{
		// ����]
		m_rotDestPlayer.y -= VALUE_ROTATE_PLAYER;
		if (m_rotDestPlayer.y < -180.0f) {
			m_rotDestPlayer.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_RSHIFT))
	{
		// �E��]
		m_rotDestPlayer.y += VALUE_ROTATE_PLAYER;
		if (m_rotDestPlayer.y >= 180.0f) {
			m_rotDestPlayer.y -= 360.0f;
		}
	}

	// �ړI�̊p�x�܂ł̍���
	float fDiffRotY = m_rotDestPlayer.y - m_rotPlayer.y;
	if (fDiffRotY >= 180.0f)
	{
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f)
	{
		fDiffRotY += 360.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	m_rotPlayer.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (m_rotPlayer.y >= 180.0f) 
	{
		m_rotPlayer.y -= 360.0f;
	}
	if (m_rotPlayer.y < -180.0f)
	{
		m_rotPlayer.y += 360.0f;
	}

	/// �ʒu�ړ�
	m_posPlayer.x += m_movePlayer.x;
	m_posPlayer.y += m_movePlayer.y;
	m_posPlayer.z += m_movePlayer.z;

	// �ړ��ʂɊ�����������
	m_movePlayer.x += (0.0f - m_movePlayer.x) * RATE_MOVE_PLAYER;
	m_movePlayer.y += (0.0f - m_movePlayer.y) * RATE_MOVE_PLAYER;
	m_movePlayer.z += (0.0f - m_movePlayer.z) * RATE_MOVE_PLAYER;

	// �t�B�[���h�͈̓`�F�b�N
	if (m_posPlayer.x < -310.0f)
	{
		m_posPlayer.x = -310.0f;
	}
	if (m_posPlayer.x > 310.0f)
	{
		m_posPlayer.x = 310.0f;
	}
	if (m_posPlayer.z < -310.0f)
	{
		m_posPlayer.z = -310.0f;
	}
	if (m_posPlayer.z > 310.0f)
	{
		m_posPlayer.z = 310.0f;
	}
	if (m_posPlayer.y < 0.0f)
	{
		m_posPlayer.y = 0.0f;
	}
	if (m_posPlayer.y > 80.0f)
	{
		m_posPlayer.y = 80.0f;
	}

	if (GetKeyPress(VK_SPACE))
	{
		m_movePlayer.y += VALUE_MOVE_PLAYER;
	}

	if (GetKeyPress(VK_RETURN))
	{
		// ���Z�b�g
		m_posPlayer = XMFLOAT3(0.0f, 20.0f, 0.0f);
		m_movePlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotDestPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	g_retPos = m_posPlayer;
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotPlayer.x),
		XMConvertToRadians(m_rotPlayer.y), XMConvertToRadians(m_rotPlayer.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(m_posPlayer.x, m_posPlayer.y, m_posPlayer.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	//PrintDebugProc("[˺�� �� : (%f : %f : %f)]\n", m_posPlayer.x, m_posPlayer.y, m_posPlayer.z);
	//PrintDebugProc("[˺�� ѷ : (%f) < Ӹ÷ ��:(%f) >]\n", m_rotPlayer.y, m_rotDestPlayer.y);
	//PrintDebugProc("\n");

	PrintDebugProc("*** ˺�� ��� ***\n");
	PrintDebugProc("ϴ   ��޳ : \x1e\n");//��
	PrintDebugProc("���  ��޳ : \x1f\n");//��
	PrintDebugProc("���� ��޳ : \x1d\n");//��
	PrintDebugProc("з�  ��޳ : \x1c\n");//��
	PrintDebugProc("�ޮ����   : I\n");
	PrintDebugProc("���       : K\n");
	PrintDebugProc("���� �ݶ� : LSHIFT\n");
	PrintDebugProc("з�  �ݶ� : RSHIFT\n");
	PrintDebugProc("\n");
}


//=============================================================================
// �`�揈��
//=============================================================================
void Player::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// �s����������`��
	m_modelPlayer.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// ������������`��
	SetBlendState(BS_ALPHABLEND);	// �A���t�@�u�����h�L��
	SetZWrite(false);				// Z�o�b�t�@�X�V���Ȃ�
	m_modelPlayer.Draw(pDC, g_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Z�o�b�t�@�X�V����
	SetBlendState(BS_NONE);			// �A���t�@�u�����h����
}


//=============================================================================
// �ʒu�擾
//=============================================================================
XMFLOAT3& GetPlayerPos()
{
	return g_retPos;
}
