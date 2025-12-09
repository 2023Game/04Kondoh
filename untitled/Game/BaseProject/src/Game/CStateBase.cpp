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

int CStateBase::GetIndex() const
{
	return mIndex;
}

const std::string& CStateBase::GetName() const
{
	return mStateName;
}

// 他の状態への遷移条件を追加
void CStateBase::AddTransition(int nextStateId, bool isWaitEnd, CondFunc func)
{
	// 遷移情報をリストに追加
	Transition trans{ nextStateId, isWaitEnd, func };
	mTransitions.push_back(trans);
}

// 他の状態へ遷移する条件を満たしているか
int CStateBase::CheckTransition() const
{
	// リストの先頭から順番に遷移条件を満たしているか判断する
	for (const Transition& trans : mTransitions)
	{
		// 遷移条件の関数が設定されていない場合
		if (trans.condition == nullptr)
		{
			// 以下の2つを満たしていたら、遷移先の状態へ遷移する
			// ①現在の状態が終了時に遷移する
			// ②現在の状態が終了している
			if (trans.isWaitEnd && IsEnd())
			{
				return trans.nextStateId;
			}
		}
		// 遷移条件の関数が設定されている場合
		else
		{
			// 現在の状態の終了待ち設定かつ、現在の状態が終了していなければ、
			// 遷移条件を満たしていない
			if (trans.isWaitEnd && !IsEnd()) continue;

			// 設定されている遷移条件を満たしていたら、次の状態へ遷移する
			if (trans.condition())
			{
				return trans.nextStateId;
			}
		}
	}

	// 全ての遷移条件を満たしていない
	return -1;
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