#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CColliderCapsule.h"
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
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

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
	CVector CalcMoveVec() const;

	// ��퓬���̑ҋ@���
	void UpdateIdle();

	// �퓬���̑ҋ@���
	void UpdateAttackIdle();
	// �U��
	void UpdateAttack();
	// �U���I���҂�
	void UpdateAttackWait();

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
	void UpdateMotionBlur();

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
	void ChangeAnimation(EAnimType type);

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

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
	EState mState;	// �v���C���[�̏��

	// �U������
	enum class EAttackWay
	{
		eIdle,         // �ҋ@

		eUpAttackS,	   // ���U��
		eUpAttackM,	   // ����U��
		eUpAttackL,	   // ����U��

		eDwonAttackS,  // �㉺�U��
		eDwonAttackM,  // �����U��
		eDwonAttackL,  // �����U��

		eRightAttackS, // ��E�U��
		eRightAttackM, // ���E�U��
		eRightAttackL, // ���E�U��

		eLeftAttackS,  // �㍶�U��
		eLeftAttackM,  // �����U��
		eLeftAttackL,  // �����U��,
	};
	EAttackWay mAttackWay; // �U������

	// �U���̋���
	enum class EAttackPower
	{
		eAttackS,   // ��U��
		eAttackM,   // ���U��
		eAttackL,   // ���U��

		Num,        // �U���̋����̐�
	};
	EAttackPower mAttackPower; // �U���̋���

	// ���[�h�I��
	enum class EMode
	{
		eNotBattle, // ��퓬���[�h
		eBattle,    // �퓬���[�h
	};
	EMode mMode;  //���[�h�I��

	bool IsBattleMode;

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	CColliderLine* mpColliderLine;  // �c�����̐����R���C�_�[
	CColliderLine* mpColliderLineX; // �������iX���j�̐����R���C�_�[
	CColliderLine* mpColliderLineZ; // �������iZ���j�̐����R���C�_�[
	CColliderCapsule* mpColliderCapsule;  //�J�v�Z���R���C�_�[
	CTransform* mpRideObject;

	CVector mEvaStartPos; // ����J�n���̈ʒu
	CVector mEvaEndPos;   // ����I�����̈ʒu

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// �Ή����˃G�t�F�N�g
	CFlamethrower* mpFlamethrower;

	// ���[�V�����u���[���|����c�莞��
	float mMotionBlurRemainTime;
};
