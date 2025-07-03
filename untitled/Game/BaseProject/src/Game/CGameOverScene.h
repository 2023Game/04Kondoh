#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CSound;
class CGameOverUI;

class CGameOverScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameOverScene();
	// �f�X�g���N�^
	~CGameOverScene();
	// �V�[���ǂݍ���
	void Load();
	// �V�[���̍X�V����
	void Update();

private:
	CGameOverUI* mpGameOverUI;
};