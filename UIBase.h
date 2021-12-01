#ifndef __UI_BASE__
#define __UI_BASE__
//================================================================================
// UIベース
//--------------------------------------------------------------------------------
// UIBase.h
//--------------------------------------------------------------------------------
// 履歴
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "main.h"
#include "polygon.h"
#include "Texture.h"


//********************************************************************************
// クラス・構造体定義
//********************************************************************************
class UIBase
{
public:
	UIBase();
	~UIBase();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:

};


#endif // !__UI_BASE__
