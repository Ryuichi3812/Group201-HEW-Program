//================================================================================
// フェードアウト・イン
//--------------------------------------------------------------------------------
// fade.cpp
//--------------------------------------------------------------------------------
// 2021 11/29 SatoKen コメント追加
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "fade.h"
#include "polygon.h"

#include "Sound.h"


//********************************************************************************
// 定数・マクロ定義
//********************************************************************************
#define FADE_RATE	0.02f	//フェードイン・アウトの刻み


//********************************************************************************
// グローバル変数
//********************************************************************************
static float	g_fRed		= 0.0f;		//フェードアウト色
static float	g_fGreen	= 0.0f;
static float	g_fBlue		= 0.0f;

static float	g_fAlpha	= 0.0f;		//不透明度
static EFade	g_eFade = FADE_NONE;	//状態


//================================================================================
// 初期化
//================================================================================
HRESULT InitFade()
{
	g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
	return S_OK;
}


//================================================================================
// 終了
//================================================================================
void UninitFade()
{
	g_fAlpha = 0.0f;
	g_eFade = FADE_NONE;
}


//================================================================================
// 更新
//================================================================================
void UpdateFade()
{
	switch (g_eFade)
	{
	case FADE_NONE:
		break;

	case FADE_OUT:
		g_fAlpha += FADE_RATE;	//不透明度をます
		if (g_fAlpha >= 1.0f)
		{
			//フェードイン処理に切り替え
			g_fAlpha = 1.0f;
			g_eFade = FADE_IN;
		}
		//ボリュームもフェードアウト
		CSound::SetVolume(1.0f - g_fAlpha);
		break;

	case FADE_IN:
		g_fAlpha -= FADE_RATE;	//透明度をます
		if (g_fAlpha <= 0.0f)
		{
			//フェードイン終了
			g_fAlpha = 0.0f;
			g_eFade = FADE_NONE;
		}
		//ボリュームもフェードイン
		CSound::SetVolume(1.0f - FADE_RATE);
		break;

	default:
		break;
	}

}


//================================================================================
// 描画
//================================================================================
void DrawFade()
{
	SetBlendState(BS_ALPHABLEND);	// αブレンディング有効

	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	//画面全体に半透明ポリゴンを描画
	SetPolygonPos(0.0f, 0.0f);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonTexture(nullptr);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonColor(g_fRed, g_fGreen, g_fBlue);
	SetPolygonAlpha(g_fAlpha);
	DrawPolygon(GetDeviceContext());

	//元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);
	SetPolygonAlpha(1.0f);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

	SetBlendState(BS_NONE);		// αブレンディング無効
}


//================================================================================
// フェードアウト開始
//================================================================================
void StartFadeOut()
{
	if (g_eFade != FADE_OUT)
	{
		g_eFade = FADE_OUT;
		g_fAlpha = 0.0f;
	}
}


//================================================================================
// 状態取得
//================================================================================
EFade GetFade()
{
	return g_eFade;
}


//================================================================================
// フェードアウト色設定
//================================================================================
void SetfadeColor(float fR, float fG, float fB)
{
	g_fRed = fR;
	g_fGreen = fG;
	g_fBlue = fB;
}