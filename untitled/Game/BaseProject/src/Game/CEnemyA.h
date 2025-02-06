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
	void TakeDamage(int damage, CObjectBase* causer) override;
	// ���S
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
		eWalk,			// ���s
		eRun,			// ����
		eRightAttack,	// �U��
		eDeath,			// ���S
		eStan,			// �C��
		eChance,		// �U���`�����X�I�I

		Num
	};

	// �G�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		ePatrol,	// ����
		eChase,		// �ǐՒ�
		eLost,		// ������
		eAttack,	// �U��
		eDeath,		// ���S
		eStan,		// �C��
		eChance,	// �U���`�����X�I�I
	};
	//��Ԑ؂�ւ�
	void ChangeState(int state) override;

	enum class EAttackType
	{

		eLeftAttackS,	// ����U��
		eLeftAttackM,	// �����U��
		eRightAttackS,	// �E��U��
		eRightAttackM,	// �E���U��
	};
	// �U���^�C�v�؂�ւ�
	void ChangeAttackType(int attacktype) override;

	// �v���C���[������͈͓��ɓ��������ǂ���
	bool IsFoundPlayer() const;
	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookPlayer() const;
	// �v���C���[���U���o���邩�ǂ���
	bool CanAttackPlayer() const;
	// �U�����Ɉړ����鋗����
	bool AttackRangeMin();
	// �X�^�����邩
	bool IsStan();

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);
	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// ���ɏ��񂷂�|�C���g��ύX
	void ChangePatrolPoint();

	// ���X�e�[�g�x�[�XAI��

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// ���񒆂̍X�V����
	void UpdatePatrol();
	// �ǐՒ��̍X�V����
	void UpdateChase();
	// �����������̍X�V����
	void UpdateLost();
	// �U�����̍X�V����
	void UpdateAttack();
	// ���S���̍X�V����
	void UpdateDeth();
	// �C�⎞�̍X�V����
	void UpdateStan();
	// �A�^�b�N�`���[���X�I�I�̍X�V����
	void UpdateChance();

	// ��Ԃ̕�������擾
	std::string GetStateStr(int state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(int state) const;


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
};
#endif
