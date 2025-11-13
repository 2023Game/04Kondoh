#pragma once
#include "CStateBase.h"

class CStatePatrol : public CStateBase
{
public:
	CStatePatrol(CEnemyBase* owner);

	void Enter() override;

	void Update() override;

private:

	int mStateStep;	// ステート
};