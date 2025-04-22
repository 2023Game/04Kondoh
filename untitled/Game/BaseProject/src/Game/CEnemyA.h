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
	CEnemyA(std::vector<CVector> patrolPoints);
	~CEnemyA();
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

	// �_���[�W���󂯂�
	void TakeDamage(int damage,float stan, CObjectBase* causer) override;
	// ���S
	void Death() override;
	// ���ݏ���
	void Stan() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:

	// �_���[�W�v�Z
	void CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,			// T�|�[�Y
		eIdle,			// �ҋ@
		eBattleIdle,	// �퓬�ҋ@
		eWalk,			// ���s
		eRun,			// ����
		eDeath,			// ���S
		eStan,			// �C��A�j���[�V����

		eBlowL,			// ���ガ����
		eBlowR,			// �E�ガ����
		eRoundKickL,	// ���񂵏R��
		eRoundKickR,	// �E�񂵏R��
		eTackle,		// ���������r�^�b�N��
		eHeadButt,		// �����o���U��

		eHit1,			// ������1
		eHit2,			// ������2
		eHit3,			// ������3
		eChanceHit1,	// �`�����X���̋�����1
		eChanceHit2,	// �`�����X���̋�����2
		eChanceHit3,	// �`�����X���̋�����3
		eChanceHit4,	// �`�����X���̋�����4

		Num
	};

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

		eStan,			// ����
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
		eTackle,		// ���������r�^�b�N��
		eHeadButt,		// ���˂��U��
		eTripleAttack,	// �O�A�U��
	};
	// �U���^�C�v�؂�ւ�
	void ChangeAttackType(int attacktype) override;

	// �v���C���[������͈͓��ɓ��������ǂ���
	bool IsFoundPlayer() const;
	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookPlayer() const;

	/// <summary>
	/// �U���o���邩�ǂ���
	/// </summary>
	/// <param name="range">�U���͈͂�ݒ�</param>
	/// <returns>�U���͈͂������Ftrue</returns>
	bool CanAttackPlayer(float range) const;

	// �U�����Ɉړ����鋗����
	bool AttackRangeMin();

	// �v���C���[�̍U�������m�������H
	bool IsPlayerAttackDetected() const;

	/// <summary>
	/// �v���C���[�̍U�������m���̏���
	/// </summary>
	/// <returns>true�̏ꍇ�́A��Ԃ��ς����</returns>
	bool DetectedPlayerAttack();

	// �ǂ̍U�������邩���肷��
	void AttackPickDetect();

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);
	// �����_���ňړ�
	bool RandMove(float speed);

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// ���ɏ��񂷂�|�C���g��ύX
	void ChangePatrolPoint();

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

	// ���S���̍X�V����
	void UpdateDeath();
	// ���ݎ��̍X�V����
	void UpdateStan();

	// ���U�����Ƃ̍X�V������

	// �ガ�����U��
	void UpdateBlowL();
	void UpdateBlowR();
	// �񂵏R��U��
	void UpdateRoundKickL();
	void UpdateRoundKickR();
	// ���������r�^�b�N��
	void UpdateTackle();
	// �����o���U��
	void UpdateHeadButt();
	// �O�A�U��
	void UpdataTripleAttack();


	// ��Ԃ̕�������擾
	std::string GetStateStr(int state) const;
	// �U���^�C�v�̕�����擾
	std::string GetAttackTypeStr(int state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(int state) const;

	CDebugFieldOfView* mpDebugAttack;  // �U���͈͂̃f�o�b�O�\��
	// �v���C���[�̍U���͈͂̊p�x
	float mPlayerAttackAngle;
	// �v���C���[�̍U���͈͂̋���
	float mPlayerAttackLength;

	CDebugFieldOfView* mpDebugFov;  // ����͈͂̃f�o�b�O�\��
	float mFovAngle;                // ����͈͂̊p�x
	float mFovLength;                // ����͈͂̋���

	CNavNode* mpLostPlayerNode;  // �v���C���[�����������ʒu�̃m�[�h

	CVector mAttackStartPos; // �U���J�n���̈ʒu
	CVector mAttackEndPos;   // �U���I�����̈ʒu

	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex; // ���ɏ��񂷂�|�C���g�̔ԍ�


	// ����̋��R���C�_
	CColliderSphere* mpLAttackCol; 
	// �E��̋��R���C�_
	CColliderSphere* mpRAttackCol;

	std::vector<CNavNode*> mMoveRoute;	// ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l

	CObjectBase* mpBattleTarget;	// �퓬����
	bool mIsBattle;					// �퓬��Ԃ�

	int mAttackCount;		// �U���̉�
	bool mIsTripleAttack;	// �O�A�U����Ԃ�

	/*
	int mpDetectType;	// �U���^�C�v
	bool mIsParry;	// �X�^�����Ă��邩
	*/

	// �v���C���[�̍U�������Ɍ��m�ς݂ł���
	bool mIsDetectedPlayerAttack;
};
#endif
