#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;
class CNavNode;

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemyA : public CXCharacter
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

		eTPose,		     // T�|�[�Y
		eIdle,		     // �ҋ@
		eWalk,		     // ���s
		eRun,		     // ����
		eRightAttack,    // �U��

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type, bool restart = false);

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �G�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		ePatrol,    // ����
		eChase,     // �ǐՒ�
		eLost,      // ������
		eAttack     // �U��
	};
	//��Ԑ؂�ւ�
	void ChangeState(EState state);

	// �v���C���[������͈͓��ɓ��������ǂ���
	bool IsFoundPlayer() const;
	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookPlayer() const;
	// �v���C���[���U���o���邩�ǂ���
	bool CanAttackPlayer() const;
	// �U�����Ɉړ����鋗����
	bool AttackRangeMin();

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);

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

	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state)const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(EState state) const;

	EState mState;	// �G�̏��
	int mStateStep; // ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime; // �o�ߎ��Ԍv���p

	CDebugFieldOfView* mpDebugFov;  // ����͈͂̃f�o�b�O�\��
	float mFovAngle;                // ����͈͂̊p�x
	float mFovLength;                // ����͈͂̋���

	CNavNode* mpLostPlayerNode;  // �v���C���[�����������ʒu�̃m�[�h

	CVector mAttackStartPos; // �U���J�n���̈ʒu
	CVector mAttackEndPos;   // �U���I�����̈ʒu

	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex; // ���ɏ��񂷂�|�C���g�̔ԍ�

	//CColliderLine* mpColliderLine;  // �c�����̐����R���C�_�[
	//CColliderLine* mpColliderLineX; // �������iX���j�̐����R���C�_�[
	//CColliderLine* mpColliderLineZ; // �������iZ���j�̐����R���C�_�[
	CColliderCapsule* mpColliderCapsule;  // �J�v�Z���R���C�_�[

	std::vector<CNavNode*> mMoveRoute; // ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;                // ���Ɉړ�����m�[�h�̃C���f�b�N�X�l
};
#endif
