#ifndef CENEMYA_H
#define CENEMYA_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemyA : public CXCharacter
{
public:
	// �R���X�g���N�^
	CEnemyA();
	~CEnemyA();
	// �X�V����
	void Update() override;
	// �`��
	void Render() override;

private:

	// �ҋ@���
	void UpdateIdle();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ����

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

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
	};
	EState mState;	// �G�̏��


};

#endif
