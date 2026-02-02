#include "CStateIdle.h"
#include "CEnemyBase.h"

CStateIdle::CStateIdle(CEnemyBase* owner, float time)
	: CStateBase(owner, "待機")
	, mIdleTime(time)
	, mElapsedTime(0.0f)
{
}

void CStateIdle::Enter()
{
	// ベースクラスの処理を呼び出す
	CStateBase::Enter();

	mpOwner->SetIsBattle(false);
	mpOwner->SetBattleTarget(nullptr);

	// 待機アニメーションを再生
	mpOwner->ChangeStateAnimation(mIndex);
	// 移動停止
	mpOwner->SetMoveSpeed(CVector::zero);
	// 経過時間初期化
	mElapsedTime = 0.0f;
}

void CStateIdle::Update()
{
	// ベースクラスの処理を呼び出す
	CStateBase::Update();

	// プレイヤーが視野範囲内に入るもしくは、
	// 既に戦闘状態であれば、追跡状態へ移行
	/*if (IsFoundPlayer() || mIsBattle)
	{
		mIsEnd = true;
		ChangeState((int)EState::eChase);
		return;
	}*/

	if (mElapsedTime < mIdleTime)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 状態の処理を終了
		mIsEnd = true;
	}
}

