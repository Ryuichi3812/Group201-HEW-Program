//================================================================================
// �w�i
//--------------------------------------------------------------------------------
// bg.cpp
//--------------------------------------------------------------------------------
// 2021 11/29 SatoKen �R�����g�ǉ�
//================================================================================


//********************************************************************************
// �C���N���[�h��
//********************************************************************************
#include "bg.h"
#include "polygon.h"
#include "Texture.h"
#include "SceneManager.h"


//********************************************************************************
// �萔�E�}�N����`
//********************************************************************************
#define POS_X_BG			(0.0f)			// �w�i���W_X�@�i���S�j
#define POS_Y_BG			(0.0f)			// �@�@�@�@_Y�@�i���S�j

#define WIDTH_BG			(SCREEN_WIDTH)	// �w�i��	�i��ʕ��j
#define HEIGHT_BG			(SCREEN_HEIGHT)	// �w�i����	�i��ʍ����j
#define ANGLE_BG			(0)				// �w�i�p�x
#define COLOR_R_BG			(1.0f)			// �w�i�F
#define COLOR_G_BG			(1.0f)
#define COLOR_B_BG			(1.0f)

#define COUNT_X_BG			(1)				// �w�i�R�}��
#define COUNT_Y_BG			(1)

#define PATH_TEXTURE_BG_TITLE	(L"data/texture/Bg_01.jpg")		// �^�C�g���w�i
#define PATH_TEXTURE_BG_GAME	(L"data/texture/Bg_00.png")		// �Q�[���w�i

#define BG_TITLE	(SCENE_TITLE)
#define BG_GAME		(SCENE_GAME)
#define MAX_BG		(SCENE_MAX)


//********************************************************************************
// �O���[�o���ϐ�
//********************************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// �e�N�X�`�����
static LPCWSTR g_pszTexture[MAX_BG] =			// �w�i�I��p
{
	PATH_TEXTURE_BG_TITLE,
	PATH_TEXTURE_BG_GAME
};

static SCENE_TYPE g_Type;	// �w�i�^�C�v�i�V�[���^�C�v�ɂ���ĕς��j


//================================================================================
// �R���X�g���N�^
//================================================================================
Bg::Bg()
{

}

//================================================================================
// �f�X�g���N�^
//================================================================================
Bg::~Bg()
{
}

//================================================================================
// ������
//================================================================================
HRESULT Bg::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �V�[���^�C�v�擾
	g_Type = GetSceneType();
	hr = CreateTextureFromFile(pDevice, g_pszTexture[g_Type], &g_pTexture);

	return hr;
}


//================================================================================
// �I��
//================================================================================
void Bg::Uninit()
{
	// �e�N�X�`�����
	SAFE_RELEASE(g_pTexture);
}


//================================================================================
// �X�V
//================================================================================
void Bg::Update()
{
	
}

//================================================================================
// �`��
//================================================================================
void Bg::Draw()
{
	// Z�o�b�t�@����(Z�`�F�b�N��&Z�X�V��)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetPolygonSize(WIDTH_BG, HEIGHT_BG);							// �\���T�C�Y
	SetPolygonPos(POS_X_BG, POS_Y_BG);								// �\���ʒu
	SetPolygonAngle(ANGLE_BG);										// �p�x
	SetPolygonColor(COLOR_R_BG, COLOR_G_BG, COLOR_B_BG);			// �F����
	SetPolygonTexture(g_pTexture);									// �\���摜
	SetPolygonFrameSize(COUNT_X_BG, COUNT_Y_BG);					// �摜�̒��̘g�P������̃T�C�Y
	SetPolygonUV(0.0f, 0.0f);										// �\���摜�̘g�ʒu����
	DrawPolygon(pDC);

	// ���ɖ߂�
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonAngle(0);

	// Z�o�b�t�@�L��(Z�`�F�b�N�L&Z�X�V�L)
	SetZBuffer(true);

}