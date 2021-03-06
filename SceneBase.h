#ifndef __SCENE_BASE_H__
#define __SCENE_BASE_H__
//================================================================================
// シーンベース
//--------------------------------------------------------------------------------
// SceneBase.h
//--------------------------------------------------------------------------------
// 履歴
//================================================================================


//********************************************************************************
// インクルード部
//********************************************************************************
#include "main.h"
#include "SceneManager.h"


//********************************************************************************
// クラス・構造体定義
//********************************************************************************
class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:

};

#endif // !__SCENE_BASE_H__

