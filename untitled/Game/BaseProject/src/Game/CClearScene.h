#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CGameClear;

class CClearScene : public CSceneBase
{
public:
	// コンストラクタ
	CClearScene();
	// デストラクタ
	~CClearScene();
	// シーン読み込み
	void Load();
	// シーンの更新処理
	void Update();

private:
	CGameClear* mpClearUI;

};
