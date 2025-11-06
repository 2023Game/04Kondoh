#pragma once
#include <memory>

class CStateBase;

class CStateMachine : public CTask {
public:

	CStateMachine() = default;
	~CStateMachine() = default;

	// 更新処理
	void Update();
	// 状態を変更する関数
	void ChangeState(std::shared_ptr<CStateBase> newState);
	// 現在のステートのゲッタ
	std::shared_ptr<CStateBase> GetCurrentState() const;

private:
	// 現在のステート
	std::shared_ptr<CStateBase> mCurrentState;

};