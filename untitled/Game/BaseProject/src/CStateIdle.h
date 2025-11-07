#pragma once
#include "CStateBase.h"

class CEnemyBase;

class CStateIdle : public CStateBase {
public:
	// コンストラクタ
	CStateIdle(CEnemyBase* owner);
	// 
	void OnStart() override;

	void OnUpdate() override;

	void OnEnd() override;
};
