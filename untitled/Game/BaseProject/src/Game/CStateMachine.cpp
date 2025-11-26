#include "CStateMachine.h"
#include "CStateBase.h"

CStateMachine::CStateMachine()
	: mpCuttent(nullptr)
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

void CStateMachine::Update()
{
	// 実行中の状態が存在する場合
	if (mpCuttent != nullptr)
	{
		// 実行中の状態の処理が終了してなければ
		if (!mpCuttent->IsEnd())
		{
			// 更新処理を呼び出す
			mpCuttent->Update();
		}
	}
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
	if (mpCuttent != nullptr)
	{
		// 実行中の状態の終了処理を呼び出す
		mpCuttent->Exit();
		mpCuttent = nullptr;
	}

	// 指定されたインデクス値が範囲外であれば、処理しない
	if (!(0 <= index && index < mStates.size())) return;

	// 指定された状態を、現在の状態に設定
	mpCuttent = mStates[index];
	// 現在の状態が存在する場合は、開始処理を呼び出す
	if (mpCuttent != nullptr)
	{
		mpCuttent->Enter();
	}
}

#if _DEBUG
void CStateMachine::DebugRender()
{
	// 現在の状態をデバッグ表示
	std::string str = "なし";
	if (mpCuttent != nullptr) str = mpCuttent->GetName();
	CDebugPrint::Print("状態：%S", str.c_str());
}
#endif
