#include "CStateLost.h"
#include "CEnemyBase.h"
#include "CPlayer.h"

#define LOST_TIME 5.0f				// 壁に引っかかった時の追跡処理有効時間

CStateLost::CStateLost(CEnemyBase* owner)
	: CStateBase("見失う",owner)
	, mElapsedTime(0.0f)
	, mStateStep(0)
{
}

void CStateLost::Enter()
{
	CStateBase::Enter();

	CPlayer* player = CPlayer::Instance();
	mpOwner->SetIsBattle(true);
	mpOwner->SetBattleTarget(player);

	mpOwner->SetLostPlayerNodePos(player->Position());

	mElapsedTime = 0.0f;
}

void CStateLost::Update()
{

	switch (mStateStep)
	{
	case 0:
	{
		// アニメーションの設定
		mpOwner->ChangeStateAnimation(mIndex);

		mStateStep++;
		break;
	}
	case 1:


		if (mpOwner->NavMoveTo(mpOwner->GetLostPlayerNodePos(), mpOwner->GetRunSpeed()))
		{
			if (mElapsedTime <= LOST_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// 戦闘状態終了
				mpOwner->SetIsBattle(false);
				mpOwner->SetBattleTarget(nullptr);
				mIsEnd = true;
			}
		}
		break;
	}
}
