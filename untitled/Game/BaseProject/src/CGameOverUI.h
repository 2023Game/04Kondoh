#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CGameOverUI : public CTask
{
public:
	// �R���X�g���N�^
	CGameOverUI();
	// �f�X�g���N�^
	~CGameOverUI();

	// �Q�[���I�[�o��ʏI����
	bool IsEnd() const;
	// �Q�[�����J�n���邩
	bool IsStartGame() const;
	// �Q�[�����I�����邩
	bool IsExitGame() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:

	// �ҋ@���
	void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();

	// �Q�[���I�[�o�̏��
	enum class EState
	{
		eOpen,		// �ҋ@���
		eSelect,	// ���j���[�I��
	};
	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [CONTINUE]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickContinue();
	// [QUIT]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();

	EState mState;
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ�
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �Q�[���I�[�o��ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CImage* mpGameOverBg;	// �Q�[���I�[�o�w�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};
