#pragma once
#include "CStateBase.h"

class CStateLost : public CStateBase
{
public:
	// コンストラクタ
	CStateLost(CEnemyBase* owner);

	// 開始処理
	void Enter()override;
	// 更新処理
	void Update() override;

};
