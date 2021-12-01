//================================================================================
// �^�C�g���V�[��
//--------------------------------------------------------------------------------
// TitleScene.cpp
//--------------------------------------------------------------------------------
// 2021 11/29 SatoKen �R�����g�ǉ�
//================================================================================


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "TitleScene.h"
#include "fade.h"

#include "input.h"
#include "Sound.h"


//================================================================================
// �R���X�g���N�^
//================================================================================
TitleScene::TitleScene()
	:m_pBg(nullptr),m_pTest(nullptr)
{
	// �I�u�W�F�N�g����
	m_pBg = new Bg();
	m_pTest = new Test();
}

//================================================================================
// �f�X�g���N�^
//================================================================================
TitleScene::~TitleScene()
{
	// �I�u�W�F�N�g���
	if (m_pBg != nullptr)
	{
		delete m_pBg;
		m_pBg = nullptr;
	}

	if (m_pTest != nullptr)
	{
		delete m_pTest;
		m_pTest = nullptr;
	}
}

//================================================================================
// ������
//================================================================================
HRESULT TitleScene::Init()
{
	HRESULT hr = S_OK;

	// �w�i������
	m_pBg->Init();

	m_pTest->Init();

	// BGM�Đ�
	CSound::Play(BGM_TITLE);

	return hr;
}


//================================================================================
// �I��
//================================================================================
void TitleScene::Uninit()
{
	// �w�i�I������
	m_pBg->Uninit();

	m_pTest->Uninit();

	// BGM�X�g�b�v
	CSound::Stop(BGM_TITLE);
}


//================================================================================
// �X�V
//================================================================================
void TitleScene::Update()
{
	// �w�i�X�V
	m_pBg->Update();

	m_pTest->Update();

	// �t�F�[�h�A�E�g�J�n
	if (GetKeyTrigger(VK_RETURN))
	{
		StartFadeOut();
		CSound::Play(SE_DECIDE);
	}

	// �t�F�[�h�C�����Ɏ��̃V�[����
	if (GetFade() == FADE_IN)
		SelectScene(SCENE_GAME);

}

//================================================================================
// �`��
//================================================================================
void TitleScene::Draw()
{
	// �w�i�`��
	m_pBg->Draw();

	m_pTest->Draw();
}