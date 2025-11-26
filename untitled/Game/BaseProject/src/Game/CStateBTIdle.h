#pragma once
#include "CStateBase.h"

class CStateBTIdle : public CStateBase
{
public:
	CStateBTIdle(CEnemyBase* owner);

	void Enter() override;

	void Update() override;

private:

};