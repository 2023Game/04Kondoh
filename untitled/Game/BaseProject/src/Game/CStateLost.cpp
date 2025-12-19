#include "CStateLost.h"

CStateLost::CStateLost(CEnemyBase* owner)
	:CStateBase("見失う",owner)
{
}

void CStateLost::Enter()
{
	CStateBase::Enter();
}

void CStateLost::Update()
{
	//	CNavManager* navMgr = CNavManager::Instance();
//	CPlayer* target = CPlayer::Instance();
//
//	if (navMgr == nullptr || target == nullptr)
//	{
//		// 戦闘状態終了
//		mIsBattle = false;
//		mpBattleTarget = nullptr;
//		ChangeState((int)EState::eIdle);
//		return;
//	}
//	// プレイヤーが見えたら、追跡状態へ移行
//	if (IsLookPlayer()) 
//	{
//		ChangeState((int)EState::eChase);
//		return;
//	}
//
//	// 走るアニメーションを再生
//	ChangeAnimation((int)EAnimType::eRun);
//
//	switch (mStateStep)
//	{
//	case 0:
//		mElapsedTime = 0.0f;
//		mStateStep++;
//		break;
//
//	case 1:	// ステップ1：見失った位置までの最短経路を求める
//		// 
//		mpLostPlayerNode->SetPos(target->Position());
//
//		// 自身のノードと見失った位置のノードが更新中でなければ（更新が終わってたら）
//		if (!mpNavNode->IsUpdaing() && !mpLostPlayerNode->IsUpdaing())
//		{
//			if (!navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
//			{
//				// 戦闘状態終了
//				mIsBattle = false;
//				mpBattleTarget = nullptr;
//				// 経路が繋がっていなければ、待機状態へ戻す
//				ChangeState((int)EState::eIdle);
//				mpLostPlayerNode->SetEnable(false);
//
//			}
//			else
//			{
//				// 見失った位置まで経路が繋がっていたら、次のステップへ
//				mNextMoveIndex = 1;
//				mStateStep++;
//			}
//		}
//		break;
//	case 2:
//		// 次の移動先のインデックス値が不正値でなければ
//		if (0 <= mNextMoveIndex && mNextMoveIndex < mMoveRoute.size())
//		{
//			// プレイヤーを見失った位置まで移動
//			if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
//			{
//				if (mNextMoveIndex >= mMoveRoute.size())
//				{
//					// 戦闘状態終了
//					mIsBattle = false;
//					mpBattleTarget = nullptr;
//					// 移動が終われば、待機状態へ移行
//					ChangeState((int)EState::eIdle);
//				}
//				else
//				{
//					if (mElapsedTime <= LOST_TIME)
//					{
//						mElapsedTime += Times::DeltaTime();
//					}
//					else
//					{
//						// 戦闘状態終了
//						mIsBattle = false;
//						mpBattleTarget = nullptr;
//						// 移動が終われば、待機状態へ移行
//						ChangeState((int)EState::eIdle);
//					}
//				}
//			}
//		}
//		break;
//	}
}
