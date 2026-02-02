#include "CStateAttack.h"
#include "CEnemyBase.h"
#include "CPlayer.h"

class CCharaBase;

CStateAttack::CStateAttack(CEnemyBase* owner, const std::string& name)
	:CStateBase(owner, "UŒ‚")
{
}

void CStateAttack::Enter()
{
	CStateBase::Enter();

	CPlayer* player = CPlayer::Instance();
	mpOwner->SetIsBattle(true);
	mpOwner->SetBattleTarget(player);
}

void CStateAttack::Update()
{
}

void CStateAttack::SetRandAnimNum(int number)
{
}
