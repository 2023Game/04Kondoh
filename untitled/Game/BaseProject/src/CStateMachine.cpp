#include "CStateMachine.h"
#include "CStateBase.h"

void CStateMachine::Update()
{
	// 現在のステートが nullptrじゃ無かったら
	// ステートを更新する
	if (mCurrentState) return mCurrentState->OnUpdate();
}

void CStateMachine::ChangeState(std::shared_ptr<CStateBase> newState)
{
	// 現在のステートが nullptrじゃ無かったら
	if (mCurrentState) return mCurrentState->OnExit();


}

std::shared_ptr<CStateBase> CStateMachine::GetCurrentState() const
{
	return mCurrentState;
}
