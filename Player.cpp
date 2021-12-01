//================================================================================
// プレイヤー
//--------------------------------------------------------------------------------
// Player.cpp
//--------------------------------------------------------------------------------
// 2021 11/29 SatoKen コメント追加
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "Player.h"
#include "input.h"

#include "debugproc.h"


//********************************************************************************
// マクロ定義
//********************************************************************************
#define PLAYER_PLANE		"data/model/Hikouki.X"

#define	VALUE_MOVE_PLAYER	(0.50f)			// 移動速度
#define	RATE_MOVE_PLAYER	(0.20f)			// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(9.0f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.20f)			// 回転慣性係数

#define RADIUS_PLAYER		(3.0f)			// モデル半径


//********************************************************************************
// グローバル変数
//********************************************************************************
static XMFLOAT4X4	g_mtxWorld;			// ワールドマトリックス
static XMFLOAT3		g_retPos;			// 位置を返す用

//================================================================================
// コンストラクタ
//================================================================================
Player::Player()
{
}

//================================================================================
// デストラクタ
//================================================================================
Player::~Player()
{
}

//================================================================================
// 初期化処理
//================================================================================
HRESULT Player::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	// 位置・回転・スケールの初期設定
	m_posPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_movePlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotDestPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// モデルデータの読み込み
	if (!m_modelPlayer.Load(pDevice, pDeviceContext, PLAYER_PLANE))
	{
		MessageBoxA(GetMainWnd(), "モデルデータ読み込みエラー", "InitPlayer", MB_OK);
		return E_FAIL;
	}

	return hr;
}


//=============================================================================
// 終了処理
//=============================================================================
void Player::Uninit()
{
	// モデルの解放
	m_modelPlayer.Release();
}


//=============================================================================
// 更新処理
//=============================================================================
void Player::Update()
{
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (GetKeyPress(VK_LEFT))
	{
		if (GetKeyPress(VK_UP)) 
		{
			// 左前移動
			m_movePlayer.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y + 135.0f;
		}
		else if (GetKeyPress(VK_DOWN))
		{
			// 左後移動
			m_movePlayer.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y + 45.0f;
		}
		else 
		{
			// 左移動
			m_movePlayer.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y + 90.0f;
		}
	}
	else if (GetKeyPress(VK_RIGHT)) 
	{
		if (GetKeyPress(VK_UP))
		{
			// 右前移動
			m_movePlayer.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y - 135.0f;
		}
		else if (GetKeyPress(VK_DOWN))
		{
			// 右後移動
			m_movePlayer.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y - 45.0f;
		}
		else
		{
			// 右移動
			m_movePlayer.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			m_movePlayer.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;

			m_rotDestPlayer.y = rotCamera.y - 90.0f;
		}
	}
	else if (GetKeyPress(VK_UP))
	{
		// 前移動
		m_movePlayer.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
		m_movePlayer.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;

		m_rotDestPlayer.y = 180.0f + rotCamera.y;
	}
	else if (GetKeyPress(VK_DOWN)) 
	{
		// 後移動
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
		// 左回転
		m_rotDestPlayer.y -= VALUE_ROTATE_PLAYER;
		if (m_rotDestPlayer.y < -180.0f) {
			m_rotDestPlayer.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_RSHIFT))
	{
		// 右回転
		m_rotDestPlayer.y += VALUE_ROTATE_PLAYER;
		if (m_rotDestPlayer.y >= 180.0f) {
			m_rotDestPlayer.y -= 360.0f;
		}
	}

	// 目的の角度までの差分
	float fDiffRotY = m_rotDestPlayer.y - m_rotPlayer.y;
	if (fDiffRotY >= 180.0f)
	{
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f)
	{
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	m_rotPlayer.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (m_rotPlayer.y >= 180.0f) 
	{
		m_rotPlayer.y -= 360.0f;
	}
	if (m_rotPlayer.y < -180.0f)
	{
		m_rotPlayer.y += 360.0f;
	}

	/// 位置移動
	m_posPlayer.x += m_movePlayer.x;
	m_posPlayer.y += m_movePlayer.y;
	m_posPlayer.z += m_movePlayer.z;

	// 移動量に慣性をかける
	m_movePlayer.x += (0.0f - m_movePlayer.x) * RATE_MOVE_PLAYER;
	m_movePlayer.y += (0.0f - m_movePlayer.y) * RATE_MOVE_PLAYER;
	m_movePlayer.z += (0.0f - m_movePlayer.z) * RATE_MOVE_PLAYER;

	// フィールド範囲チェック
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
		// リセット
		m_posPlayer = XMFLOAT3(0.0f, 20.0f, 0.0f);
		m_movePlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_rotDestPlayer = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	g_retPos = m_posPlayer;
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotPlayer.x),
		XMConvertToRadians(m_rotPlayer.y), XMConvertToRadians(m_rotPlayer.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(m_posPlayer.x, m_posPlayer.y, m_posPlayer.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	//PrintDebugProc("[ﾋｺｳｷ ｲﾁ : (%f : %f : %f)]\n", m_posPlayer.x, m_posPlayer.y, m_posPlayer.z);
	//PrintDebugProc("[ﾋｺｳｷ ﾑｷ : (%f) < ﾓｸﾃｷ ｲﾁ:(%f) >]\n", m_rotPlayer.y, m_rotDestPlayer.y);
	//PrintDebugProc("\n");

	PrintDebugProc("*** ﾋｺｳｷ ｿｳｻ ***\n");
	PrintDebugProc("ﾏｴ   ｲﾄﾞｳ : \x1e\n");//↑
	PrintDebugProc("ｳｼﾛ  ｲﾄﾞｳ : \x1f\n");//↓
	PrintDebugProc("ﾋﾀﾞﾘ ｲﾄﾞｳ : \x1d\n");//←
	PrintDebugProc("ﾐｷﾞ  ｲﾄﾞｳ : \x1c\n");//→
	PrintDebugProc("ｼﾞｮｳｼｮｳ   : I\n");
	PrintDebugProc("ｶｺｳ       : K\n");
	PrintDebugProc("ﾋﾀﾞﾘ ｾﾝｶｲ : LSHIFT\n");
	PrintDebugProc("ﾐｷﾞ  ｾﾝｶｲ : RSHIFT\n");
	PrintDebugProc("\n");
}


//=============================================================================
// 描画処理
//=============================================================================
void Player::Draw()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	m_modelPlayer.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// 半透明部分を描画
	SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	SetZWrite(false);				// Zバッファ更新しない
	m_modelPlayer.Draw(pDC, g_mtxWorld, eTransparentOnly);
	SetZWrite(true);				// Zバッファ更新する
	SetBlendState(BS_NONE);			// アルファブレンド無効
}


//=============================================================================
// 位置取得
//=============================================================================
XMFLOAT3& GetPlayerPos()
{
	return g_retPos;
}
