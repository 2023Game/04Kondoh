#pragma once
#include "CStateBase.h"

// 追跡状態クラス
class CStateChase : public CStateBase
{
public:
	// コンストラクタ
	CStateChase(CEnemyBase* owner, float time);

	// 開始処理
	void Enter() override;
	// 更新処理
	void Update() override;

private:

	float mElapsedTime;	// 経過時間
};
