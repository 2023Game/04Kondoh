#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

class CImage;
class CExpandButton;

class CGameClear : public CTask
{
public:
	// �R���X�g���N�^
	CGameClear();
	// �f�X�g���N�^
	~CGameClear();

	// �N���A��ʏI����
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

	enum class EState
	{
		eOpen,
		eSelect
	};
	void ChangeState(EState state);

	// �ҋ@���
	void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();

	// [ReStart]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickReStart();
	// [QUIT]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();

	EState mState;		
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ�
	int mSelectIndex;	// ���ݑI�����Ă��鍀��
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �N���A��ʏI���t���O

	CImage* mpBackground;	// �Q�[���N���A�w�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};
