#include "CAttackBlow.h"
#include "CEnemyBase.h"
#include "Maths.h"

#define BLOW_MOVE_START		 5.0f	// 薙ぎ払い時の移動開始フレーム
#define BLOW_MOVE_END		60.0f	// 薙ぎ払い時の移動終了フレーム
#define BLOW_MOVE_SPEED		20.0f	// 薙ぎ払い時の移動スピード

CAttackBlow::CAttackBlow(CEnemyBase* owner)
	: CStateAttack(owner, "薙ぎ払い")
{
}

void CAttackBlow::Enter()
{
	CStateAttack::Enter();
}

void CAttackBlow::Update()
{
	switch (mStateStep)
	{
	case 0:
		mpOwner->ChangeStateAnimation(mIndex, mRandAnimNum);
		mStateStep++;
		break;
	case 1:
	{
		// 
		float frame = mpOwner->GetAnimationFrame();

		const CEnemyBase::AttackData& data = 
			(*mpOwner->GetAttackData())[mpOwner->GetAttackType()];
		const CEnemyBase::AttackRangeData& range = 
			mpOwner->GetRangeData(data.rangeNamber);

		float blowMoveStart = data.attackParryEndFrame;

		mpOwner->AttackStart();

		if (!mpOwner->IsMoveAttackRange(range)) {
			if (frame >= blowMoveStart) {

				// 移動終了フレームまで到達していない場合
				if (frame < BLOW_MOVE_SPEED) {
					CVector pos = mpOwner->Position();
					pos += mpOwner->VectorZ() * BLOW_MOVE_SPEED * Times::DeltaTime();
					mpOwner->Position(pos);
				}
				// 移動終了フレームまで到達した場合
				else {
					mStateStep++;
				}
			}
			else {
				mpOwner->LookAtBattleTarget();
			}
		}
		else {
			mStateStep++;
		}
		break;
	}
	case 2:
		if (mpOwner->IsAnimationFinished()) {
			mpOwner->AttackEnd();
			if()
		}


	}
}
