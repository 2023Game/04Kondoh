#include "CCharaBase.h"

#define STAN_RECOVERY 1.0f	// 怯み度の１秒間の回復量

// コンストラクタ
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMaxHp(10)
	, mHp(mMaxHp)
	, mStunPoints(0.0f)
	, mStunThreshold(100.0f)
	, mKnockBack(0.0f)
	, mAttackDir(EAttackDir::eNone)
	, mAttackPower(EAttackPower::eAttackS)
{
}

// デストラクタ
CCharaBase::~CCharaBase()
{
}

// 最大HPを取得
int CCharaBase::GetMaxHp() const
{
	return mMaxHp;
}

// 現在HPを取得
int CCharaBase::GetHp() const
{
	return mHp;
}

// 指定の攻撃タイプか
bool CCharaBase::IsAttackType(EAttackPower power, EAttackDir dir)
{
	if (mAttackPower == power && mAttackDir == dir) return true;
	else
		return false;
}

// 攻撃方向を取得
EAttackDir CCharaBase::GetAttackDir() const
{
	return mAttackDir;
}

// 攻撃の方向の文字列を取得
std::string CCharaBase::GetAttackDirStr() const
{
	switch (mAttackDir)
	{
	case EAttackDir::eUp:		return "上";
	case EAttackDir::eDown:		return "下";
	case EAttackDir::eLeft:		return "左";
	case EAttackDir::eRight:	return "右";
	}

	return "";
}

// 攻撃の強さを取得
EAttackPower CCharaBase::GetAttackPower() const
{
	return mAttackPower;
}

// 攻撃の強さの文字列を取得
std::string CCharaBase::GetAttackPowerStr() const
{
	switch (mAttackPower)
	{
	case EAttackPower::eAttackS:	return "弱";
	case EAttackPower::eAttackM:	return "中";
	case EAttackPower::eAttackL:	return "強";
	}

	return "";
}

// 指定した攻撃の方向と強さで怯むかどうか
bool CCharaBase::CheckAttackParry(EAttackDir dir, EAttackPower power) const
{
	// そもそも攻撃してない場合、パリィ出来ない
	if (!IsAttacking()) return false;

	// 攻撃の強さが一致していない場合、パリィ出来ない
	if (mAttackPower != power) return false;

	// 攻撃の方向が一致していない場合、パリィ出来ない
	if (mAttackDir != dir) return false;

	// 全ての条件式を満たした場合、パリィ出来る
	return true;
}

bool CCharaBase::CheckGuardParry() const
{
	return false;
}

// ダメージを受ける
void CCharaBase::TakeDamage(int damage, float stun, float knockback, CCharaBase* causer)
{
	// 既に死亡していたら、ダメージを受けない
	if (IsDeath()) return;

	// 受けたダメージが現在HP以上なら
	if (damage >= mHp)
	{
		// HPを0にして、死亡
		mHp = 0;
		Death();
	}
	// 現在HPの方が多い場合は、ダメージ分減らす
	else
	{
		// まだ、生きている場合、怯み値を増加
		mHp -= damage;
		mStunPoints += stun;

		// 怯み度がしきい値を超えると
		if (mStunPoints > mStunThreshold)
		{
			// 怯み処理を実行して、怯み度をリセット
			Stun();
			mStunPoints = 0.0f;
		}
		else
		{
			// 怯み度がしきい値を超えて無く場合、仰け反り処理
			if (!IsAttacking())
			{	
				mKnockBack = knockback;
				Hit();
			}
			else if (IsAttacking() 
				&& causer->GetAttackPower() == EAttackPower::eAttackL
				|| causer->GetAttackPower() == EAttackPower::eAttackM
				)
			{
				mKnockBack = knockback;
				Hit();
			}
			else if (IsAttacking()
				&& causer->GetAttackPower() == EAttackPower::eAttackS
				)
			{
			}
		}
	}
}

// 防御処理
void CCharaBase::Guard()
{
}

// 回避処理
void CCharaBase::Avoid()
{
}

// ノックバック
void CCharaBase::KnockBack()
{
}

// 仰け反り処理
void CCharaBase::Hit()
{
}

// 怯み処理
void CCharaBase::Stun()
{
}

// 死亡
void CCharaBase::Death()
{
}

bool CCharaBase::IsAvoiding() const
{
	return false;
}

// 防御中か
bool CCharaBase::IsGuarding() const
{
	return false;
}

// 防御パリィ中か
bool CCharaBase::IsGuardParry() const
{
	return false;
}

// 仰け反り中か
bool CCharaBase::IsHiting() const
{
	return false;
}

// 死んでいるかどうか
bool CCharaBase::IsDeath() const
{
	// 現在HPが0ならば、死亡
	return mHp <= 0;
}

void CCharaBase::Update()
{
	// 怯み度が少しでも溜まっていたら、
	if (mStunPoints > 0.0f)
	{
		// 徐々に回復する
		mStunPoints -= STAN_RECOVERY * Times::DeltaTime();
		if (mStunPoints < 0.0f) mStunPoints = 0.0f;
	}
}
