#include "CStateMachine.h"
#include "CStateBase.h"

void CStateMachine::Update()
{
}

void CStateMachine::ChangeState(std::shared_ptr<CStateBase> newState)
{
}

std::shared_ptr<CStateBase> CStateMachine::GetCurrentState() const
{
	return mCurrentState;
}
