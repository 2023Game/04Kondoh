#include "CCharaBase.h"

#define STAN_RECOVERY 1.0f	// ���ݓx�̂P�b�Ԃ̉񕜗�

// �R���X�g���N�^
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

// �f�X�g���N�^
CCharaBase::~CCharaBase()
{
}

// �ő�HP���擾
int CCharaBase::GetMaxHp() const
{
	return mMaxHp;
}

// ����HP���擾
int CCharaBase::GetHp() const
{
	return mHp;
}

// �w��̍U���^�C�v��
bool CCharaBase::IsAttackType(EAttackPower power, EAttackDir dir)
{
	if (mAttackPower == power && mAttackDir == dir) return true;
	else
		return false;
}

// �U���������擾
EAttackDir CCharaBase::GetAttackDir() const
{
	return mAttackDir;
}

// �U���̕����̕�������擾
std::string CCharaBase::GetAttackDirStr() const
{
	switch (mAttackDir)
	{
	case EAttackDir::eUp:		return "��";
	case EAttackDir::eDown:		return "��";
	case EAttackDir::eLeft:		return "��";
	case EAttackDir::eRight:	return "�E";
	}

	return "";
}

// �U���̋������擾
EAttackPower CCharaBase::GetAttackPower() const
{
	return mAttackPower;
}

// �U���̋����̕�������擾
std::string CCharaBase::GetAttackPowerStr() const
{
	switch (mAttackPower)
	{
	case EAttackPower::eAttackS:	return "��";
	case EAttackPower::eAttackM:	return "��";
	case EAttackPower::eAttackL:	return "��";
	}

	return "";
}

// �w�肵���U���̕����Ƌ����ŋ��ނ��ǂ���
bool CCharaBase::CheckAttackParry(EAttackDir dir, EAttackPower power) const
{
	// ���������U�����ĂȂ��ꍇ�A�p���B�o���Ȃ�
	if (!IsAttacking()) return false;

	// �U���̋�������v���Ă��Ȃ��ꍇ�A�p���B�o���Ȃ�
	if (mAttackPower != power) return false;

	// �U���̕�������v���Ă��Ȃ��ꍇ�A�p���B�o���Ȃ�
	if (mAttackDir != dir) return false;

	// �S�Ă̏������𖞂������ꍇ�A�p���B�o����
	return true;
}

bool CCharaBase::CheckGuardParry() const
{
	return false;
}

// �_���[�W���󂯂�
void CCharaBase::TakeDamage(int damage, float stun, float knockback, CObjectBase* causer)
{
	// ���Ɏ��S���Ă�����A�_���[�W���󂯂Ȃ�
	if (IsDeath()) return;

	// �󂯂��_���[�W������HP�ȏ�Ȃ�
	if (damage >= mHp)
	{
		// HP��0�ɂ��āA���S
		mHp = 0;
		Death();
	}
	// ����HP�̕��������ꍇ�́A�_���[�W�����炷
	else
	{
		// �h�䒆�Ȃ�_���[�W���󂯂Ȃ�
		if (IsGuarding())
		{
			mHp -= damage * 0.5f;
		}
		else
		{
			mHp -= damage;
			// �܂��A�����Ă���ꍇ�A���ݓx�����Z
			mStunPoints += stun;
		}

		// ���ݓx���������l�𒴂����
		if (mStunPoints > mStunThreshold)
		{
			// ���ݏ��������s���āA���ݓx�����Z�b�g
			Stun();
			mStunPoints = 0.0f;
		}
		else
		{
			if (!IsAttacking())
			{
				mKnockBack = knockback;
				// ���ݓx���������l�𒴂��Ė����ꍇ�A�����菈��
				Hit();
			}
		}
	}
}

// �h�䏈��
void CCharaBase::Guard()
{
}

// �������
void CCharaBase::Avoid()
{
}

// �m�b�N�o�b�N
void CCharaBase::KnockBack()
{
}

// �����菈��
void CCharaBase::Hit()
{
}

// ���ݏ���
void CCharaBase::Stun()
{
}

// ���S
void CCharaBase::Death()
{
}

// �h�䒆��
bool CCharaBase::IsGuarding() const
{
	return false;
}

// �h��p���B����
bool CCharaBase::IsGuardParry() const
{
	return false;
}

// ����ł��邩�ǂ���
bool CCharaBase::IsDeath() const
{
	// ����HP��0�Ȃ�΁A���S
	return mHp <= 0;
}


void CCharaBase::Update()
{
	// ���ݓx�������ł����܂��Ă�����A
	if (mStunPoints > 0.0f)
	{
		// ���X�ɉ񕜂���
		mStunPoints -= STAN_RECOVERY * Times::DeltaTime();
		if (mStunPoints < 0.0f) mStunPoints = 0.0f;
	}
}
