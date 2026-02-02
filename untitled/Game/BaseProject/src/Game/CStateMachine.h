#pragma once
#include "CStateBase.h"

class CStateMachine
{
public:
	// コンストラクタ
	CStateMachine();
	// デストラクタ
	~CStateMachine();

	// 持ち主を設定
	void SetOwner(CEnemyBase* owner);

	// 更新処理
	void Update();

	// 現在のステートを取得
	CStateBase* GetCurrentState() const;

	// 状態の登録
	void RegisterState(int index, CStateBase* state);
	// 攻撃状態の登録
	void RegisterAttackState(int index, CStateBase* state);
	// 状態を変更する関数
	void ChangeState(int index);
	// 攻撃状態を変更する関数
	void ChangeAttack(int index);

#if _DEBUG
	// デバッグ表示
	void DebugRender();
#endif

private:
	// このステートマシンの持ち主
	CEnemyBase* mpOwner;

	// 現在の状態
	CStateBase* mpCurrent;
	// 現在の攻撃状態
	CStateBase* mpAttackCurrent;
	// 登録してある状態リスト
	std::vector<CStateBase*> mStates;
	// 登録してある攻撃状態リスト
	std::vector<CStateBase*> mAttackStates;
};