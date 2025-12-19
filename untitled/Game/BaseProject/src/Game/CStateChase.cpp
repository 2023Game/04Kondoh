#include "CStateChase.h"
#include "CEnemyBase.h"
#include "CPlayer.h"

CStateChase::CStateChase(CEnemyBase* owner)
	: CStateBase("追跡", owner)
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
	
	if (mpOwner->NavMoveTo(targetPos, mpOwner->GetRunSpeed()))
	{
	}

	//// プレイヤーが見えなくなったら、見失った状態に戻す
	//if (!mpOwner->IsLookPlayer())
	//{
	//	見失い状態のEnter関数で指定する
	//	mpLostPlayerNode->SetPos(targetPos);
	//	ChangeState((int)EState::eLost);
	//	return;
	//}
	
	// 走るアニメーションを再生
	/*ChangeAnimation((int)EAnimType::eRun);*/
	
	// 経路探索管理クラスが存在すれば、
	//CNavManager* navMgr = CNavManager::Instance();
	//if (navMgr != nullptr)
	//{
	//	// 自身のノードとプレイヤーのノードが更新中でなければ、
	//	// 自身のノードからプレイヤーのノードまでの最短距離を求める
	//	CNavNode* playerNode = player->GetNavNode();
	//	if (!mpNavNode->IsUpdaing() && 
	//		playerNode != nullptr && !playerNode->IsUpdaing())
	//	{
	//		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
	//		{
	//			// 自身のノードからプレイヤーのノードまで繋がっていたら、
	//			// 移動する位置を次のノードの位置に設定
	//			targetPos = mMoveRoute[1]->GetPos();
	//		}
	//	}
	//}
	// 移動処理
	//if (MoveTo(targetPos, RUN_SPEED))
	//{
	//}
	
	//if (CanAttackPlayer(DISTANT_ATTACK_RANGE))
	//{
	//	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	//	AttackPickDetect();
	//}
}
