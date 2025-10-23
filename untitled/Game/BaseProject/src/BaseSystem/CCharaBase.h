#pragma once
#include "CObjectBase.h"

// �U������
enum class EAttackDir
{
	eNone,	// �����w��Ȃ�
	eUp,    // ��U��
	eDown,  // ���U��
	eRight, // �E�U��
	eLeft,  // ���U��
};

// �U���̋���
enum class EAttackPower
{
	eAttackS,   // ��U��
	eAttackM,   // ���U��
	eAttackL,   // ���U��

	Num,        // �U���̋����̐�
};

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// �R���X�g���N�^
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// �f�X�g���N�^
	virtual ~CCharaBase();

	// �ő�HP���擾
	int GetMaxHp() const;
	// ����HP���擾
	int GetHp() const;

	// �w��̍U���^�C�v��
	bool IsAttackType(EAttackPower power, EAttackDir dir);
	// �U���������擾
	EAttackDir GetAttackDir() const;
	// �U���̕����̕�������擾
	std::string GetAttackDirStr() const;
	// �U���̋������擾
	EAttackPower GetAttackPower() const;
	// �U���̋����̕�������擾
	std::string GetAttackPowerStr() const;


	/// <summary>
	/// �w�肵���U���̕����Ƌ����ŋ��ނ��ǂ���
	/// </summary>
	/// <param name="dir">����</param>
	/// <param name="power">����</param>
	/// <returns>true�ł���΋���</returns>
	virtual bool CheckAttackParry(EAttackDir dir, EAttackPower power) const;

	// �h�䎞�̃p���B
	virtual bool CheckGuardParry() const;

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W��</param>
	/// <param name="stan">���Z����鋯�ݓx</param>
	/// <param name="causer">�_���[�W��^��������</param>
	virtual void TakeDamage(int damage,float stun, float knockback, CObjectBase* causer);

	// �h�䏈��
	virtual void Guard();
	// �������
	virtual void Avoid();
	// �m�b�N�o�b�N
	virtual void KnockBack();
	// �����菈��
	virtual void Hit();
	// ���ݏ���
	virtual void Stun();
	// �����҂�����
//	virtual void StunWait();
	// ���S
	virtual void Death();

	// ��𒆂�
	virtual bool IsAvoiding() const;
	// �h�䒆��
	virtual bool IsGuarding() const;
	// �h��p���B����
	virtual bool IsGuardParry() const;
	// ����ł��邩�ǂ���
	bool IsDeath() const;

	// �X�V
	void Update()override;

protected:
	int mMaxHp;	// �ő�HP
	int mHp;	// ����HP

	float mStunPoints;		// ���݂̋��ݓx
	float mStunThreshold;	// ���ݓx�̂������l

	float mKnockBack;

	EAttackDir mAttackDir;
	EAttackPower mAttackPower;
};
