#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;

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
	void ChangeAnimation(EAnimType type);

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

	CVector mLostPlayerPos; // �v���C���[�������������W

	CVector mAttackStartPos; // �U���J�n���̈ʒu
	CVector mAttackEndPos;   // �U���I�����̈ʒu

	// ����|�C���g�̃��X�g
	std::vector<CVector> mPatrolPoints;
	int mNextPatrolIndex; // ���ɏ��񂷂�|�C���g�̔ԍ�
};
#endif
