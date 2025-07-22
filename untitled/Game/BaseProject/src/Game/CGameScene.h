#pragma once
#include "CSceneBase.h"

class CGameMenu;
class CGameClear;
class CSound;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	// ランダムリスポーン
	void RandomRespawn();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();



private:
	float mElapsedTime; // 経過時間計測用

	int mSpawnCount;	// 敵のリスポーン数
	float mRespawnElapsedTime;	//リスポーン経過時間計測用

	CSound* mpGameBGM;
	CGameMenu* mpGameMenu;
	CGameClear* mpGameClear;

};
