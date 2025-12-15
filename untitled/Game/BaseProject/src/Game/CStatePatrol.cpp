#include "CStatePatrol.h"
#include "CEnemyBase.h"

#define PATROL_INTERVAL 3.0f    // 次の巡回に移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f  // 巡回開始時に選択される巡回ポイントの最短距離

CStatePatrol::CStatePatrol(CEnemyBase* owner)
	: CStateBase("巡回", owner)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mNextPatrolIndex(-1)
{
}

// 巡回するポイントのリストを設定
void CStatePatrol::SetPatrolPoints(std::vector<CVector> patrolPoints)
{
	mPatrolPoints = patrolPoints;
}

// 巡回ポイントを切り替え
bool CStatePatrol::ChangePatrolPoint()
{
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return false;

	// 巡回開始時であれば、一番近い巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;     // 一番近い巡回ポイントの番号
		float nearDist = 0.0f;  // 一番近い巡回ポイントまでの距離
		// 全ての巡回ポイントを調べ、一番近い巡回ポイントを探す
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i];
			CVector vec = point - mpOwner->Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// 巡回ポイントが近すぎる場合は、スルー
			if (dist < PATROL_NEAR_DIST) continue;

			// 一番近い巡回ポイントもしくは、
			// 現在一番近い巡回ポイントよりさらに近い場合は、
			// 巡回ポイントの番号を置き換える
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dist;
			}
		}
		mNextPatrolIndex = nearIndex;
	}
	// 巡回中だった場合、次の巡回ポイントを指定する
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	return mNextPatrolIndex >= 0;
}

void CStatePatrol::Enter()
{
	CStateBase::Enter();

	mStateStep = 0;
	mElapsedTime = 0.0f;
	mNextPatrolIndex = -1;
}

void CStatePatrol::Update()
{
	CStateBase::Update();

	switch (mStateStep)
	{
		case 0:
		{
			ChangePatrolPoint();
			mStateStep++;
			break;
		case 1:
			mpOwner->ChangeStateAnimation(mIndex, 1);
			if(mpOwner->NavMoveTo(mPatrolPoints[mNextPatrolIndex], mpOwner->GetWalkSpeed()))
			{
				mStateStep++;
			}
			break;
		}
		// ステップ2：移動後の待機
		case 2:
			mpOwner->ChangeStateAnimation(mIndex, 0);
			if (mElapsedTime < PATROL_INTERVAL)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				mStateStep = 0;
				mElapsedTime = 0.0f;
			}
			break;
	}
}
