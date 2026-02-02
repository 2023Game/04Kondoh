#include "CStateChase.h"
#include "CEnemyBase.h"
#include "CPlayer.h"

CStateChase::CStateChase(CEnemyBase* owner)
	: CStateBase(owner, "追跡")
	, mElapsedTime(0.0f)
{
}

void CStateChase::Enter()
{
	// ベースクラスの処理を呼び出す
	CStateBase::Enter();

	CPlayer* player = CPlayer::Instance();
	mpOwner->SetIsBattle(true);
	mpOwner->SetBattleTarget(player);
}

void CStateChase::Update()
{
	if (mpOwner->DetectedPlayerAttack()) return;
	
	// プレイヤーの座標へ向けて移動する
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();
	
	// 戦闘状態に入っていなければ
	if (!mpOwner->GetIsBattle())
	{
		// 戦闘状態になる
		mpOwner->SetIsBattle(true);
		mpOwner->SetBattleTarget(player);
	}

	mpOwner->ChangeStateAnimation(mIndex);
	
	// プレイヤーの位置まで経路探索して移動する
	if (mpOwner->NavMoveTo(targetPos, mpOwner->GetRunSpeed()))
	{
	}

	//if (CanAttackPlayer(DISTANT_ATTACK_RANGE))
	//{
	//	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	//	AttackPickDetect();
	//}
}
