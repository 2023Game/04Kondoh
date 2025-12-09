#include "CStateMachine.h"
#include "CStateBase.h"
#include "CEnemyBase.h"

CStateMachine::CStateMachine()
	: mpOwner(nullptr)
	, mpCurrent(nullptr)
{
}

CStateMachine::~CStateMachine()
{
	// 登録してあるステートを全て削除
	auto itr = mStates.begin();
	while (itr != mStates.end())
	{
		// ステートが存在したら、削除
		CStateBase* state = *itr;
		if (state != nullptr)
		{
			delete state;
		}
		// リストから取り除く
		itr = mStates.erase(itr);
	}
}

// 持ち主を設定
void CStateMachine::SetOwner(CEnemyBase* owner)
{
	mpOwner = owner;
}

void CStateMachine::Update()
{
	// 実行中の状態が存在する場合
	if (mpCurrent != nullptr)
	{
		// 実行中の状態の処理が終了してなければ
		if (!mpCurrent->IsEnd())
		{
			// 更新処理を呼び出す
			mpCurrent->Update();
		}

		// 他の状態への遷移条件を満たしているか
		int nextStateId = mpCurrent->CheckTransition();
		if (nextStateId >= 0)
		{
			// 遷移条件を満たしていたら、次の状態へ遷移
			if (mpOwner != nullptr) mpOwner->ChangeState(nextStateId);
			else ChangeState(nextStateId);
		}
	}
}

// 現在のステートを取得
CStateBase* CStateMachine::GetCurrentState() const
{
	return mpCurrent;
}

// 状態の登録
void CStateMachine::RegisterState(int index, CStateBase* state)
{
	// 不正なインデクス値の場合、処理しない
	if (index < 0) return; 
	// 指定されたインデクス値が、リストのサイズより大きい場合
	if (index >= mStates.size())
	{
		// リストの要素数を追加（追加した要素はnullptrで初期化）
		mStates.resize(index + 1, nullptr);
	}
	// 指定した状態に番号を設定
	state->SetIndex(index);
	// 指定したインデクス値に、指定した状態を登録
	mStates[index] = state;

}

// 状態の切り替え
void CStateMachine::ChangeState(int index)
{
	// 既に実行中の状態が存在する場合
	if (mpCurrent != nullptr)
	{
		// 実行中の状態の終了処理を呼び出す
		mpCurrent->Exit();
		mpCurrent = nullptr;
	}

	// 指定されたインデクス値が範囲外であれば、処理しない
	if (!(0 <= index && index < mStates.size())) return;

	// 指定された状態を、現在の状態に設定
	mpCurrent = mStates[index];
	// 現在の状態が存在する場合は、開始処理を呼び出す
	if (mpCurrent != nullptr)
	{
		mpCurrent->Enter();
	}
}

#if _DEBUG
void CStateMachine::DebugRender()
{
	// 現在の状態をデバッグ表示
	std::string str = "なし";
	if (mpCurrent != nullptr) str = mpCurrent->GetName();
	CDebugPrint::Print("状態：%S", str.c_str());
}
#endif
