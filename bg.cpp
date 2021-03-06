//================================================================================
// 背景
//--------------------------------------------------------------------------------
// bg.cpp
//--------------------------------------------------------------------------------
// 2021 11/29 SatoKen コメント追加
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "bg.h"
#include "polygon.h"
#include "Texture.h"
#include "SceneManager.h"


//********************************************************************************
// 定数・マクロ定義
//********************************************************************************
#define POS_X_BG			(0.0f)			// 背景座標_X　（中心）
#define POS_Y_BG			(0.0f)			// 　　　　_Y　（中心）

#define WIDTH_BG			(SCREEN_WIDTH)	// 背景幅	（画面幅）
#define HEIGHT_BG			(SCREEN_HEIGHT)	// 背景高さ	（画面高さ）
#define ANGLE_BG			(0)				// 背景角度
#define COLOR_R_BG			(1.0f)			// 背景色
#define COLOR_G_BG			(1.0f)
#define COLOR_B_BG			(1.0f)

#define COUNT_X_BG			(1)				// 背景コマ数
#define COUNT_Y_BG			(1)

#define PATH_TEXTURE_BG_TITLE	(L"data/texture/Bg_01.jpg")		// タイトル背景
#define PATH_TEXTURE_BG_GAME	(L"data/texture/Bg_00.png")		// ゲーム背景

#define BG_TITLE	(SCENE_TITLE)
#define BG_GAME		(SCENE_GAME)
#define MAX_BG		(SCENE_MAX)


//********************************************************************************
// グローバル変数
//********************************************************************************
static ID3D11ShaderResourceView* g_pTexture;	// テクスチャ情報
static LPCWSTR g_pszTexture[MAX_BG] =			// 背景選択用
{
	PATH_TEXTURE_BG_TITLE,
	PATH_TEXTURE_BG_GAME
};

static SCENE_TYPE g_Type;	// 背景タイプ（シーンタイプによって変わる）


//================================================================================
// コンストラクタ
//================================================================================
Bg::Bg()
{

}

//================================================================================
// デストラクタ
//================================================================================
Bg::~Bg()
{
}

//================================================================================
// 初期化
//================================================================================
HRESULT Bg::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// シーンタイプ取得
	g_Type = GetSceneType();
	hr = CreateTextureFromFile(pDevice, g_pszTexture[g_Type], &g_pTexture);

	return hr;
}


//================================================================================
// 終了
//================================================================================
void Bg::Uninit()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}


//================================================================================
// 更新
//================================================================================
void Bg::Update()
{
	
}

//================================================================================
// 描画
//================================================================================
void Bg::Draw()
{
	// Zバッファ無効(Zチェック無&Z更新無)
	SetZBuffer(false);

	ID3D11DeviceContext* pDC = GetDeviceContext();

	SetPolygonSize(WIDTH_BG, HEIGHT_BG);							// 表示サイズ
	SetPolygonPos(POS_X_BG, POS_Y_BG);								// 表示位置
	SetPolygonAngle(ANGLE_BG);										// 角度
	SetPolygonColor(COLOR_R_BG, COLOR_G_BG, COLOR_B_BG);			// 色決定
	SetPolygonTexture(g_pTexture);									// 表示画像
	SetPolygonFrameSize(COUNT_X_BG, COUNT_Y_BG);					// 画像の中の枠１つ当たりのサイズ
	SetPolygonUV(0.0f, 0.0f);										// 表示画像の枠位置決定
	DrawPolygon(pDC);

	// 元に戻す
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonAngle(0);

	// Zバッファ有効(Zチェック有&Z更新有)
	SetZBuffer(true);

}