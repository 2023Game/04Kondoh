#include "CCharaBase.h"

#define STAN_RECOVERY 1.0f	// ���ݓx�̂P�b�Ԃ̉񕜗�

// �R���X�g���N�^
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMaxHp(10)
	, mHp(mMaxHp)
	, mStanPoints(0.0f)
	, mStanThreshold(100.0f)
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
bool CCharaBase::CheckParry(EAttackDir dir, EAttackPower power) const
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

// �_���[�W���󂯂�
void CCharaBase::TakeDamage(int damage, float stan, CObjectBase* causer)
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
		mHp -= damage;

		// �܂��A�����Ă���ꍇ�A���ݓx�����Z
		mStanPoints += stan;
		// ���ݓx���������l�𒴂����
		if (mStanPoints > mStanThreshold)
		{
			// ���ݏ��������s���āA���ݓx�����Z�b�g
			Stan();
			mStanPoints = 0.0f;
		}
	}
}

// ���S
void CCharaBase::Death()
{
}

// ����ł��邩�ǂ���
bool CCharaBase::IsDeath() const
{
	// ����HP��0�Ȃ�΁A���S
	return mHp <= 0;
}

// ���ݏ���
void CCharaBase::Stan()
{
}

void CCharaBase::Update()
{
	// ���ݓx�������ł����܂��Ă�����A
	if (mStanPoints > 0.0f)
	{
		// ���X�ɉ񕜂���
		mStanPoints -= STAN_RECOVERY * Times::DeltaTime();
		if (mStanPoints < 0.0f) mStanPoints = 0.0f;
	}
}
