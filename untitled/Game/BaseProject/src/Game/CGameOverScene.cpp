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
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ここでゲームオーバ画面に必要な
	// リソースの読み込みやクラスの生成を行う

	// ゲームオーバBGMを再生

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
	// ゲーム開始ならば、ゲームシーンを読み込む
	if (mpGameOverUI->IsEnd())
	{
		// ゲーム開始
		if (mpGameOverUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// ゲーム終了ならば、アプリを閉じる
		else if (mpGameOverUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
