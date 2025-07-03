#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CSound;
class CGameOverUI;

class CGameOverScene : public CSceneBase
{
public:
	// コンストラクタ
	CGameOverScene();
	// デストラクタ
	~CGameOverScene();
	// シーン読み込み
	void Load();
	// シーンの更新処理
	void Update();

private:
	CGameOverUI* mpGameOverUI;
};