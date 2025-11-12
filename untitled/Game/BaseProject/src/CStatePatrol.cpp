#include "CStatePatrol.h"
#include "CEnemyBase.h"

CStatePatrol::CStatePatrol(CEnemyBase* owner)
	:CStateBase("巡回", owner)
{
}

//void CStatePatrol::Enter()
//{
//	CStateBase::Enter();
//	switch (mStateStep)
//	{
//	case 0:
//		mpOwner->mNextPatrolIndex = -1;
//		if (ChangePatrolPoint())
//		{
//			mStateStep++;
//		}
//		break;
//	case 1:
//		if (UpdatePatrolRoute())
//		{
//			return;
//		}
//		break;
//	}
//}
//
//void CStatePatrol::Update()
//{
//	CStateBase::Update();
//
//	switch (mStateStep)
//	{
//	case 0:
//	{
//		ChangeAnimation((int)EAnimType::eWalk);
//		// 最短経路の次のノードまで移動
//		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];
//		if (MoveTo(moveNode->GetPos(), WALK_SPEED))
//		{
//			// 移動が終われば、次のノードへ切り替え
//			mNextMoveIndex++;
//			// 最後のノード（目的地のノード）だった場合は、次のステップへ進める
//			if (mNextMoveIndex >= mMoveRoute.size())
//			{
//				mStateStep++;
//			}
//		}
//		break;
//	}
//	// ステップ3：移動後の待機
//	case 2:
//		ChangeAnimation((int)EAnimType::eIdle);
//		if (mElapsedTime < PATROL_INTERVAL)
//		{
//			mElapsedTime += Times::DeltaTime();
//		}
//		else
//		{
//			// 待機最大時間をランダムで決める
//			mIdleTime = Math::Rand(IDLE_TIME_MIN, IDLE_TIME_MAX);
//			if (ChangePatrolPoint())
//			{
//				mStateStep = 1;
//				mElapsedTime = 0.0f;
//			}
//		}
//		break;
//	}
//}
