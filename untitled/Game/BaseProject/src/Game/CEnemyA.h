#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CEnemyBase.h"
#include "CCollider.h"
#include "CModel.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;
class CNavNode;

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemyA : public CEnemyBase
{

public:
	// �R���X�g���N�^
	CEnemyA(const CVector& pos, std::vector<CVector> patrolPoints);
	~CEnemyA();

	// �K�v�ȃ��\�[�X��ǂݍ���
	static void LoadResources();

	// �I�u�W�F�N�g�폜����
	void DeleteObject(CObjectBase* obj) override;
	// �X�V����
	void Update() override;
	// �`��
	void Render() override;

	// �w��̃X�e�[�g��
	bool IsState(int state) override;
	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// ��𒆂��ǂ���
	bool IsAvoiding() const override;
	// �h�䒆���ǂ���
	bool IsGuarding() const override;

	// �_���[�W���󂯂�
	void TakeDamage(int damage,float stan, float knockback, CObjectBase* causer) override;

	// �p���B�����ꂽ���̏���
	void Parry() override;
	// �����菈��
	void Hit() override;
	// ���ݏ���
	void Stun() override;
	// ���S����
	void Death() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,			// T�|�[�Y
		eIdle,			// �ҋ@
		eBattleIdle,	// �퓬�ҋ@
		eWalk,			// ���s
		eRun,			// ����
		eStun,			// ����
		eStunWait,		// �����҂�����
		eDeath,			// ���S

		eBlowL,			// ���ガ����
		eBlowR,			// �E�ガ����
		eRoundKickL,	// ���񂵏R��
		eRoundKickR,	// �E�񂵏R��
		eTackleStart,	// �^�b�N���̗���
		eTackle,		// �^�b�N��
		eTackleEnd,		// �^�b�N���̑҂�����
		eHeadButt,		// ���˂��U��

		eBattleWalkL,	// �퓬���̍����s
		eBattleWalkR,	// �퓬���̉E���s

		eGuardIdle,		// �h��ҋ@���
		eGuardHit,		// �h�䎞�̃q�b�g
		eLeftAvoid,		// �����
		eRightAvoid,	// �E���

		eHit1,			// ������1
		eHit2,			// ������2
		eHit3,			// ������3
		eChanceHit1,	// �`�����X���̋�����1
		eChanceHit2,	// �`�����X���̋�����2
		eChanceHit3,	// �`�����X���̋�����3
		eChanceHit4,	// �`�����X���̋�����4

		Num
	};
	EAnimType mAnimType;

	// �G�̏��
	enum class EState
	{
		eIdle,			// �ҋ@
		ePatrol,		// ����

		eBattleIdle,	// �ҋ@�i�퓬�j
		eChase,			// �ǐՒ�
		eLost,			// ������

		eAttack,		// �U��

		eGuard,			// �h��
		eAvoid,			// ���

		eKnockBack,		// �m�b�N�o�b�N
		eHit,			// ������
		eStun,			// ����
		eStunWait,		// �����҂�����
		eParried,		// �p���B���ꂽ��
		eDeath,			// ���S
	};
	//��Ԑ؂�ւ�
	void ChangeState(int state) override;

	// �U���^�C�v
	enum class EAttackType
	{
		eNone,			// �w��Ȃ�

		eBlowL,			// ���ガ����
		eBlowR,			// �E�ガ����
		eRoundKickL,	// ���񂵏R��
		eRoundKickR,	// �E�񂵏R��
		eTackleStart,	// �^�b�N���̗���
		eTackle,		// �^�b�N��
		eTackleEnd,		// �^�b�N���I�����̗\������
		eHeadButt,		// ���˂��U��
		eTripleAttack,	// �O�A�U��
	};
	// �U���^�C�v�؂�ւ�
	void ChangeAttackType(int attacktype) override;

	// TODO:�ҋ@�s���̃N���X�����

	// �_���[�W�v�Z
	void CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const;

	/// <summary>
	/// �U���o���邩�ǂ���
	/// </summary>
	/// <param name="range">�U���͈͂�ݒ�</param>
	/// <returns>�U���͈͂������Ftrue</returns>
	bool CanAttackPlayer(float range) const;

	// �ړ�����U���͈͂��H
	bool IsMoveAttackRange();

	// �v���C���[�̍U�������m�������H
	bool IsPlayerAttackDetected() const;
	// �v���C���[�̍U���͈͓����H
	bool IsPlayerAttackRange() const;

	/// <summary>
	/// �v���C���[�̍U�������m���̏���
	/// </summary>
	/// <returns>true�̏ꍇ�́A��Ԃ��ς����</returns>
	bool DetectedPlayerAttack();

	// �ǂ̍U�������邩���肷��
	void AttackPickDetect();

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// ���̐��ʕ����x�N�g�����擾
//	CVector GetHeadForwardVec() const;

	// ���ɏ��񂷂�|�C���g��ύX
	bool ChangePatrolPoint();
	// ���񃋁[�g���X�V����
	bool UpdatePatrolRoute();


	// ���X�e�[�g�x�[�XAI��

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// ���񒆂̍X�V����
	void UpdatePatrol();

	// �퓬�ҋ@��Ԃ̍X�V����
	void UpdateBattleIdle();
	// �ǐՒ��̍X�V����
	void UpdateChase();
	// �����������̍X�V����
	void UpdateLost();

	// �U�����̍X�V����
	void UpdateAttack();

	// �h�䎞�̍X�V����
	void UpdateGuard();
	// ������̍X�V����
	void UpdateAvoid();


	// �����莞�̍X�V����
	void UpdateHit();
	// �������̍X�V����
	void UpdateStun();
	// �����҂��̍X�V����
	void UpdateStunWait();
	// �p���B���ꂽ���̍X�V����
	void UpdateParried();
	// ���S���̍X�V����
	void UpdateDeath();


	// ���U�����Ƃ̍X�V������

	// �ガ�����U��
	void UpdateBlowL();
	void UpdateBlowR();
	// �񂵏R��U��
	void UpdateRoundKickL();
	void UpdateRoundKickR();
	// �^�b�N��
	void UpdateTackleStart();
	void UpdateTackle();
	void UpdateTackleEnd
	();
	// �����o���U��
	void UpdateHeadButt();
	// �O�A�U��
	void UpdateTripleAttack();


	// �ҋ@�s���̍X�V����
	// ���s�������R�ɂ���ׂ̍X�V������

	// ���ړ�(horizontal)
	void UpdateHorizonMove();
	// �O�i�s��(forward)
	void UpdateForwardMove();
	// ��ލs��(back)
	void UpdateBackMove();

	// ��Ԃ̕�������擾
	std::string GetStateStr(int state) const;
	// �U���^�C�v�̕�����擾
	std::string GetAttackTypeStr(int state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(int state) const;

	EAnimType GetAnimType() const;
	int mAvoidAnim;

	CDebugFieldOfView* mpDebugAttack;  // �U���͈͂̃f�o�b�O�\��
	// �v���C���[�̍U���͈͂̊p�x
	float mPlayerAttackAngle;
	// �v���C���[�̍U���͈͂̋���
	float mPlayerAttackLength;

	CDebugFieldOfView* mpDebugFov;  // ����͈͂̃f�o�b�O�\��

	float mRandMoveAngle;	// �����_���̈ړ��p�x
	int mRandHitAnim;		// �����_���ȋ�����A�j���[�V����

	CVector mMoveStartPos; // �U���J�n���̈ʒu
	CVector mMoveEndPos;   // �U���I�����̈ʒu


	// ����̋��R���C�_
	CColliderSphere* mpLHandCol; 
	// �E��̋��R���C�_
	CColliderSphere* mpRHandCol;
	// �����̋��R���C�_
	CColliderSphere* mpLFootCol;
	// �E���̋��R���C�_
	CColliderSphere* mpRFootCol;
	// �U���p�̃R���C�_�S�i���̕����j
	CColliderSphere* mpHeadCol;

	// �퓬����
	CObjectBase* mpBattleTarget;

#if _DEBUG
	CMatrix mHeadForwardMtx;
#endif


	bool mIsBattle;			// �퓬��Ԃ�
	bool mIsGuard;			// �K�[�h��Ԃ�
	bool mIsAvoid;			// �����Ԃ�
	bool mIsTripleAttack;	// �O�A�U����Ԃ�

	int mAttackCount;		// ���̍U���̉�
	int mTackleCount;		// �^�b�N�����o����܂ł̃J�E���g

	// �v���C���[�̍U�������Ɍ��m�ς݂ł���
	bool mIsDetectedPlayerAttack;
	

	// �v���C���[�����������ʒu�̃m�[�h
	CNavNode* mpLostPlayerNode; 
	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	// ���߂��ŒZ�o�H�L���p
	std::vector<CNavNode*> mMoveRoute;
	// ���ɏ��񂷂�|�C���g�̔ԍ�
	int mNextPatrolIndex;
	// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l
	int mNextMoveIndex;		
};
#endif
