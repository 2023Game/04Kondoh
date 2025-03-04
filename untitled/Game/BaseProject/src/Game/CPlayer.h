#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CRideableObject.h"
#include "CSound.h"

class CFlamethrower;

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
		eIdle,		// �ҋ@

		eAttack,	// �U��
		eAttackWait,// �U���I���҂�

		eDefense,   // �h��
		eEvasion,   // ���

		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
	};

	// �U������
	enum class EAttackWay
	{
		eIdle,         // �ҋ@

		eUpAttack,    // ��U��
		eDwonAttack,  // ���U��
		eRightAttack, // �E�U��
		eLeftAttack,  // ���U��
	};

	// �U���̋���
	enum class EAttackPower
	{
		eAttackS,   // ��U��
		eAttackM,   // ���U��
		eAttackL,   // ���U��

		Num,        // �U���̋����̐�
	};

	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �w��̍U���^�C�v��
	bool IsAttackType(EAttackPower power, EAttackWay way);
	// �U������
	bool IsAttacking() const override;
	// 
	void AttackStart() override;
	//
	void AttackEnd() override;
	// ���݂̏�Ԃ��擾
	EState GetState();
	// �U���̕������擾
	EAttackWay GetAttackWay();
	// �U���̈З͂��擾
	EAttackPower GetAttackPower();

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

	// ��퓬���̑ҋ@���
	void UpdateIdle();
	// �퓬���̑ҋ@���
	void UpdateAttackIdle();
	// �U��
	void UpdateAttack();
	// �U���I���҂�
//	void UpdateAttackWait();
	// ���
	void UpdateEvasion();
	// �h��
	void UpdateDefense();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	// �ړ��̍X�V����
	void UpdateMove();

	// ���[�V�����u���[�̍X�V����
	/*void UpdateMotionBlur();*/


	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eAttackTPose,    // �퓬����T�|�[�Y
		eAttackIdle,     // �퓬���̑ҋ@�A�j���[�V����

		eAttackWalk,     // ���s
		eAttackBackWalk, // �������ւ̕��s
		eAttackLeftWalk,     // �������ւ̕��s
		eAttackRightWalk,    // �E�����ւ̕��s

		eUpAttackS,	   // ���U���A�j���[�V����
		eUpAttackM,	   // ����U���A�j���[�V����
		eUpAttackL,	   // ����U���A�j���[�V����

		eDwonAttackS,  // �㉺�U���A�j���[�V����
		eDwonAttackM,  // �����U���A�j���[�V����
		eDwonAttackL,  // �����U���A�j���[�V����

		eRightAttackS, // ��E�U���A�j���[�V����
		eRightAttackM, // ���E�U���A�j���[�V����
		eRightAttackL, // ���E�U���A�j���[�V����

		eLeftAttackS,  // �㍶�U���A�j���[�V����
		eLeftAttackM,  // �����U���A�j���[�V����
		eLeftAttackL,  // �����U���A�j���[�V����

		eDefense,      // �h��
		eEvasion,      // ���

		eJumpStart,	   // �W�����v�J�n
		eJump,		   // �W�����v��
		eJumpEnd,	   // �W�����v�I��

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
	void ChangeEvasion();
	// �U����Ԃ֐؂�ւ�
	void ChangeAttack();

	EAttackWay mAttackWay;			// �U������
	EAttackPower mAttackPower;		// �U���̋���
	EAttackPower mCurrAttackPower;	// ���݂̍U���̋���

	// ���[�h�I��
	enum class EMode
	{
		eNotBattle, // ��퓬���[�h
		eBattle,    // �퓬���[�h
	};
	EMode mMode;  //���[�h�I��

	// ���b�N�I������^�[�Q�b�g��ύX
	void ChangeLockOnTarget();
	// �^�[�Q�b�g�����b�N�I������
	void LockOnTarget();
	
	bool mIsBattleMode;				// �o�g�����[�h��
	bool mIsLockOn;					// ���b�N�I�����Ă��邩
	CObjectBase* mpLockOnTarget;	// �^�[�Q�b�g�̃|�C���^




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
	// ����p�R���C�_
	CColliderSphere* mpDetectCol;

	CTransform* mpRideObject;

	CVector mEvaDir;    // ������

	int mStateStep;     // ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime; // �o�ߎ��Ԍv���p

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	//// �Ή����˃G�t�F�N�g
	//CFlamethrower* mpFlamethrower;

	//// ���[�V�����u���[���|����c�莞��
	//float mMotionBlurRemainTime;
};
