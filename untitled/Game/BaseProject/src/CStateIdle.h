#pragma once
#include "CStateBase.h"

// 待機状態クラス
class CStateIdle : public CStateBase {
public:
	// コンストラクタ
	CStateIdle(CEnemyBase* owner, float time);

	// 開始処理
	void Enter() override;
	// 更新処理
	void Update() override;

private:

	float mElapsedTime;	// 経過時間
	float mIdleTime;	// 待機時間
};
