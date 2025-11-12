#pragma once
#include "CStateBase.h"

class CStateMachine
{
public:
	// コンストラクタ
	CStateMachine();
	// デストラクタ
	~CStateMachine();

	// 更新処理
	void Update();

	// 状態の登録
	void RegisterState(int index, CStateBase* state);
	// 状態を変更する関数
	void ChangeState(int index);

#if _DEBUG
	// デバッグ表示
	void DebugRender();
#endif

private:
	// 現在の状態
	CStateBase* mpCuttent;
	// 登録してある状態リスト
	std::vector<CStateBase*> mStates;
};