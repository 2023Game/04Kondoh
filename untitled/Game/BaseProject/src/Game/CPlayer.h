#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

class CFlamethrower;
class CGaugeUI2D;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer : public CXCharacter
{
public:

	// �v���C���[�̏��
	enum class EState
	{
//		eIdle,			// �ҋ@
		eBattleIdle,	// �퓬���̑ҋ@

		eAttack,	// �U��

		eGuardStart,	// �h��J�n
		eGuard,			// �h�䒆
		eGuardEnd,		// �h��I��
		eGuardParry,	// �h��p���B
		eAvoid,			// ���

		eKnockBack,	// �m�b�N�o�b�N
		eHit,		// ������
		eStun,		// ����
		eDeath,		// ���S

		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};
	// ���݂̏�Ԃ��擾
	EState GetState();

	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;
	// �h�䒆��
	bool IsGuarding() const override;
	// �W�����v����
	bool IsJumping() const;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, float stan, float knockback, CObjectBase* causer) override;

	// �����菈��
	void Hit() override;
	// ���S����
	void Death() override;

	// �X�V
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

private:
	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec(bool force = false) const;
	/// <summary>
	/// �_���[�W�ʂ��v�Z���ĕԂ�
	/// </summary>
	/// <param name="taker">�_���[�W���󂯂�L����</param>
	/// <param name="outDamage">�_���[�W�ʂ�Ԃ��p</param>
	/// <param name="outStan">���ݓx��Ԃ��p</param>
	void CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const;

	// ��퓬���̑ҋ@���
//	void UpdateIdle();
	// �퓬���̑ҋ@���
	void UpdateBattleIdle();
	// �U������
	void UpdateAttack();
	// �h��J�n����
	void UpdateGuardStart();
	// �h�䒆����
	void UpdateGuard();
	// �h��I��
	void UpdateGuardEnd();
	// �h��p���B����
	void UpdateGuardParry();
	// �������
	void UpdateAvoid();

	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	// �ړ��̍X�V����
	void UpdateMove();

	// �����菈��
	void UpdateHit();
	// ���S����
	void UpdateDeath();


	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eBattleTPose,	// �퓬����T�|�[�Y
		eBattleIdle,	// �퓬���̑ҋ@�A�j���[�V����

		eBattleWalk,		// ���s
		eBattleBackWalk,	// �������ւ̕��s
		eBattleLeftWalk,	// �������ւ̕��s
		eBattleRightWalk,	// �E�����ւ̕��s

		eRun,		// ����
		eBackRun,	// �������֑���
		eLeftRun,	// �������֑���
		eRightRun,	// �E�����֑���

		eUpAttackS,		// ���U���A�j���[�V����
		eUpAttackM,		// ����U���A�j���[�V����
		eUpAttackL,		// ����U���A�j���[�V����

		eRightAttackS,	// ��E�U���A�j���[�V����
		eRightAttackM,	// ���E�U���A�j���[�V����
		eRightAttackL,	// ���E�U���A�j���[�V����

		eLeftAttackS,	// �㍶�U���A�j���[�V����
		eLeftAttackM,	// �����U���A�j���[�V����
		eLeftAttackL,	// �����U���A�j���[�V����

		eGuardStart,	// �h��J�n
		eGuard,			// �h�䒆
		eGuardEnd,		// �h��I��
		eGuardHit,		// �h�䎞�̋�����
		eGuardParry,	// �h�䎞�̃p���B
		eAvoid,			// ���

		eJumpStart,		// �W�����v�J�n
		eJump,			// �W�����v��
		eJumpEnd,		// �W�����v�I��
		eJumpAttack,	// �W�����v�U��

		eHit1,			// ������1
		eHit2,			// ������2
		eDeath1,		// ���S�P
		eDeath2,		// ���S�Q

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type, bool restart = false);

	// �U��������A�U���̈З͂ɂ����
	// �؂�ւ���A�j���[�V�����̎�ނ��擾
	EAnimType GetAttackAnimType() const;

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float speed;		// �A�j���[�V�������x�i1.0�œ��{�j
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];


	EState mState;	// �v���C���[�̏��
	//��Ԑ؂�ւ�
	void ChangeState(EState state);
	// �����Ԃ֐؂�ւ�
	void ChangeAvoid();
	// �U����Ԃ֐؂�ւ�
	void ChangeAttack();

	// �I�𒆂̍U���̋���
	EAttackPower mSelectAttackPower;
	
	bool mIsBattleMode;		// �o�g�����[�h��
	bool mIsAvoiding;		// ��𒆂�

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	// �{�̂̃R���C�_
	CCollider* mpBodyCol;
	// �U���p�̃R���C�_�P�i���̐n�̕����j
	CColliderCapsule* mpAttackCol1;
	// �U���p�̃R���C�_�Q�i���̎�����̕����j
	CColliderCapsule* mpAttackCol2;
	// �U���p�̃R���C�_�R�i���̕����j
	CColliderSphere* mpAttackCol3;

	CTransform* mpRideObject;

	CGaugeUI2D* mpHpUI;


	int mStateStep;     // ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime; // �o�ߎ��Ԍv���p

	float mKnockBack;

	CVector mAvoidDir;	// ������
	CVector mMoveStartPos;	// �ړ��J�n�̈ʒu
	CVector mMoveEndPos;	// �ړ��I���̈ʒu

	int mRandHitAnim;	// �����_���ȋ�����A�j���[�V����
	int mRandDeathAnim;	// �����_���Ȏ��S�A�j���[�V����

	bool mIsRun;			// �����t���O
	float mMoveElapsedTime;	// �ړ��o�ߎ���
	int mPushRunKey;		// ���͂���Ă��郉���L�[

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;

};
