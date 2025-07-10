#include "CGameOverScene.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CGameOverUI.h"

CGameOverScene::CGameOverScene()
	: CSceneBase(EScene::eGameOver)
	, mpGameOverUI(nullptr)
{
}

CGameOverScene::~CGameOverScene()
{
}

void CGameOverScene::Load()
{
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// �����ŃQ�[���I�[�o��ʂɕK�v��
	// ���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// �Q�[���I�[�oBGM���Đ�

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);
	mpGameOverUI = new CGameOverUI();
	AddTask(mpGameOverUI);

}

void CGameOverScene::Update()
{
	// �Q�[���J�n�Ȃ�΁A�Q�[���V�[����ǂݍ���
	if (mpGameOverUI->IsEnd())
	{
		// �Q�[���J�n
		if (mpGameOverUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpGameOverUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
