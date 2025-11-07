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
	// 前のステートを終了
	if (mCurrentState) return mCurrentState->OnEnd();
	// 新しいステートを代入
	mCurrentState = newState;
	// 新しいステートを開始
	if (mCurrentState) return mCurrentState->OnStart();

}

std::shared_ptr<CStateBase> CStateMachine::GetCurrentState() const
{
	return mCurrentState;
}
