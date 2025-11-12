#include "CStateBase.h"


CStateBase::CStateBase(const std::string& name, CEnemyBase* owner)
	: mpOwner(owner)
	, mStateName(name)
	, mIndex(-1)
	, mIsEnd(false)
{
}

// ステートの処理が終わったかどうか
bool CStateBase::IsEnd() const
{
	return mIsEnd;
}

// 状態番号を設定
void CStateBase::SetIndex(int index)
{
	mIndex = index;
}

const std::string& CStateBase::GetName() const
{
	return mStateName;
}

// ステートが始まる時に一度だけ呼ばれる関数
void CStateBase::Enter()
{
	// 状態の処理を初期化
	mIsEnd = false;
}

// ステートが更新される時に呼ばれる関数
void CStateBase::Update()
{
}

// ステートが終了する時に一度だけ呼ばれる関数
void CStateBase::Exit()
{
}