#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CClearUI;

class CClearScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CClearScene();
	// �f�X�g���N�^
	~CClearScene();
	// �V�[���ǂݍ���
	void Load();
	// �V�[���̍X�V����
	void Update();

private:
	CClearUI* mpClearUI;

};
